#include"../Manager/InputManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/Collision.h"
#include "./PlayableChara/PlAxeMan.h"
#include "PlayerBase.h"

void PlayerBase::Destroy(void)
{

}

void PlayerBase::SetParam(void)
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
	//関数ポインタを使ってます
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

	//攻撃の初期化(とりあえず通常攻撃で初期化しておく)
	ChangeAct(ATK_ACT::ATK);

	//コンストラクタのmodeで初期化する
	ChangeMode(mode_);

	skillNo_ = SKILL_NUM::ONE;

	dodgeCdt_ = DODGE_CDT_MAX;
	moveSpeed_ = 0.0f;
	ChangeControll(SceneManager::CNTL::KEYBOARD);

	ChangeSkillControll(SKILL_NUM::ONE);

	ChangeState(CPU_STATE::NORMAL);

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

	//モデルの更新
	trans_.Update();

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

	//それぞれの更新
	modeUpdate_();

	//アクション判定
	Action();

	//スキルが終わったらクールタイムのカウント開始
	CoolTimeCnt();


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
	if (!IsMove() && !IsDodge() && !IsAtkAction())
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
		moveSpeed_ = 0.0f;
	}
	//それぞれの操作更新
	cntlUpdate_();

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
	act_ = _act;
	//クールタイム中なら処理しない
	if (isCool_[static_cast<int>(act_)])return;
	ResetParam(_act);
	SyncActPos(atk_.pos_);
	changeAct_[_act]();


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

void PlayerBase::ResetParam(ATK_ACT _act)
{
	atk_ = atkMax_[act_];
}

void PlayerBase::KeyBoardControl(void)
{
	auto& ins = InputManager::GetInstance();
	DINPUT_JOYSTATE input;
	if (GetJoypadInputState(DX_INPUT_PAD1) && CheckHitKeyAll() < 0)
	{
		ChangeControll(SceneManager::CNTL::PAD);
	}
	//前
	if (ins.IsNew(KEY_INPUT_W)){Move(0.0f, AsoUtility::AXIS_Y);}

	//右
	else if (ins.IsNew(KEY_INPUT_D)){Move(90.0f, AsoUtility::AXIS_Y);}

	//下
	else if (ins.IsNew(KEY_INPUT_S)){Move(180.0f, AsoUtility::AXIS_Y);}

	//左
	else if (ins.IsNew(KEY_INPUT_A)){Move(270.0f, AsoUtility::AXIS_Y);}

	//動いてないときはスピード0にする
	else{moveSpeed_ = 0.0f;}





	//攻撃（攻撃アニメーションのフレームが0以下だったらフレームを設定）
	if (ins.IsTrgDown(KEY_INPUT_E) && IsAtkable()){ChangeAct(ATK_ACT::ATK);}

	//スキル切り替え
	if (ins.IsTrgDown(KEY_INPUT_J) && !IsAtkAction())
	{
		skillNo_ = static_cast<SKILL_NUM>(static_cast<int>(skillNo_) + 1);

		//MAXになったらスキル１に戻る
		skillNo_ == SKILL_NUM::MAX ? SKILL_NUM::ONE : skillNo_;
	}

	//スキル
	if (ins.IsTrgDown(KEY_INPUT_Q) && IsSkillable()){ChangeAct(static_cast<ATK_ACT>(skillNo_));}

	//回避
	if (ins.IsTrgDown(KEY_INPUT_N) && IsSkillable())
	{
		ResetAnim(ANIM::DODGE, SPEED_ANIM_DODGE);
		CntUp(dodgeCnt_);
	}
}

void PlayerBase::GamePad(void)
{
	auto& ins = InputManager::GetInstance();
	if (GetJoypadNum() == 0)
	{
		ChangeControll(SceneManager::CNTL::KEYBOARD);
	}

	// 左スティックの横軸
	leftStickX_ = ins.GetJPadInputState(padNum_).AKeyLX;

	//縦軸
	leftStickY_ = ins.GetJPadInputState(padNum_).AKeyLY;

	//方向決め
	auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));
	stickDeg_ = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(stickRad) + 90.0f));
	//前
	if (leftStickY_ < -1) { Move(stickDeg_, AsoUtility::AXIS_Y); }

	//右
	else if (leftStickX_ > 1) { Move(stickDeg_, AsoUtility::AXIS_Y); }

	//下
	else if (leftStickY_ > 1){Move(stickDeg_, AsoUtility::AXIS_Y);}

	//左
	else if (leftStickX_ < -1){Move(stickDeg_, AsoUtility::AXIS_Y);}

	else{moveSpeed_ = 0.0f;}

	//攻撃（攻撃アニメーションのフレームが0以下だったらフレームを設定）
	if (ins.IsPadBtnTrgDown(padNum_, InputManager::JOYPAD_BTN::RIGHT)
		&& IsAtkable())
	{
		if (isCool_[static_cast<int>(ATK_ACT::ATK)])return;
		ChangeAct(ATK_ACT::ATK);
		CntUp(atkStartCnt_);
	}


	//スキル(短押しと長押しで分けている)
	atkTypeUpdate_();

	if (ins.IsPadBtnTrgDown(padNum_, InputManager::JOYPAD_BTN::R_BUTTON))
	{
		if (!IsAtkAction()) 
		{ 

			skillNo_ = static_cast<SKILL_NUM>(static_cast<int>(skillNo_) + 1);
			ChangeSkillControll(skillNo_);
			if (skillNo_ == SKILL_NUM::MAX)
			{
				skillNo_ = SKILL_NUM::ONE;
			}
		}
		
	}

	//回避
	if (ins.IsPadBtnTrgDown(padNum_, InputManager::JOYPAD_BTN::LEFT)
		&& IsDodgeable())
	{
		ResetAnim(ANIM::DODGE, SPEED_ANIM_DODGE);
		CntUp(dodgeCnt_);
	}
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
	//DrawFormatString(0, 32, 0xffffff
	//	, "FrameATK(%f)\nisAtk(%d)\nisBackSrash(%d)\nDodge(%f)\nSkill(%f)"
	//	, atk_.cnt_, atk_.IsAttack(), atk_.IsBacklash()
	//	, dodgeCnt_, atk_.cnt_);
	DrawFormatString(0, 32, 0xffffff
		, "AtkCooltime(%.2f)\nSkill1Cool(%.2f)\nSkill2Cool(%.2f)\natkDulation(%f)"
		,coolTime_[static_cast<int>(ATK_ACT::ATK)]
		, coolTime_[static_cast<int>(ATK_ACT::SKILL1)]
		, coolTime_[static_cast<int>(ATK_ACT::SKILL2)]
		, atk_.duration_);

	DrawSphere3D(colPos_, CHARACTER_SCALE * 100, 8, color_Col_, color_Col_, false);
	DrawSphere3D(atk_.pos_, atk_.radius_, 8, color_Atk_, color_Atk_, false);

	//プレイヤーの当たり判定
	//DrawSphere3D(colPos_, radius_, 4, 0xffff00, 0xffff00, false);
	//プレイヤーの索敵判定
	//DrawSphere3D(trans_.pos, searchRange_, 2, isMove_ ? 0xff0000 : 0xffffff, isMove_ ? 0xff0000 : 0xffffff, false);
	//プレイヤーの索敵判定
	DrawSphere3D(userOnePos_, 20, 2, 0x0000ff, 0xffffff, false);

	//攻撃状態の時、球体の色変更
	if (atk_.IsAttack()){color_Atk_ = ATK_COLOR;}
	else{color_Atk_ = 0x00ffff;}

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
	//ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);

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
	//共通
	NmlActCommon();

	auto& ins = InputManager::GetInstance();
	//スキル
	if (ins.IsPadBtnTrgDown(padNum_, InputManager::JOYPAD_BTN::TOP)
		&& IsSkillable() && !isCool_[static_cast<int>(skillNo_)])
	{
		//スキルごとにアニメーションを決めて、カウント開始
		ChangeAct(static_cast<ATK_ACT>(skillNo_));
		CntUp(atkStartCnt_);
	}
}

void PlayerBase::NmlActCommon(void)
{
	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
		if (IsFinishAtkStart()) { CntUp(atk_.cnt_); }
	}
	else
	{
		if ((atk_.IsAttack() || atk_.IsBacklash()))
		{
			CntUp(atk_.cnt_);
			//クールタイムの初期化
			coolTime_[static_cast<int>(act_)] = 0.0f;
		}
		else
		{
			InitAtk();
		}
	}
}

void PlayerBase::ChargeAct(void)
{
	//スキルボタンは同じだからスキルボタンを押しているときと離した時の処理を各役割ごとで作る
	auto& ins = InputManager::GetInstance();
	if (ins.IsPadBtnTrgDown(padNum_, InputManager::JOYPAD_BTN::TOP))
	{
		//ボタンの押しはじめの時に値初期化
		ResetGuardCnt();
	}

	//スキル(長押しでガード状態維持)
	if (ins.IsPadBtnNew(padNum_, InputManager::JOYPAD_BTN::TOP))
	{
		//スキルごとにアニメーションを決めて、カウント開始
		ChangeAct(static_cast<ATK_ACT>(skillNo_));

		//押している反応
		isPush_ = true;
	}
	else if (ins.IsPadBtnTrgUp(padNum_, InputManager::JOYPAD_BTN::TOP))
	{
		InitAtk();
		isPush_ = false;
	}
}

void PlayerBase::ChangeChargeAct(void)
{
	atkTypeUpdate_ = std::bind(&PlayerBase::ChargeAct, this);
}

void PlayerBase::ChangeNmlAct(void)
{
	atkTypeUpdate_ = std::bind(&PlayerBase::NmlAct, this);
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
	skillNo_ = SKILL_NUM::ONE;

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

void PlayerBase::SyncActPos(VECTOR& _localPos)
{
	//追従対象の位置
	VECTOR followPos = trans_.pos;

	//追従対象の向き
	Quaternion followRot = trans_.quaRot;

	//追従対象の角度
	VECTOR relativeActPos = atkMax_[act_].pos_;

	VECTOR addPos = followRot.PosAxis(VScale(relativeActPos, CHARACTER_SCALE));
	//VECTOR relativeActPos = followRot.PosAxis(_localPos);
	atk_.pos_ = VAdd(trans_.pos, addPos);
}

void PlayerBase::ChangeSkillControll(SKILL_NUM _skill)
{
	atkType_ = atkTypes_[static_cast<int>(_skill)];
	isPush_ = false;
	moveAble_ = true;
	changeAtkType_[atkType_]();
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

void PlayerBase::SkillOneControll(void)
{

}

void PlayerBase::SkillTwoControll(void)
{

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