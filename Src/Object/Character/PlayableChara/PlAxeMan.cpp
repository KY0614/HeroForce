#include"../Manager/ResourceManager.h"
#include "PlAxeMan.h"

PlAxe::PlAxe(SceneManager::PLAY_MODE _mode) : PlayerBase(_mode) { mode_ = _mode; }

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

	//当たり判定の設定
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	atkStartRange_ = ATK_START_RANGE;
}
void PlAxe::InitAct(void)
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
	dulationMax_.emplace(ATK_ACT::ATK,FRAME_ATK_DURATION);
	dulationMax_.emplace(ATK_ACT::SKILL1,FRAME_SKILL1_DURATION);
	dulationMax_.emplace(ATK_ACT::SKILL2,FRAME_SKILL2_DURATION);

	//後隙
	backLashMax_.emplace(ATK_ACT::ATK,FRAME_ATK_BACKRASH);
	backLashMax_.emplace(ATK_ACT::SKILL1,FRAME_SKILL1_BACKRASH);
	backLashMax_.emplace(ATK_ACT::SKILL2,FRAME_SKILL2_BACKRASH);

	//クールタイム
	coolTimeMax_[static_cast<int>(ATK_ACT::ATK)] = ATK_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_COOLTIME;
}

void PlAxe::AtkFunc(void)
{
}

void PlAxe::Skill1Func(void)
{
}

void PlAxe::Skill2Func(void)
{

}
