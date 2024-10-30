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

	skills_[SKILL_NUM::ONE].ResetCnt();
	skills_[SKILL_NUM::ONE].duration_ = FRAME_SKILL1_DURATION;
	skills_[SKILL_NUM::ONE].backlash_ = FRAME_ATK_BACKRASH;
	skills_[SKILL_NUM::ONE].pow_ = 0;

	skills_[SKILL_NUM::TWO].ResetCnt();
	skills_[SKILL_NUM::TWO].duration_ = FRAME_SKILL2_DURATION;
	skills_[SKILL_NUM::TWO].backlash_ = FRAME_ATK_BACKRASH;
	skills_[SKILL_NUM::TWO].pow_ = 0;
}

void PlayerBase::Destroy(void)
{
}

void PlayerBase::SetParam(void)
{
	atk_.pos_ = VAdd(trans_.pos, ATK_COL_LOCAL_POS);
	atk_.ResetCnt();
	atk_.duration_ = FRAME_ATK_DURATION;
	atk_.backlash_ = FRAME_ATK_BACKRASH;
	atk_.pow_ = 0;

	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	float scale = SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { 0.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);

	hp_ = MAX_HP;
}

void PlayerBase::Init(void)
{
	SetParam();



	InitDebug();


	dodgeCdt_ = DODGE_CDT_MAX;
	speedMove_ = 0.0f;
	ChangeControll(CNTL::KEYBOARD);

	//アニメーション初期化
	InitAnimNum();
	ResetAnim(ANIM::WALK, SPEED_ANIM_IDLE);

	//モデルの初期化
	




	trans_.Update();
}

void PlayerBase::Update(void)
{
	//共通処理
	Common();

	//停止状態の時のアニメーション
	if (!IsMove() && !IsDodge() && !IsAtkAction() && !IsSkillAll())
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
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

	switch (skillNo_)
	{
	case PlayerBase::SKILL_NUM::ONE:
		Action(skills_[SKILL_NUM::ONE]);
		break;
	case PlayerBase::SKILL_NUM::TWO:
		Action(skills_[SKILL_NUM::TWO]);
		break;
	default:
		break;
	}


	//攻撃中か毎フレーム判定する
	//Attack();
	Action(atk_);
	SyncActPos(atk_, ATK_COL_LOCAL_POS);
	SyncActPos(skills_[SKILL_NUM::ONE], SKILL1_COL_LOCAL_POS);
	SyncActPos(skills_[SKILL_NUM::TWO], SKILL2_COL_LOCAL_POS);





}

void PlayerBase::Draw(void)
{
	MV1DrawModel(trans_.modelId);
	DrawDebug();
}



void PlayerBase::Move(float _deg, VECTOR _axis)
{
	speedMove_ = SPEED_MOVE;
	if (!IsDodge() && !IsAtkAction()&&!IsSkillAll())
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
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
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

	if (ins.IsTrgDown(KEY_INPUT_Q)&&!IsAtkAction()&&!IsDodge())
	{
		Count(skillCnt_[SKILL_NUM::ONE]);
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
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::TOP) && !IsAtkAction()&&!IsSkillAll())
	{
		//アニメーション
		SkillAnim();
		Count(skills_[skillNo_].cnt_);
	}

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER)&&!IsSkillAll())
	{
		skillNo_ = static_cast<SKILL_NUM>(static_cast<int>(skillNo_) + 1);
		if (skillNo_ == SKILL_NUM::MAX)
		{
			skillNo_ = SKILL_NUM::ONE;
		}
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

void PlayerBase::Common(void)
{
	//アニメーションの更新
	Anim();

	//モデルの更新
	trans_.Update();

	
}

void PlayerBase::DrawDebug(void)
{
	const unsigned int ATK_COLOR = 0xff0000;
	//球体
	//DrawSphere3D(trans_.pos, 20.0f, 8, 0x0, color_, true);
	//値見る用
	DrawFormatString(0, 0, 0xffffff
		, "FrameATK(%f)\nisAtk(%d)\nisBackSrash(%d)\nDodge(%f)\nSkill(%f)\nSkillNum(%d)\nStick(%f)\nHP(%d)"
		, atk_.cnt_, atk_.IsAttack(),atk_.IsBacklash(), frameDodge_,skills_[SKILL_NUM::ONE].cnt_, skillNo_, stickDeg_, hp_);

	DrawSphere3D(VAdd(trans_.pos, {0.0f,20.0f,0.0f}), SCALE * 100, 8, color_Col_, color_Col_, false);
	DrawSphere3D(atk_.pos_, COL_ATK, 8, color_Atk_, color_Atk_, false);
	DrawSphere3D(skills_[SKILL_NUM::ONE].pos_, COL_SKILL1, 8, color_skl1_, color_skl1_, false);
	DrawSphere3D(skills_[SKILL_NUM::TWO].pos_, COL_SKILL2, 8, color_skl2_, color_skl2_, false);

	if (atk_.IsAttack())
	{
		color_Atk_ = ATK_COLOR;
	}
	else
	{
		color_Atk_ = 0x00ffff;
	}

	if (skills_[SKILL_NUM::ONE].IsAttack())
	{
		color_skl1_ = ATK_COLOR;
	}
	else
	{
		color_skl1_ = 0x00ffff;
	}

	if (skills_[SKILL_NUM::TWO].IsAttack())
	{
		color_skl2_ = ATK_COLOR;
	}
	else
	{
		color_skl2_ = 0x00ffff;
	}
}



void PlayerBase::Turn(float _deg, VECTOR _axis)
{
	//transform_.quaRot =
	//	transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));

	trans_.quaRot =
		trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}


void PlayerBase::Action(ATK& _act)
{
	if (_act.IsAttack() || _act.IsBacklash())
	{
		Count(_act.cnt_);
	}
	else
	{
		_act.ResetCnt();
	}
}

void PlayerBase::SyncActPos(ATK& _act,VECTOR _colPos)
{
	//追従対象の位置
	VECTOR followPos = trans_.pos;

	//追従対象の向き
	Quaternion followRot = trans_.rot;

	//追従対象の角度
	VECTOR relativeActPos = followRot.PosAxis(_colPos);
	_act.pos_ = VAdd(trans_.pos, relativeActPos);
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
		color_Col_ = 0xffffff;
	}
}

void PlayerBase::Skill_1(void)
{
}

void PlayerBase::Skill_2(void)
{
}

void PlayerBase::SkillAnim(void)
{
	switch (skillNo_)
	{
	case PlayerBase::SKILL_NUM::ONE:
		ResetAnim(ANIM::SKILL_1, SPEED_ANIM_ATK);
		break;
	case PlayerBase::SKILL_NUM::TWO:
		ResetAnim(ANIM::SKILL_2, SPEED_ANIM_ATK);
		break;
	case PlayerBase::SKILL_NUM::MAX:
		break;
	default:
		break;
	}
}

void PlayerBase::Damage(void)
{
	hp_--;

	const unsigned int  DMG_COLOR = 0x00ffff;
	color_Col_ = DMG_COLOR;
}

void PlayerBase::InitDebug(void)
{
	color_Col_ = 0xffffff;
	color_Atk_ = 0x00ffff;
	color_skl1_ = 0x00ffff;
	color_skl2_ = 0x00ffff;
}

