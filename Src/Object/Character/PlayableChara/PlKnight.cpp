#include"../Manager/ResourceManager.h"
#include"../Manager/SceneManager.h"
#include "PlKnight.h"
PlKnight::PlKnight(SceneManager::PLAY_MODE _mode) :PlayerBase(_mode) { mode_ = _mode; }


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

	//ìñÇΩÇËîªíËÇÃê›íË
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	atkStartRange_ = ATK_START_RANGE;
}

void PlKnight::AtkFunc(void)
{
}

void PlKnight::Skill1Func(void)
{
}

void PlKnight::Skill2Func(void)
{
}

