#include"../Manager/InputManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/Collision.h"
#include "./PlayableChara/PlAxeMan.h"
#include "PlayerBase.h"

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
#ifdef DEBUG_ON
	InitDebug();
#endif // DEBUG_ON
	//関数ポインタ初期化
	cpuStateChanges_.emplace(CPU_STATE::NORMAL, std::bind(&PlayerBase::CpuChangeNml, this));
	cpuStateChanges_.emplace(CPU_STATE::ATTACK, std::bind(&PlayerBase::CpuChangeAtk, this));
	cpuStateChanges_.emplace(CPU_STATE::BREAK, std::bind(&PlayerBase::CpuChangeBreak, this));

	changeAtkType_.emplace(ATK_TYPE::CHARGEATK, std::bind(&PlayerBase::ChangeChargeAct, this));
	changeAtkType_.emplace(ATK_TYPE::NORMALATK, std::bind(&PlayerBase::ChangeNmlAct, this));

	changeCntl_.emplace(SceneManager::CNTL::KEYBOARD, std::bind(&PlayerBase::ChangeKeyBoard, this));
	changeCntl_.emplace(SceneManager::CNTL::PAD, std::bind(&PlayerBase::ChangeGamePad, this));

	changeAct_.emplace(ATK_ACT::ATK, std::bind(&PlayerBase::ChangeNmlAtk, this));
	changeAct_.emplace(ATK_ACT::SKILL1, std::bind(&PlayerBase::ChangeSkillOne, this));
	changeAct_.emplace(ATK_ACT::SKILL2, std::bind(&PlayerBase::ChangeSkillTwo, this));

	changeMode_.emplace(SceneManager::PLAY_MODE::USER, std::bind(&PlayerBase::ChangeUser, this));
	changeMode_.emplace(SceneManager::PLAY_MODE::CPU, std::bind(&PlayerBase::ChangeCpu, this));

	changeNmlActControll_.emplace(SceneManager::CNTL::KEYBOARD, std::bind(&PlayerBase::ChangeNmlActKeyBoard, this));
	changeNmlActControll_.emplace(SceneManager::CNTL::PAD, std::bind(&PlayerBase::ChangeNmlActPad, this));

	changeChargeActCntl_.emplace(SceneManager::CNTL::KEYBOARD, std::bind(&PlayerBase::ChangeChargeActKeyBoard, this));
	changeChargeActCntl_.emplace(SceneManager::CNTL::PAD, std::bind(&PlayerBase::ChangeChargeActPad, this));

	//キーボードとパッドのボタンを共通させる(これらはInputManagerで定義したものから使えるようにしたい)
	//------------------------------------------------------
	inputActionMap_["ATTACK"] = { {InputType::KEYBOARD,ATK_KEY},{InputType::PAD,RIGHT_BTN} };
	inputActionMap_["DODGE"] = { {InputType::KEYBOARD,DODGE_KEY},{InputType::PAD,LEFT_BTN} };
	inputActionMap_["SKILL"] = { {InputType::KEYBOARD,SKILL_KEY},{InputType::PAD,TOP_BTN} };
	inputActionMap_["SKILL_CHANGE"] = { {InputType::KEYBOARD,SKILL_CHANGE_KEY},{InputType::PAD,R_BUTTON} };




	//攻撃の初期化(とりあえず通常攻撃で初期化しておく)
	ChangeAct(ATK_ACT::ATK);

	//コンストラクタのmodeで初期化する
	ChangeMode(mode_);


	ChangeControll(SceneManager::CNTL::KEYBOARD);

	ChangeSkillControll(ATK_ACT::SKILL1);

	ChangeState(CPU_STATE::NORMAL);

	ChangeChargeActControll();
	ChangeNmlActControll();

	skillNo_ = ATK_ACT::SKILL1;

	dodgeCdt_ = DODGE_CDT_MAX;

	moveSpeed_ = 0.0f;

	searchRange_ = SEARCH_RANGE;

	preAtk_ = ATK_ACT::MAX;

	isEnemySerch_ = false;

	isMove2CallPlayer_ = false;

	userOnePos_ = { -400.0f,0.0f,0.0f };

	//act_ = ATK_ACT::MAX;


	calledMoveSpeed_ = 1.0f;

	atk_.isHit_ = false;

	isCall_ = false;

	multiHitInterval_ = 0.0f;

	atkStartCnt_ = 0.0f;

	isPush_ = false;



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


	//モデルの更新
	trans_.Update();

	SyncActPos(atk_);

	colPos_ = VAdd(trans_.pos, VScale(PLAYER_COL_LOCAL_POS, CHARACTER_SCALE));


#ifdef DEBUG_ON
	//この処理は後でキーボードのところに置いておく
	{
		auto& ins = InputManager::GetInstance();
		//CPUを強制的に呼び出す
		if (ins.IsTrgDown(KEY_INPUT_X))
		{
			isCall_ = true;
		}
		if (!isMove2CallPlayer_
			&& isCall_
			&& !AsoUtility::IsHitSpheres(trans_.pos, SEARCH_RANGE, userOnePos_, MY_COL_RADIUS)
			&& cpuState_ == CPU_STATE::BREAK)
		{
			isMove2CallPlayer_ = true;
			ChangeState(CPU_STATE::NORMAL);

			//前の攻撃の初期化
			preAtk_ = ATK_ACT::MAX;

			//呼び出されて移動するときの加速度
			calledMoveSpeed_ = CALLED_MOVE_SPEED_SCALE;

			//プレイヤーに優先的についていきたいから敵を察知しないようにする
			isEnemySerch_ = false;

			//ターゲットを呼び出されたプレイヤーに設定する
			SetTargetPos(userOnePos_);
		}
	}

#endif // DEBUG_ON
	//スキルが終わったらクールタイムのカウント開始
	CoolTimeCnt();

	//それぞれの更新(プレイヤーとCPU)
	modeUpdate_();

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
void PlayerBase::CpuUpdate(void)
{
	cpuStateUpdate_();
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
	if (!IsAtkAction())
	{
		ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);
	}
	if (!IsDodge() && moveAble_)
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
	//停止状態の時のアニメーション
	//if (!IsMove() && !IsDodge())
	//{
	//	if (!IsAtkAction())
	//	{
	//		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
	//		moveSpeed_ = 0.0f;
	//	}
	//}


	//それぞれの操作更新
	cntlUpdate_();



	//停止アニメーションになる条件
	if (!IsMove() && !IsDodge() && 0.0f >= atkStartCnt_ && !atk_.IsAttack() && !atk_.IsBacklash())
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
		moveSpeed_ = 0.0f;
	}


	ProcessAct();

	//回避
	Dodge();

}

void PlayerBase::ChangeMode(SceneManager::PLAY_MODE _mode)
{
	mode_ = _mode;
	changeMode_[mode_]();
}

void PlayerBase::ChangeUser(void)
{
	modeUpdate_ = std::bind(&PlayerBase::UserUpdate, this);
}

void PlayerBase::ChangeCpu(void)
{
	modeUpdate_ = std::bind(&PlayerBase::CpuUpdate, this);
}

//役割ごとのCPU処理を作るので残しておく
void PlayerBase::CpuActUpdate(ATK_ACT _act)
{

}

void PlayerBase::CpuChangeNml(void)
{
	cpuStateUpdate_ = std::bind(&PlayerBase::CpuNmlUpdate, this);
}

void PlayerBase::CpuChangeAtk(void)
{
	isCall_ = false;
	cpuStateUpdate_ = std::bind(&PlayerBase::CpuAtkUpdate, this);
}

void PlayerBase::CpuChangeBreak(void)
{
	breakCnt_ = 0.0f;
	cpuStateUpdate_ = std::bind(&PlayerBase::CpuBreakUpdate, this);
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
	ChangeAtkType(_act);


	//atkStartCnt_をここで開始させる
	//CntUp(atkStartCnt_);
}

void PlayerBase::ChangeNmlAtk(void)
{
	ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);
	actUpdate_ = std::bind(&PlayerBase::AtkFunc, this);
}

void PlayerBase::ChangeSkillOne(void)
{
	ResetAnim(ANIM::SKILL_1, SPEED_ANIM_ATK);
	isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
	actUpdate_ = std::bind(&PlayerBase::Skill1Func, this);
}

void PlayerBase::ChangeSkillTwo(void)
{
	ResetAnim(ANIM::SKILL_2, SPEED_ANIM_ATK);
	actUpdate_ = std::bind(&PlayerBase::Skill2Func, this);
}

void PlayerBase::ResetParam(ATK& _atk)
{
	_atk = atkMax_[act_];
}

void PlayerBase::KeyBoardControl(void)
{
	auto& ins = InputManager::GetInstance();



	if (ins.IsNew(MOVE_FRONT_KEY)) { moveDeg_ = 0.0f; }
	else if (ins.IsNew(MOVE_LEFT_KEY)) { moveDeg_ = 270.0f; }
	else if (ins.IsNew(MOVE_BACK_KEY)) { moveDeg_ = 180.0f; }
	else if (ins.IsNew(MOVE_RIGHT_KEY)) { moveDeg_ = 90.0f; }
	else { actCntl_ = ACT_CNTL::NONE; }



	if (ins.IsTrgDown(DODGE_KEY)) { actCntl_ = ACT_CNTL::DODGE; }
	if (ins.IsTrgDown(ATK_KEY)
		&& IsAtkable() && !isCool_[static_cast<int>(ATK_ACT::ATK)])
	{
		actCntl_ = ACT_CNTL::NMLATK;
	}

	if (ins.IsNew(MOVE_FRONT_KEY) || ins.IsNew(MOVE_LEFT_KEY)
		|| ins.IsNew(MOVE_BACK_KEY) || ins.IsNew(MOVE_RIGHT_KEY))
	{
		actCntl_ = ACT_CNTL::MOVE;
	}

	//if (CheckHitKeyAll() == 0)
	//{
	//	actCntl_ = ACT_CNTL::NONE;
	//}

	if (ins.IsTrgDown(SKILL_CHANGE_KEY)) { SkillChange(); }



	//長押ししているときに移動できるからどうにかする
	//auto& ins = InputManager::GetInstance();
	DINPUT_JOYSTATE input;
	if (GetJoypadInputState(DX_INPUT_PAD1) && CheckHitKeyAll() < 0)
	{
		ChangeControll(SceneManager::CNTL::PAD);
		ChangeNmlActControll();
		ChangeChargeActControll();
		return;
	}

}

void PlayerBase::GamePad(void)
{
	auto& ins = InputManager::GetInstance();
	// 左スティックの横軸
	leftStickX_ = ins.GetJPadInputState(padNum_).AKeyLX;

	//縦軸
	leftStickY_ = ins.GetJPadInputState(padNum_).AKeyLY;

	//方向決め
	auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));
	stickDeg_ = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(stickRad) + 90.0f));

	//前
	if (leftStickY_ != 0 || leftStickX_ != 0)
	{
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = stickDeg_;
	}

	else { actCntl_ = ACT_CNTL::NONE; }

	//攻撃（攻撃アニメーションのフレームが0以下だったらフレームを設定）
	if (ins.IsPadBtnTrgDown(padNum_, ATK_BTN))
	{
		if (IsAtkable() && !isCool_[static_cast<int>(ATK_ACT::ATK)])
		{
			actCntl_ = ACT_CNTL::NMLATK;
		}
	}

	if (ins.IsPadBtnTrgDown(padNum_, SKILL_CHANGE_BTN) && !IsAtkAction()) { SkillChange(); }
	//回避
	if (ins.IsPadBtnTrgDown(padNum_, DODGE_BTN) && IsDodgeable()) { actCntl_ = ACT_CNTL::DODGE; }

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
		, "FrameATK(%f)\nisAtk(%d)\nisBackSrash(%d)\nDodge(%f)\nSkill(%f)\nactCntl(%d)\natkStartTime(%f)\natkStartCnt(%f)\nskillType(%d)"
		, atk_.cnt_, atk_.IsAttack(), atk_.IsBacklash()
		, dodgeCnt_, atk_.cnt_, actCntl_, atkStartTime_[static_cast<int>(SKILL_NUM::ONE)], atkStartCnt_, atkType_);
	//DrawFormatString(0, 32, 0xffffff
	//	, "AtkCooltime(%.2f)\nSkill1Cool(%.2f)\nSkill2Cool(%.2f)\natkDulation(%f)\natkCnt(%f)"
	//	, coolTime_[static_cast<int>(ATK_ACT::ATK)]
	//	, coolTime_[static_cast<int>(ATK_ACT::SKILL1)]
	//	, coolTime_[static_cast<int>(ATK_ACT::SKILL2)]
	//	, atk_.duration_
	//,atkStartCnt_);

	//DrawFormatString(0, 32, 0xffffff, "atkPos(%f,%f,%f)", atk_.pos_.x, atk_.pos_.y, atk_.pos_.z);
	DrawSphere3D(colPos_, CHARACTER_SCALE * 100, 8, color_Col_, color_Col_, false);
	DrawSphere3D(atk_.pos_, atk_.radius_, 8, color_Atk_, color_Atk_, false);

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



void PlayerBase::CpuMove(VECTOR _targetPos)
{
	//移動速度の更新
	moveSpeed_ = SPEED_MOVE * calledMoveSpeed_;
	isMove_ = true;

	//方向ベクトル取得
	VECTOR targetVec = GetTargetVec(_targetPos);

	//回転
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);

	//移動
	trans_.pos = VAdd(trans_.pos, targetVec);
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
	actUpdate_();
}

void PlayerBase::NmlAct(void)
{
	//短押しの更新
	nmlActUpdate_();
}

void PlayerBase::NmlActKeyBoard(void)
{
	//ボタンを押したらスキル状態を返す
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(SKILL_KEY)) { actCntl_ = ACT_CNTL::NMLSKILL; }
}

void PlayerBase::NmlActPad(void)
{
	auto& ins = InputManager::GetInstance();
	//ボタンを押したらスキル状態を返す
	if (ins.IsPadBtnTrgDown(padNum_, SKILL_BTN)) { actCntl_ = ACT_CNTL::NMLSKILL; }
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

void PlayerBase::NmlActCommon(void)
{
	if (CheckAct(ACT_CNTL::NMLSKILL)
		&& IsSkillable() && !isCool_[static_cast<int>(skillNo_)])
	{
		//スキルごとにアニメーションを決めて、カウント開始
		ChangeAct(static_cast<ATK_ACT>(skillNo_));
		ResetParam(atk_);
		CntUp(atkStartCnt_);
	}
}

void PlayerBase::NmlAtkUpdate(void)
{
	//近接攻撃用(atk_変数と遠距離で分ける)
	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
		if (IsFinishAtkStart())
		{
			CntUp(atk_.cnt_);
		}
	}
	else if (IsFinishAtkStart())
	{
		if ((atk_.IsAttack() || atk_.IsBacklash()))
		{
			CntUp(atk_.cnt_);
			//クールタイムの初期化
			coolTime_[static_cast<int>(act_)] = 0.0f;
		}
		else //if(atk_.IsFinishMotion())/*これつけると通常連打の時にバグる*/
		{
			InitAtk();
		}
	}
}

void PlayerBase::ChargeAct(void)
{
	chargeActUpdate_();
	if (CheckAct(ACT_CNTL::CHARGE_SKILL_DOWN))
	{
		//ボタンの押しはじめの時に値初期化
		ResetGuardCnt();
	}


	//スキル(長押しでガード状態維持)
	if (CheckAct(ACT_CNTL::CHARGE_SKILL_KEEP))
	{
		//スキルごとにアニメーションを決めて、カウント開始
		ChangeAct(static_cast<ATK_ACT>(skillNo_));

		//押している反応
		isPush_ = true;
	}
	else if (CheckAct(ACT_CNTL::CHARGE_SKILL_UP))
	{
		InitAtk();
		isPush_ = false;
		actCntl_ = ACT_CNTL::NONE;
	}
}

void PlayerBase::ChargeActKeyBoard(void)
{
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(SKILL_KEY))
	{
		actCntl_ = ACT_CNTL::CHARGE_SKILL_DOWN;
	}
	else if (ins.IsNew(SKILL_KEY) && !isCool_[static_cast<int>(skillNo_)])
	{
		actCntl_ = ACT_CNTL::CHARGE_SKILL_KEEP;
	}
	else if (ins.IsTrgUp(SKILL_KEY)) { actCntl_ = ACT_CNTL::CHARGE_SKILL_UP; }

}

void PlayerBase::ChargeActPad(void)
{
	auto& ins = InputManager::GetInstance();
	if (ins.IsPadBtnTrgDown(padNum_, SKILL_BTN)) { actCntl_ = ACT_CNTL::CHARGE_SKILL_DOWN; }

	else if (ins.IsPadBtnNew(padNum_, SKILL_BTN) && !isCool_[static_cast<int>(skillNo_)])
	{
		actCntl_ = ACT_CNTL::CHARGE_SKILL_KEEP;
	}
	else if (ins.IsPadBtnTrgUp(padNum_, SKILL_BTN)) { actCntl_ = ACT_CNTL::CHARGE_SKILL_UP; }

}

void PlayerBase::ChangeChargeActKeyBoard(void)
{
	chargeActUpdate_ = std::bind(&PlayerBase::ChargeActKeyBoard, this);
}

void PlayerBase::ChangeChargeActPad(void)
{
	chargeActUpdate_ = std::bind(&PlayerBase::ChargeActPad, this);
}

void PlayerBase::ChangeChargeActControll(void)
{
	changeChargeActCntl_[cntl_]();
}

void PlayerBase::ChangeAtkType(ATK_ACT _act)
{
	atkType_ = atkTypes_[static_cast<int>(_act)];
	changeAtkType_[atkType_]();
}

void PlayerBase::ChangeChargeAct(void)
{
	atkTypeUpdate_ = std::bind(&PlayerBase::ChargeAct, this);
}

void PlayerBase::ChangeNmlAct(void)
{
	atkTypeUpdate_ = std::bind(&PlayerBase::NmlActCommon, this);
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

void PlayerBase::ChangeState(CPU_STATE _state)
{
	cpuState_ = _state;
	cpuStateChanges_[_state]();
}

void PlayerBase::Reset(void)
{
	//アニメーション初期化
	InitAnimNum();
	InitCharaAnim();
	ResetAnim(ANIM::NONE, SPEED_ANIM);
	SetParam();

	skillNo_ = ATK_ACT::SKILL1;

	dodgeCdt_ = DODGE_CDT_MAX;
	moveSpeed_ = 0.0f;
	ChangeControll(SceneManager::CNTL::KEYBOARD);

	searchRange_ = SEARCH_RANGE;

	preAtk_ = ATK_ACT::MAX;

	isEnemySerch_ = false;

	isMove2CallPlayer_ = false;

	userOnePos_ = { -400.0f,0.0f,0.0f };

	act_ = ATK_ACT::MAX;

	calledMoveSpeed_ = 1.0f;

	atk_.isHit_ = false;

	isCall_ = false;

	multiHitInterval_ = 0.0f;

	atkStartCnt_ = 0.0f;

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
	ChangeAtkType(static_cast<ATK_ACT>(_skill));
}






void PlayerBase::Dodge(void)
{
	//ドッジフラグがtrueになったら
	if (IsDodge() && !IsCoolDodge())
	{
		CntUp(dodgeCnt_);
		//スキル中に回避が出た時にスキルのカウントをリセット
		atk_.ResetCnt();
		if (dodgeCnt_ < FRAME_DODGE_MAX)
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
		CntUp(dodgeCdt_);
		ResetDodgeFrame();
#ifdef DEBUG_ON
		color_Col_ = 0xffffff;
#endif // DEBUG_ON
	}
}
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
	//設定された向きに向かって動く
	if (CheckAct(ACT_CNTL::MOVE)) { Move(moveDeg_, AsoUtility::AXIS_Y); }

	//動いてないときはスピード0にする
	else { moveSpeed_ = 0.0f; }

	//攻撃（攻撃アニメーションのフレームが0以下だったらフレームを設定）
	if (CheckAct(ACT_CNTL::NMLATK))
	{
		NmlAtkInit();
	}

	if (InputManager::GetInstance().IsTrgDown(SKILL_KEY))
	{
		switch (skillNo_)
		{
		case PlayerBase::ATK_ACT::SKILL1:
			SkillOneInit();
			break;
		case PlayerBase::ATK_ACT::SKILL2:
			SkillTwoInit();
			break;
		}
	}

	atkTypeUpdate_();

	//回避
	if (CheckAct(ACT_CNTL::DODGE) && IsSkillable())
	{
		ResetAnim(ANIM::DODGE, SPEED_ANIM_DODGE);
		CntUp(dodgeCnt_);
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
	if (isCool_[static_cast<int>(ATK_ACT::ATK)])return;
	ChangeAct(ATK_ACT::ATK);
	ResetParam(atk_);
	CntUp(atkStartCnt_);
}

void PlayerBase::SkillOneInit(void)
{
}

void PlayerBase::SkillTwoInit(void)
{
}



void PlayerBase::CpuNmlUpdate(void)
{
	auto& col = Collision::GetInstance();
	const float MOVE_DELAY_MAX = 0.3f;

	//待機アニメーション
	//停止状態の時のアニメーション
	if (!isMove_ && !IsDodge() && !IsAtkAction())
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
		moveSpeed_ = 0.0f;
	}
	//歩きアニメーション
	else
	{
		ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);
	}

	//移動量の初期化
	moveSpeed_ = 0.0f;

	//preAtk_ = ATK_ACT::MAX;

	if (!isEnemySerch_)
	{
		if (AsoUtility::IsHitSpheres(trans_.pos, FOLLOW_PLAYER_RADIUS, userOnePos_, FOLLOW_PLAYER_RADIUS))
		{
			//呼び出しが終わったらそれぞれの初期化
			isMove_ = false;
			isMove2CallPlayer_ = false;
			calledMoveSpeed_ = 1.0f;
			isCall_ = false;
			moveStartDelay_ = 0.0f;
		}
		else
		{
			//プレイヤーのところまで動く
			targetPos_ = userOnePos_;
			CntUp(moveStartDelay_);
			if (moveStartDelay_ >= MOVE_DELAY_MAX)
			{
				moveStartDelay_ = MOVE_DELAY_MAX;
				CpuMove(targetPos_);
			}
		}
	}
	else
	{
		//敵を察知したら、ゲームシーン側でSetTargetPosの中に敵の座標を入れて
		//それに向けて動かす
		CpuMove(targetPos_);
	}


}

void PlayerBase::CpuAtkUpdate(void)
{
	//攻撃が終わっているなら状態遷移
	if (atk_.IsFinishMotion())
	{
		//前の攻撃の初期化
		preAtk_ = ATK_ACT::MAX;

		//休憩状態に遷移
		ChangeState(CPU_STATE::BREAK);
		return;
	}
	//前の攻撃が何もなかったら、攻撃種類を決める
	if (preAtk_ == ATK_ACT::MAX)
	{
		for (int i = ATK_TOTAL - 1; i > -1; i--)
		{
			if (!isCool_[i])
			{
				ChangeAct(static_cast<ATK_ACT>(i));
				preAtk_ = static_cast<ATK_ACT>(i);
				return;
			}
			else
			{
				continue;
			}
		}
	}
}

void PlayerBase::CpuBreakUpdate(void)
{
	const float BREAK_TIME = 2.0f;
	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
	if (breakCnt_ > BREAK_TIME)
	{
		//ブレイクが終わったら通常状態へ移行
		ChangeState(CPU_STATE::NORMAL);
		return;
	}
	CntUp(breakCnt_);
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

