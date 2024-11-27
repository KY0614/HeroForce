#include"../Manager/ResourceManager.h"
#include"../Manager/SceneManager.h"
#include "PlKnight.h"
PlKnight::PlKnight(SceneManager::PLAY_MODE _mode, InputManager::JOYPAD_NO _padNum) :PlayerBase(_mode, padNum_)
{
	mode_ = _mode;
	padNum_ = _padNum;
}


void PlKnight::SetParam(void)
{
	//acts_[ATK_ACT::ATK].pos_ = VAdd(trans_.pos, ATK_COL_LOCAL_POS);
	//acts_[ATK_ACT::ATK].ResetCnt();
	//acts_[ATK_ACT::ATK].duration_ = FRAME_ATK_DURATION;
	//acts_[ATK_ACT::ATK].backlash_ = FRAME_ATK_BACKRASH;
	//acts_[ATK_ACT::ATK].pow_ = 0;

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
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	atkStartRange_ = ATK_START_RANGE;
}

void PlKnight::InitAct(void)
{

	//範囲
	colRadius_.emplace(ATK_ACT::ATK, COL_ATK);
	colRadius_.emplace(ATK_ACT::SKILL1, COL_SKILL1);
	colRadius_.emplace(ATK_ACT::SKILL2, COL_SKILL2);

	//座標
	colLocalPos_.emplace(ATK_ACT::ATK, ATK_COL_LOCAL_POS);
	colLocalPos_.emplace(ATK_ACT::SKILL1, SKILL1_COL_LOCAL_POS);
	colLocalPos_.emplace(ATK_ACT::SKILL2, SKILL2_COL_LOCAL_POS);

	//持続時間
	dulationMax_.emplace(ATK_ACT::ATK, FRAME_ATK_DURATION);
	dulationMax_.emplace(ATK_ACT::SKILL1, FRAME_SKILL1_DURATION);
	dulationMax_.emplace(ATK_ACT::SKILL2, FRAME_SKILL2_DURATION);

	//後隙
	backLashMax_.emplace(ATK_ACT::ATK, FRAME_ATK_BACKRASH);
	backLashMax_.emplace(ATK_ACT::SKILL1, FRAME_SKILL1_BACKRASH);
	backLashMax_.emplace(ATK_ACT::SKILL2, FRAME_SKILL2_BACKRASH);

	//クールタイム
	coolTimeMax_[static_cast<int>(ATK_ACT::ATK)] = ATK_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_COOLTIME;

	//攻撃発生時間
	atkStartTime_[static_cast<int>(ATK_ACT::ATK)] = ATK_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_START;

	//攻撃タイプ
	atkType_[static_cast<int>(ATK_ACT::ATK)] = ATK_TYPE::NORMALATK;
	atkType_[static_cast<int>(ATK_ACT::SKILL1)] = ATK_TYPE::NORMALATK;
	atkType_[static_cast<int>(ATK_ACT::SKILL2)] = ATK_TYPE::CHARGEATK;

}

void PlKnight::AtkFunc(void)
{

}

void PlKnight::ResetGuardCnt(void)
{
	coolTime_[static_cast<int>(SKILL_NUM::TWO)] -= 3.0f;
}



void PlKnight::Skill1Func(void)
{
	//斬撃飛ばす
}

void PlKnight::Skill2Func(void)
{
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
	else
	{
		dulationMax_[ATK_ACT::SKILL2] = coolTime_[static_cast<int>(ATK_ACT::SKILL2)];
	}
}

void PlKnight::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::UNIQUE_2, BLOCKING_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}
