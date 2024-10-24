#include"../Manager/InputManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include "PlayerBase.h"
PlayerBase::PlayerBase(void)
{
	atk_.ResetCnt();
	atk_.duration_ = FRAME_ATK_DURATION;
	atk_.backlash_ = FRAME_ATK_BACKRASH;
	atk_.pow_ = 0;
}

void PlayerBase::Destroy(void)
{
}

void PlayerBase::SetParam(void)
{

}

void PlayerBase::Init(void)
{
	dodgeCdt_ = DODGE_CDT_MAX;
	speedMove_ = 0.0f;
	ChangeControll(CNTL::KEYBOARD);

	//アニメーション初期化
	InitAnimNum();
	ResetAnim(ANIM::WALK, SPEED_ANIM_IDLE);

	//モデルの初期化
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	float scale = 0.5f;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { 0.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);

	trans_.Update();
}

void PlayerBase::Update(void)
{
	//アニメーションの更新
	Anim();

	//モデルの更新
	trans_.Update();

	//停止状態の時のアニメーション
	if (!IsMove() && !IsDodge() && !IsAtkAction() &&!IsSkill(SKILL::ONE))
	{
		ResetAnim(ANIM::IDLE,SPEED_ANIM_IDLE);
		speedMove_ = 0.0f;
	}

	//回避
	Dodge();

	switch (cntl_)
	{
	case PlayerBase::CNTL::KEYBOARD:
		KeyBoardControl();
		break;
	case PlayerBase::CNTL::PAD:
		GamePad();
		break;
	}


	//攻撃中か毎フレーム判定する
	Attack();

	Skill_1();

}

void PlayerBase::Draw(void)
{
	MV1DrawModel(trans_.modelId);
	DrawDebug();
}

void PlayerBase::Move(float _deg, VECTOR _axis)
{
	speedMove_ = SPEED_MOVE;
	if (!IsDodge() && !IsAtkAction())
	{
		ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);
		Turn(_deg, _axis);
		VECTOR dir = trans_.GetForward();
		//移動方向
		VECTOR movePow = VScale(dir, speedMove_);
		//移動処理
		trans_.pos = VAdd(trans_.pos, movePow);
	}
}

void PlayerBase::InitAnimNum(void)
{
	animNum_.emplace(ANIM::IDLE, IDLE_NUM);
	animNum_.emplace(ANIM::WALK, RUN_NUM);
	animNum_.emplace(ANIM::DODGE, DODGE_NUM);
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_NUM);
}

void PlayerBase::KeyBoardControl(void)
{
	auto& ins = InputManager::GetInstance();
	DINPUT_JOYSTATE input;
	if (GetJoypadInputState(DX_INPUT_PAD1)&&CheckHitKeyAll() < 0)
	{
		ChangeControll(CNTL::PAD);
	}
	//前
	if (ins.IsNew(KEY_INPUT_W))
	{
		Move(0.0f, AsoUtility::AXIS_Y);
	}
	//右
	else if (ins.IsNew(KEY_INPUT_D))
	{
		Move(90.0f, AsoUtility::AXIS_Y);
	}
	//下
	else if (ins.IsNew(KEY_INPUT_S))
	{
		Move(180.0f, AsoUtility::AXIS_Y);
	}
	//左
	else if (ins.IsNew(KEY_INPUT_A))
	{
		Move(270.0f, AsoUtility::AXIS_Y);
	}
	else
	{
		speedMove_ = 0.0f;
	}
	

	
	

	//攻撃（攻撃アニメーションのフレームが0以下だったらフレームを設定）
	if (ins.IsTrgDown(KEY_INPUT_E) && !atk_.IsAttack() && !IsDodge())
	{
		//アニメーション
		ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);
		//カウンタ開始
		Count(atk_.cnt_);
	}

	if (ins.IsTrgDown(KEY_INPUT_Q)&&!IsAtkAction()/*&&!IsSkill(SKILL::TWO)*/)
	{
		Count(skillCnt_[SKILL::ONE]);
		ResetAnim(ANIM::SKILL_1,SPEED_ANIM);
	}

	//回避
	if (ins.IsTrgDown(KEY_INPUT_N) && !IsDodge() && !IsAtkAction()&&!IsCoolDodge())
	{
		ResetAnim(ANIM::DODGE,SPEED_ANIM_DODGE);
		Count(frameDodge_);
	}

}

void PlayerBase::GamePad(void)
{
	auto& ins = InputManager::GetInstance();
	if (GetJoypadNum() == 0)
	{
		ChangeControll(CNTL::KEYBOARD);
	}

	// 左スティックの横軸
	leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;

	//縦軸
	leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;

	//方向決め
	auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));
	stickDeg_ = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(stickRad) + 90.0f));
	//前
	if (leftStickY_ < -1)
	{
		Move(stickDeg_, AsoUtility::AXIS_Y);
	}
	//右
	else if (leftStickX_ > 1)
	{
		Move(stickDeg_, AsoUtility::AXIS_Y);
	}
	//下
	else if (leftStickY_ > 1)
	{
		Move(stickDeg_, AsoUtility::AXIS_Y);
	}
	//左
	else if (leftStickX_ < -1)
	{
		Move(stickDeg_, AsoUtility::AXIS_Y);
	}
	else
	{
		speedMove_ = 0.0f;
	}

	//攻撃（攻撃アニメーションのフレームが0以下だったらフレームを設定）
	if ( ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1,InputManager::JOYPAD_BTN::RIGHT) && !atk_.IsAttack() && !IsDodge())
	{
		//アニメーション
		ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);
		//カウンタ開始
		Count(atk_.cnt_);
	}

	//スキル
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::TOP) && !IsAtkAction()/*&&!IsSkill(SKILL::TWO)*/)
	{
		Count(skillCnt_[SKILL::ONE]);
		ResetAnim(ANIM::SKILL_1, SPEED_ANIM);
	}

	//回避
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT) && !IsDodge() && !IsAtkAction() && !IsCoolDodge())
	{
		ResetAnim(ANIM::DODGE, SPEED_ANIM_DODGE);
		Count(frameDodge_);
	}
}

void PlayerBase::ChangeControll(CNTL _cntl)
{
	//if (cntl_ == _cntl) { return; }
	cntl_ = _cntl;
	switch (cntl_)
	{
	case PlayerBase::CNTL::KEYBOARD:
		break;
	case PlayerBase::CNTL::PAD:
		break;
	}
}

void PlayerBase::DrawDebug(void)
{
	//球体
	//DrawSphere3D(trans_.pos, 20.0f, 8, 0x0, color_, true);
	//値見る用
	DrawFormatString(0, 0, 0xffffff
		, "FrameATK(%f)\nisAtk(%d)\nisBackSrash(%d)\nDodge(%f)\nSkill(%f)\nStick(%f)"
		, atk_.cnt_, atk_.IsAttack(),atk_.IsBacklash(), frameDodge_,skillCnt_[SKILL::ONE],stickDeg_);
}



void PlayerBase::Turn(float _deg, VECTOR _axis)
{
	//transform_.quaRot =
	//	transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));

	trans_.quaRot =
		trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}


void PlayerBase::Attack(void)
{
	if (atk_.IsAttack()||atk_.IsBacklash())
	{
		Count(atk_.cnt_);
	}
	else 
	{
		atk_.ResetCnt();
	}
	
}

void PlayerBase::Dodge(void)
{
	//ドッジフラグがtrueになったら
	if (IsDodge()&&!IsCoolDodge())
	{
		Count(frameDodge_);
		if (frameDodge_ < FRAME_DODGE_MAX)
		{
			VECTOR dir = trans_.GetForward();
			//移動方向
			VECTOR movePow = VScale(dir, SPEED_DODGE);
			//移動処理
			trans_.pos = VAdd(trans_.pos, movePow);
		}
		else
		{
			dodgeCdt_ = 0.0f;
		}
	}
	else
	{
		Count(dodgeCdt_);
		ResetDodgeFrame();
		color_ = 0xffffff;
	}
}

void PlayerBase::Skill_1(void)
{
	skillNum_ = "Skill_1";
	if (IsSkill(SKILL::ONE))
	{
		Count(skillCnt_[SKILL::ONE]);
	}
	else
	{
		skillCnt_[SKILL::ONE] = 0.0f;
	}
}

void PlayerBase::Skill_2(void)
{
	skillNum_ = "Skill_2";
}

void PlayerBase::Damage(void)
{
	ResetAnim(ANIM::DAMAGE,SPEED_ANIM_RUN);
}

