#include"../PlayerInput.h"
#include"../PlayerDodge.h"
#include"../../../Manager/Generic/InputManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(void)
{
	skillNo_ = ATK_ACT::SKILL1;

	//dodgeCdt_ = DODGE_CDT_MAX;
	dodge_ = new PlayerDodge();
	dodge_->Init();

	moveSpeed_ = 0.0f;

	userOnePos_ = { -400.0f,0.0f,0.0f };


	atk_.isHit_ = false;

	multiHitInterval_ = 0.0f;

	atkStartCnt_ = 0.0f;

	isPush_ = false;

	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		coolTime_[i] = coolTimeMax_[i];
	}
}

void PlayerBase::Destroy(void)
{

}



void PlayerBase::Init(void)
{
	//アニメーション初期化
	InitAnimNum();
	InitCharaAnim();
	ResetAnim(ANIM::NONE, SPEED_ANIM);
	SetParam();

	InitAct();
#ifdef DEBUG_ON
	InitDebug();
#endif // DEBUG_ON
	//関数ポインタ初期化

	changeCntl_.emplace(SceneManager::CNTL::KEYBOARD, std::bind(&PlayerBase::ChangeKeyBoard, this));
	changeCntl_.emplace(SceneManager::CNTL::PAD, std::bind(&PlayerBase::ChangeGamePad, this));

	changeAct_.emplace(ATK_ACT::ATK, std::bind(&PlayerBase::ChangeNmlAtk, this));
	changeAct_.emplace(ATK_ACT::SKILL1, std::bind(&PlayerBase::ChangeSkillOne, this));
	changeAct_.emplace(ATK_ACT::SKILL2, std::bind(&PlayerBase::ChangeSkillTwo, this));

#ifdef DEBUG_INPUT
	//キーボードとパッドのボタンを共通させる(これらはInputManagerで定義したものから使えるようにしたい)
	//----------------------------------------------------------------------------------------------------------------
	inputActionMap_["ATTACK"] = { {InputType::KEYBOARD,ATK_KEY},{InputType::PAD,RIGHT_BTN} };
	inputActionMap_["DODGE"] = { {InputType::KEYBOARD,DODGE_KEY},{InputType::PAD,LEFT_BTN} };
	inputActionMap_["SKILL"] = { {InputType::KEYBOARD,SKILL_KEY},{InputType::PAD,TOP_BTN} };
	inputActionMap_["SKILL_CHANGE"] = { {InputType::KEYBOARD,SKILL_CHANGE_KEY},{InputType::PAD,R_BUTTON} };

#endif // DEBUG_INPUT




	//攻撃の初期化(とりあえず通常攻撃で初期化しておく)
	ChangeAct(ATK_ACT::ATK);


	ChangeControll(SceneManager::CNTL::KEYBOARD);

	ChangeSkillControll(ATK_ACT::SKILL1);


	//ChangeChargeActControll();
	//ChangeNmlActControll();

	radius_ = MY_COL_RADIUS;

	skillNo_ = ATK_ACT::SKILL1;

	//dodgeCdt_ = DODGE_CDT_MAX;
	dodge_ = new PlayerDodge();
	dodge_->Init();

	moveSpeed_ = 0.0f;

	userOnePos_ = { -400.0f,0.0f,0.0f };


	atk_.isHit_ = false;

	multiHitInterval_ = 0.0f;

	atkStartCnt_ = 0.0f;

	isPush_ = false;

	PlayerInput::CreateInstance();

	//モデルの初期化
	trans_.Update();

	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		coolTime_[i] = coolTimeMax_[i];
	}
}

void PlayerBase::Update(void)
{
	//アニメーションの更新
	Anim();

#ifdef INPUT_DEBUG_ON
	InputUpdate();
#endif // INPUT_DEBUG_ON


	//座標のバックアップ
	prePos_ = trans_.pos;

	//モデルの更新
	trans_.Update();

	SyncActPos(atk_);

	colPos_ = VAdd(trans_.pos, VScale(PLAYER_COL_LOCAL_POS, CHARACTER_SCALE));

	UserUpdate();


#ifdef DEBUG_ON


#endif // DEBUG_ON
	//スキルが終わったらクールタイムのカウント開始
	CoolTimeCnt();

	////それぞれの更新(プレイヤーとCPU)
	//modeUpdate_();

	//各アクション(ATKやスキルの更新)
	Action();




#ifdef DEBUG_ON
	{
		auto& ins = InputManager::GetInstance();
		const float SPEED = 5.0f;
		if (ins.IsNew(KEY_INPUT_W)) { userOnePos_.z += SPEED; }
		if (ins.IsNew(KEY_INPUT_D)) { userOnePos_.x += SPEED; }
		if (ins.IsNew(KEY_INPUT_S)) { userOnePos_.z -= SPEED; }
		if (ins.IsNew(KEY_INPUT_A)) { userOnePos_.x -= SPEED; }



		// 左スティックの横軸
		leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;

		//縦軸
		leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;
		auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));
		stickDeg_ = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(stickRad) + 90.0f));
		//前
		if (leftStickY_ < -1) { userOnePos_.z += SPEED; }
		//右
		else if (leftStickX_ > 1) { userOnePos_.x += SPEED; }
		//下
		else if (leftStickY_ > 1) { userOnePos_.z -= SPEED; }
		//左
		else if (leftStickX_ < -1) { userOnePos_.x -= SPEED; }

	}

#endif // DEBUG_ON

}

void PlayerBase::Draw(void)
{
	MV1DrawModel(trans_.modelId);
#ifdef DEBUG_ON
	DrawDebug();

#endif // DEBUG_ON
#ifdef INPUT_DEBUG_ON
	if (IsPressed("ATTACK"))
	{
		DrawString(0, 32, "ATTACK is pressed", 0x000000);
	}
	if (IsPressed("DODGE"))
	{
		DrawString(0, 32, "DODGE is pressed", 0x000000);
	}
#endif // INPUT_DEBUG_ON


}



void PlayerBase::Move(float _deg, VECTOR _axis)
{
	if (!isAtk_&&!isSkill_)
	{
		ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);
	}
	if (!dodge_->IsDodge() && moveAble_)
	{
		moveSpeed_ = SPEED_MOVE;
		Turn(_deg, _axis);
		VECTOR dir = trans_.GetForward();
		//移動方向
		VECTOR movePow = VScale(dir, moveSpeed_);
		//移動処理
		trans_.pos = VAdd(trans_.pos, movePow);
	}
}

void PlayerBase::UserUpdate(void)
{
	//それぞれの操作更新
	cntlUpdate_();

	//それぞれの入力処理
	PlayerInput::GetInstance().Update(this,padNum_);
	//ProcessInput();

	//停止アニメーションになる条件
	if (!IsMove() && !dodge_->IsDodge() && 0.0f >= atkStartCnt_ &&!isAtk_&&!isSkill_)
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
		moveSpeed_ = 0.0f;
	}

	//操作関係
	ProcessAct();

	//回避
	//Dodge();
	dodge_->Update(trans_);
	if (dodge_->IsDodge() && !dodge_->IsCoolDodge()) {
		atk_.ResetCnt();
		atkStartCnt_ = 0.0f;
		isAtk_ = false;
		isSkill_ = false;
		moveAble_ = true;
	}


}


void PlayerBase::InitAnimNum(void)
{
	animNum_.emplace(ANIM::NONE, T_POSE_NUM);
	animNum_.emplace(ANIM::IDLE, IDLE_NUM);
	animNum_.emplace(ANIM::WALK, RUN_NUM);
	animNum_.emplace(ANIM::DODGE, DODGE_NUM);
	animNum_.emplace(ANIM::DAMAGE, DAMAGE_NUM);
	animNum_.emplace(ANIM::DEATH, DEATH_NUM);
}

void PlayerBase::InitCharaAnim(void)
{

}

void PlayerBase::InitAct(void)
{

}

void PlayerBase::ChangeAct(const ATK_ACT _act)
{
	//クールタイム中なら処理しない
	if (isCool_[static_cast<int>(act_)] && !IsAtkable())return;
	act_ = _act;


	//変更点
	changeAct_[_act]();
	//ChangeAtkType(_act);
	//atkStartCnt_をここで開始させる
	//CntUp(atkStartCnt_);
}

void PlayerBase::ChangeNmlAtk(void)
{
	ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);
	//actUpdate_ = std::bind(&PlayerBase::AtkFunc, this);
}

void PlayerBase::ChangeSkillOne(void)
{
	ResetAnim(ANIM::SKILL_1, SPEED_ANIM_ATK);
	isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
	//actUpdate_ = std::bind(&PlayerBase::Skill1Func, this);
}

void PlayerBase::ChangeSkillTwo(void)
{
	ResetAnim(ANIM::SKILL_2, SPEED_ANIM_ATK);
	//actUpdate_ = std::bind(&PlayerBase::Skill2Func, this);
}

void PlayerBase::ResetParam(ATK& _atk)
{
	_atk = atkMax_[act_];
}

void PlayerBase::KeyBoardControl(void)
{
	auto& ins = InputManager::GetInstance();
	if (ins.IsNew(PlayerInput::MOVE_FRONT_KEY)) { moveDeg_ = 0.0f; }
	else if (ins.IsNew(PlayerInput::MOVE_LEFT_KEY)) { moveDeg_ = 270.0f; }
	else if (ins.IsNew(PlayerInput::MOVE_BACK_KEY)) { moveDeg_ = 180.0f; }
	else if (ins.IsNew(PlayerInput::MOVE_RIGHT_KEY)) { moveDeg_ = 90.0f; }
}

void PlayerBase::GamePad(void)
{
	auto& ins = PlayerInput::GetInstance();
	moveDeg_ = ins.GetStickDeg();
}

void PlayerBase::ResetGuardCnt(void)
{

}

void PlayerBase::ChangeControll(SceneManager::CNTL _cntl)
{
	cntl_ = _cntl;
	changeCntl_[cntl_]();
}


#ifdef DEBUG_ON
void PlayerBase::DrawDebug(void)
{
	const unsigned int ATK_COLOR = 0xff0000;
	//球体
	DrawSphere3D(trans_.pos, 20.0f, 8, 0x0, 0xff0000, true);
	//値見る用
	DrawFormatString(0, 32, 0xffffff
		, "FrameATK(%f)\nisAtk(%d)\nisBackSrash(%d)\nDodge(%f)\nSkill(%f)\natkStartTime(%f)\natkStartCnt(%f)\nskillType(%d)"
		, atk_.cnt_, atk_.IsAttack(), atk_.IsBacklash()
		, dodge_->GetDodgeCnt(), atk_.cnt_, atkStartTime_[static_cast<int>(SKILL_NUM::ONE)], atkStartCnt_, atkType_);


	//DrawFormatString(0, 32, 0xffffff, "atkPos(%f,%f,%f)", atk_.pos_.x, atk_.pos_.y, atk_.pos_.z);
	DrawSphere3D(colPos_, CHARACTER_SCALE * 100, 8, color_Col_, color_Col_, false);
	DrawSphere3D(atk_.pos_, atk_.radius_, 8, color_Atk_, color_Atk_, false);

	DrawFormatString(0, 400, 0xffffff, "actCntl(%d)", PlayerInput::GetInstance().GetAct());

	//プレイヤーの当たり判定
	//DrawSphere3D(colPos_, radius_, 4, 0xffff00, 0xffff00, false);
	//プレイヤーの索敵判定
	//DrawSphere3D(trans_.pos, searchRange_, 2, isMove_ ? 0xff0000 : 0xffffff, isMove_ ? 0xff0000 : 0xffffff, false);
	//プレイヤーの索敵判定
	//DrawSphere3D(userOnePos_, 20, 2, 0x0000ff, 0xffffff, false);

	//攻撃状態の時、球体の色変更
	if (atk_.IsAttack()) { color_Atk_ = ATK_COLOR; }
	else { color_Atk_ = 0x00ffff; }

}
#endif // DEBUG_ON

VECTOR PlayerBase::GetTargetVec(VECTOR _targetPos)
{
	//標的への方向ベクトルを取得						※TODO:ベクトルはSceneGameからもらう
	VECTOR targetVec = VSub(_targetPos, trans_.pos);

	//正規化
	targetVec = VNorm(targetVec);

	//Y座標は必要ないので要素を消す
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//移動量を求める
	VECTOR ret = VScale(targetVec, moveSpeed_);

	return ret;
}




void PlayerBase::Turn(float _deg, VECTOR _axis)
{
	//transform_.quaRot =
	//	transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));

	trans_.quaRot =
		trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}


void PlayerBase::Action(void)
{
	//攻撃ごとの処理
	//actUpdate_();
}

void PlayerBase::NmlAct(void)
{
	//短押しの更新
	//nmlActUpdate_();
}

void PlayerBase::NmlActKeyBoard(void)
{
	////ボタンを押したらスキル状態を返す
	//auto& ins = InputManager::GetInstance();
	//if (ins.IsTrgDown(SKILL_KEY)) { actCntl_ = ACT_CNTL::NMLSKILL; }
}

void PlayerBase::NmlActPad(void)
{
	//auto& ins = InputManager::GetInstance();
	////ボタンを押したらスキル状態を返す
	//if (ins.IsPadBtnTrgDown(padNum_, SKILL_BTN)) { actCntl_ = ACT_CNTL::NMLSKILL; }
}

void PlayerBase::ChangeNmlActControll(void)
{
	changeNmlActControll_[cntl_]();
}

void PlayerBase::ChangeNmlActKeyBoard(void)
{
	nmlActUpdate_ = std::bind(&PlayerBase::NmlActKeyBoard, this);
}

void PlayerBase::ChangeNmlActPad(void)
{
	nmlActUpdate_ = std::bind(&PlayerBase::NmlActPad, this);
}




void PlayerBase::InitAtk(void)
{
	//攻撃カウント初期化
	atk_.ResetCnt();

	//スキルが終わったらクールタイムのカウント開始
	isCool_[static_cast<int>(act_)] = true;

	//移動可能にする
	moveAble_ = true;

	//消すかも
	atk_.isHit_ = false;

	//攻撃の発生
	atkStartCnt_ = 0.0f;

	
}

void PlayerBase::Reset(void)
{
	//アニメーション初期化
	InitAnimNum();
	InitCharaAnim();
	ResetAnim(ANIM::NONE, SPEED_ANIM);
	SetParam();

	skillNo_ = ATK_ACT::SKILL1;

	//dodgeCdt_ = DODGE_CDT_MAX;
	dodge_->Init();
	moveSpeed_ = 0.0f;
	ChangeControll(SceneManager::CNTL::KEYBOARD);

	userOnePos_ = { -400.0f,0.0f,0.0f };

	act_ = ATK_ACT::MAX;
	//モデルの初期化
	trans_.Update();
}

void PlayerBase::ChangeGamePad(void)
{
	cntlUpdate_ = std::bind(&PlayerBase::GamePad, this);
}

void PlayerBase::ChangeKeyBoard(void)
{
	cntlUpdate_ = std::bind(&PlayerBase::KeyBoardControl, this);
}

void PlayerBase::SyncActPos(ATK& _atk)
{
	//追従対象の位置
	VECTOR followPos = trans_.pos;

	//追従対象の向き
	Quaternion followRot = trans_.quaRot;

	//追従対象の角度
	VECTOR relativeActPos = atkMax_[act_].pos_;

	VECTOR addPos = followRot.PosAxis(VScale(relativeActPos, CHARACTER_SCALE));

	_atk.pos_ = VAdd(trans_.pos, addPos);
}

void PlayerBase::ChangeSkillControll(ATK_ACT _skill)
{
	atkType_ = atkTypes_[static_cast<int>(_skill)];
	isPush_ = false;
	moveAble_ = true;

	//変更点
	//ChangeAtkType(static_cast<ATK_ACT>(_skill));
}






//void PlayerBase::Dodge(void)
//{
//	//ドッジフラグがtrueになったら
//	if (IsDodge() && !IsCoolDodge())
//	{
//		CntUp(dodgeCnt_);
//		//スキル中に回避が出た時にスキルのカウントをリセット
//		atk_.ResetCnt();
//		if (dodgeCnt_ < FRAME_DODGE_MAX)
//		{
//			VECTOR dir = trans_.GetForward();
//			//移動方向
//			VECTOR movePow = VScale(dir, SPEED_DODGE);
//			//移動処理
//			trans_.pos = VAdd(trans_.pos, movePow);
//		}
//		else
//		{
//			dodgeCdt_ = 0.0f;
//		}
//	}
//	else
//	{
//		CntUp(dodgeCdt_);
//		ResetDodgeFrame();
//#ifdef DEBUG_ON
//		color_Col_ = 0xffffff;
//#endif // DEBUG_ON
//	}
//}
void PlayerBase::Damage(void)
{
	//とりあえず1ダメージ減らす
	hp_--;

	//アニメーション変更
	ResetAnim(ANIM::DAMAGE, SPEED_ANIM_DODGE);
#ifdef DEBUG_ON
	const unsigned int  DMG_COLOR = 0x00ffff;
	color_Col_ = DMG_COLOR;
#endif // DEBUG_ON
	if (!IsAlive()) { ResetAnim(ANIM::DEATH, SPEED_ANIM); }

}

#ifdef DEBUG_ON
void PlayerBase::InitDebug(void)
{
	color_Col_ = 0xffffff;
	color_Atk_ = 0x00ffff;
	color_skl1_ = 0x00ffff;
	color_skl2_ = 0x00ffff;


}
#endif // DEBUG_ON




//CPU
//------------------------------------------------
void PlayerBase::RandAct(void)
{
}

void PlayerBase::CoolTimeCnt(void)
{
	if (act_ == ATK_ACT::MAX)return;
	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		if (isCool_[i])
		{
			CntUp(coolTime_[i]);
		}
		if (coolTime_[i] >= coolTimeMax_[i])
		{
			isCool_[i] = false;
			coolTime_[i] = coolTimeMax_[i];
		}
		else if (coolTime_[i] <= 0.0f)
		{
			coolTime_[i] = 0.0f;
		}
	}
}


void PlayerBase::ProcessAct(void)
{
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	//設定された向きに向かって動く
	if (ins.CheckAct(ACT_CNTL::MOVE)) { Move(moveDeg_, AsoUtility::AXIS_Y); }

	//動いてないときはスピード0にする
	else { moveSpeed_ = 0.0f; }

	if (ins.CheckAct(ACT_CNTL::CHANGE_SKILL)) { SkillChange(); }

	AtkFunc();
	
	switch (skillNo_)
	{
	case PlayerBase::ATK_ACT::SKILL1:
		Skill1Func();
		break;
	case PlayerBase::ATK_ACT::SKILL2:
		Skill2Func();
		break;
	}

	//回避
	if (ins.CheckAct(ACT_CNTL::DODGE) && IsSkillable())
	{
		float dodgeCnt = dodge_->GetDodgeCnt();
		ResetAnim(ANIM::DODGE, SPEED_ANIM_DODGE);
		CntUp(dodgeCnt);
		dodge_->SetDodgeCnt(dodgeCnt);
	}

}

void PlayerBase::SkillChange(void)
{
	skillNo_ = static_cast<ATK_ACT>(static_cast<int>(skillNo_) + 1);
	//MAXになったらスキル１に戻る
	skillNo_ == ATK_ACT::MAX ? skillNo_ = ATK_ACT::SKILL1 : skillNo_ = skillNo_;
	//変更点
	ChangeSkillControll(skillNo_);
}

void PlayerBase::NmlAtkInit(void)
{
}

void PlayerBase::SkillOneInit(void)
{
}

void PlayerBase::SkillTwoInit(void)
{
}




#ifdef INPUT_DEBUG_ON
void PlayerBase::InputUpdate(void)
{
	auto& ins = InputManager::GetInstance();
	char keyState[256] = {};
	int padState = {};
	GetHitKeyStateAll(keyState);
	padState = GetJoypadInputState(static_cast<int>(padNum_));

	//それぞれのアクション名に割り当たっているすべての入力をチェック
	for (const auto& mapInfo : inputActionMap_)
	{
		bool isPressed = false;
		for (const auto& inputInfo : mapInfo.second)
		{
			isPressed = (inputInfo.type == InputType::KEYBOARD && keyState[inputInfo.buttonId_]) ||
				(inputInfo.type == InputType::PAD && padState & inputInfo.buttonId_);
			if (isPressed)
			{
				break;
			}
		}
		//押されたら押されたアクションボタンがtrueになる？
		currentInput_[mapInfo.first] = isPressed;
	}
}

bool PlayerBase::IsPressed(const std::string& action)const
{
	auto it = currentInput_.find(action);
	if (it == currentInput_.end())//未定義のボタン名が来たら無条件でfalseを返す
	{
		return false;
	}
	else
	{
		return it->second;
	}
}
#endif // INPUT_DEBUG_ON

