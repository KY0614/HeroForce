#include"../Manager/ResourceManager.h"
#include "PlAxeMan.h"

PlAxe::PlAxe(const SceneManager::PLAY_MODE _mode, const SceneManager::CNTL _cntl) : PlayerBase(_mode, _cntl)
{
	mode_ = _mode;
	cntl_ = _cntl;
}

PlAxe::PlAxe(const SceneManager::PLAY_MODE _mode, const InputManager::JOYPAD_NO _padNum) : PlayerBase(_mode, _padNum)
{
	mode_ = _mode;
	padNum_ = _padNum;
}

void PlAxe::SetParam(void)
{
	InitAct();

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

	atkStartRange_ = ATK_START_RANGE;
}
void PlAxe::ChargeAct(void)
{
	chargeActUpdate_();

	if (isCool_[static_cast<int>(ATK_ACT::SKILL1)])return;
	//スキル(長押しでガード状態維持)
	if (CheckAct(ACT_CNTL::CHARGE_SKILL_DOWN))
	{
		ResetParam(atk_);
		CntUp(atkStartCnt_);
	}
	if (CheckAct(ACT_CNTL::CHARGE_SKILL_KEEP))
	{
		//スキルごとにアニメーションを決めて、カウント開始
		ChangeAct(static_cast<ATK_ACT>(skillNo_));

		//押している反応
		isPush_ = true;
	}
	else if (CheckAct(ACT_CNTL::CHARGE_SKILL_UP))
	{
		//InitAtk();
		isPush_ = false;
		//actCntl_ = ACT_CNTL::NONE;
	}



	
}
void PlAxe::InitAct(void)
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

void PlAxe::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::UNIQUE_2, SPIN_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void PlAxe::AtkFunc(void)
{
	
}

void PlAxe::Skill1Func(void)
{
	//力溜めて打ち込むやつ
	//moveAble_ = false;
	//クールタイムの初期化
	//coolTime_[static_cast<int>(act_)] = 0.0f;

	if (isCool_[static_cast<int>(ATK_ACT::SKILL1)])return;

	if (IsAtkStart())
	{
		CntUp(atkStartCnt_);
		if (stepAnim_ >= SKILLONE_CHARGE_STEP)
		{
			stepAnim_ = SKILLONE_CHARGE_STEP;
		}
	}
	if ((CheckAct(ACT_CNTL::CHARGE_SKILL_UP) || IsFinishAtkStart())/*&&!atk_.IsFinishMotion()*/)
	{
		CntUp(atk_.cnt_);
		coolTime_[static_cast<int>(act_)] = 0.0f;
	}
	if (atk_.IsFinishMotion())
	{
		InitAtk();
	}
	
}

void PlAxe::Skill2Func(void)
{
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