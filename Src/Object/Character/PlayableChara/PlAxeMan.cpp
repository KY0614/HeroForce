#include"../Manager/ResourceManager.h"
#include "PlAxeMan.h"

PlAxe::PlAxe(SceneManager::PLAY_MODE _mode, InputManager::JOYPAD_NO _padNum) : PlayerBase(_mode, _padNum)
{
	mode_ = _mode;
	padNum_ = _padNum;
}

void PlAxe::SetParam(void)
{
	InitAct();

	//InitAct(ATK_ACT::SKILL1, FRAME_SKILL1_DURATION, FRAME_SKILL1_BACKRASH);
	//InitAct(ATK_ACT::SKILL2, FRAME_SKILL2_DURATION, FRAME_SKILL2_BACKRASH);

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
	atkTypes_[static_cast<int>(ATK_ACT::SKILL1)] = ATK_TYPE::NORMALATK;
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
	if (IsAtkStart())
	{
		if (stepAnim_ >= 16.9f)
		{
			stepAnim_ = 16.9f;
		}
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
		SyncActPos(atk_.pos_);
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