#include"../Manager/ResourceManager.h"
#include"../Manager/SceneManager.h"
#include "PlKnight.h"
PlKnight::PlKnight(const SceneManager::PLAY_MODE _mode, const SceneManager::CNTL _cntl) :PlayerBase(_mode, _cntl)
{
	mode_ = _mode;
	cntl_ = _cntl;
}

PlKnight::PlKnight(const SceneManager::PLAY_MODE _mode, const InputManager::JOYPAD_NO _padNum) :PlayerBase(_mode, _padNum)
{
	mode_ = _mode;
	padNum_ = _padNum;
}


void PlKnight::SetParam(void)
{
	InitAct();

	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
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

	//当たり判定の設定
	radius_ = MY_COL_RADIUS;

	atkStartRange_ = ATK_START_RANGE;
}

void PlKnight::InitAct(void)
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
	atkTypes_[static_cast<int>(ATK_ACT::SKILL2)] = ATK_TYPE::CHARGEATK;

}

void PlKnight::AtkFunc(void)
{

}

void PlKnight::ResetGuardCnt(void)
{
	if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] > GUARD_STARTABLE_COOL)
	{
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = false;
		coolTime_[static_cast<int>(SKILL_NUM::TWO)] -= SKILL_TWO_START_COOLTIME;
		atkMax_[ATK_ACT::SKILL2].duration_ = coolTime_[static_cast<int>(ATK_ACT::SKILL2)];
		CntUp(atkStartCnt_);
	}
}



void PlKnight::Skill1Func(void)
{
	//斬撃飛ばす
}

void PlKnight::Skill2Func(void)
{
	if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] <= 0.0f || !isPush_)
	{
		//スキル切り替え出来ないようにする
		return;
	}

	if (IsAtkStart())CntUp(atkStartCnt_);

	if (IsFinishAtkStart())
	{
		CntUp(atk_.cnt_);
	}
	if (IsAtkAction())
	{
		if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] > 0.0f)
		{
			moveAble_ = false;
			isCool_[static_cast<int>(SKILL_NUM::TWO)] = false;
			CntDown(coolTime_[static_cast<int>(SKILL_NUM::TWO)]);
			if (stepAnim_ >= 10.0f)
			{
				stepAnim_ = 10.0f;
			}
		}
	}
	else if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] <= SKILL_TWO_START_COOLTIME)
	{
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
		return;
	}
}

void PlKnight::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::UNIQUE_2, BLOCKING_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}
