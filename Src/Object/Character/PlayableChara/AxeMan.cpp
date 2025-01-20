#include "AxeMan.h"

AxeMan::AxeMan(const SceneManager::CNTL _cntl) : PlayerBase(_cntl)
{
	cntl_ = _cntl;
}

AxeMan::AxeMan(const InputManager::JOYPAD_NO _padNum) : PlayerBase(_padNum)
{
	padNum_ = _padNum;
}

void AxeMan::SetParam(void)
{
	InitAct();

	//攻撃の初期化
	InitAtk();

	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));
	float scale = CHARACTER_SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { -300.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);

	hp_ = MAX_HP;

	moveAble_ = true;

	//当たり判定の設定
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

}
void AxeMan::InitAct(void)
{
	//通常攻撃の最大値
	atkMax_.emplace(ATK_ACT::ATK, ATK_MAX);

	//スキル１の最大値
	atkMax_.emplace(ATK_ACT::SKILL1, SKILL_ONE_MAX);

	//スキル２の最大値
	atkMax_.emplace(ATK_ACT::SKILL2, SKILL_TWO_MAX);


	//クールタイム
	coolTimeMax_[static_cast<int>(ATK_ACT::ATK)] = ATK_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_COOLTIME;

	//攻撃発生時間
	atkStartTime_[static_cast<int>(ATK_ACT::ATK)] = ATK_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_START;

	//攻撃タイプ
	atkTypes_[static_cast<int>(ATK_ACT::ATK)] = ATK_TYPE::NORMALATK;
	atkTypes_[static_cast<int>(ATK_ACT::SKILL1)] = ATK_TYPE::CHARGEATK;
	atkTypes_[static_cast<int>(ATK_ACT::SKILL2)] = ATK_TYPE::NORMALATK;
}

void AxeMan::InitSkill(ATK_ACT _act)
{
	if (isAtk_ || isSkill_)return;
	//スキルごとにアニメーションを決めて、カウント開始
	ChangeAct(static_cast<ATK_ACT>(_act));
	ResetParam(atk_);
	CntUp(atkStartCnt_);
	moveAble_ = false;
	isSkill_ = true;
}

void AxeMan::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::UNIQUE_2, SPIN_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void AxeMan::ChargeAct(void)
{

}

void AxeMan::SkillOneInit(void)
{
	//if (!IsAtkAction() && !isCool_[static_cast<int>(skillNo_)])
	//{
	//	//スキルごとにアニメーションを決めて、カウント開始
	//	ChangeAct(static_cast<ATK_ACT>(skillNo_));
	//	ResetParam(atk_);
	//	CntUp(atkStartCnt_);
	//	moveAble_ = false;
	//}

}

void AxeMan::SkillTwoInit(void)
{
	if (!IsAtkAction() && !isCool_[static_cast<int>(skillNo_)])
	{
		////スキルごとにアニメーションを決めて、カウント開始
		//ChangeAct(static_cast<ATK_ACT>(skillNo_));
		//ResetParam(atk_);
		//CntUp(atkStartCnt_);
		//moveAble_ = false;
	}
}

void AxeMan::AtkFunc(void)
{
	if (isSkill_)return;
	auto& ins = InputManager::GetInstance();
	//近接攻撃用
	if (ins.IsTrgDown(ATK_KEY) && !isAtk_)
	{
		if (isCool_[static_cast<int>(ATK_ACT::ATK)])return;
		ChangeAct(ATK_ACT::ATK);
		ResetParam(atk_);
		CntUp(atkStartCnt_);
		isAtk_ = true;
	}
	if (!isAtk_)return;

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
			isAtk_ = false;
		}
	}
}

void AxeMan::Skill1Func(void)
{
	//入力
	//--------------------------------------------------------------
	auto& ins = InputManager::GetInstance();
	int skillOne = static_cast<int>(ATK_ACT::SKILL1);
	if (!isCool_[static_cast<int>(ATK_ACT::SKILL1)])
	{
		if (ins.IsTrgDown(SKILL_KEY) && !IsAtkStart())
		{
			InitSkill(skillNo_);
		}
		//スキル(長押しでガード状態維持)
		if (ins.IsNew(SKILL_KEY) && IsAtkStart())
		{
			//押している反応
			//CntUp(atkStartCnt_);
		}
		else if (ins.IsTrgUp(SKILL_KEY) && IsAtkStart())
		{
			if (atkStartCnt_ <= SKILL_ONE_START_NOCHARGE)
			{
				atkStartTime_[skillOne] = SKILL_ONE_START_NOCHARGE;
			}
			else
			{
				atkStartTime_[skillOne] = atkStartCnt_;
			}
		}
	}



	if (isCool_[static_cast<int>(skillNo_)])
	{
		return;
	}
	if (0.0f < atkStartCnt_ && atkStartCnt_ < atkStartTime_[static_cast<int>(act_)])
	{
		CntUp(atkStartCnt_);
		if (stepAnim_ >= SKILL_CHARGE_STEPANIM)
		{
			stepAnim_ = SKILL_CHARGE_STEPANIM;
		}
	}
	else if (atkStartCnt_ >= atkStartTime_[static_cast<int>(skillNo_)])
	{
		CntUp(atk_.cnt_);
		if (atk_.IsFinishMotion())
		{
			coolTime_[static_cast<int>(ATK_ACT::SKILL1)] = 0.0f;

			//スキル終わったら攻撃発生時間の最大時間をセットする
			atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;

			InitAtk();
			isSkill_ = false;
		}
	}
}

void AxeMan::Skill2Func(void)
{
	auto& ins = InputManager::GetInstance();
	//入力
	if (ins.IsTrgDown(SKILL_KEY))
	{
		InitSkill(skillNo_);
	}
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
			isSkill_ = false;
		}
	}


	if (atk_.cnt_ >= SKILL2_CHANGE_ANIM_TIME)
	{
		if (stepAnim_ > 14.0f)ResetAnim(ANIM::UNIQUE_2, SPEED_ANIM_ATK);
		//回転中移動できる
		moveAble_ = true;
		//攻撃座標を移動中も同期する
		SyncActPos(atk_);
		//持続回転切り
		if (atk_.isHit_)
		{
			CntUp(multiHitInterval_);
			//クールタイムの初期化
			coolTime_[static_cast<int>(act_)] = 0.0f;
			if (multiHitInterval_ >= MULTIHIT_INTERVAL)
			{
				atk_.isHit_ = false;
				multiHitInterval_ = 0.0f;
			}
		}
		else
		{
			multiHitInterval_ = 0.0f;
		}
	}
}