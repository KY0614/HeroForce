#include"../../../Manager/Resource.h"
#include"../../../Manager/ResourceManager.h"
#include"../Enemy.h"
#include "EneGolem.h"

void EneGolem::SetParam(void)
{
	//攻撃の遷移
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneGolem::Skill_One, this));
	changeSkill_.emplace(ATK_ACT::SKILL_TWO, std::bind(&EneGolem::Skill_Two, this));

	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_GOLEM));

	//※個々で設定する
	trans_.scl = { SCALE,SCALE,SCALE };
	radius_ = MY_COL_RADIUS;
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	hp_ = HP_MAX;
	atkPow_ = ATK_POW;
	def_ = DEF;
	walkSpeed_ = WALK_SPEED;
	localCenterPos_ = LOCAL_CENTER_POS;
	stunDefMax_ = STUN_DEF_MAX;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
}

void EneGolem::InitAnim(void)
{
	//※ゴーレムは全部固有アニメーション

	//固有アニメーション初期化
	animNum_.emplace(ANIM::IDLE, EneGolem::ANIM_IDLE);
	animNum_.emplace(ANIM::WALK, EneGolem::ANIM_WALK);
	animNum_.emplace(ANIM::SKILL_1, ANIM_PUNCH);
	animNum_.emplace(ANIM::SKILL_2, ANIM_MOWDOWN);
	animNum_.emplace(ANIM::SKILL_3, ANIM_SHOUT);
	animNum_.emplace(ANIM::SKILL_4, ANIM_JUMP_ATK);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_JUMP);
	animNum_.emplace(ANIM::UNIQUE_2, ANIM_LANDING);

	//アニメーション速度設定
	changeSpeedAnim_.emplace(ANIM::IDLE, SPEED_ANIM_IDLE);
	changeSpeedAnim_.emplace(ANIM::WALK, SPEED_ANIM_WALK);
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM_PUNCH);
	changeSpeedAnim_.emplace(ANIM::SKILL_2, SPEED_ANIM_MOWDOWN);
	changeSpeedAnim_.emplace(ANIM::SKILL_3, SPEED_ANIM_SHOUT);
	changeSpeedAnim_.emplace(ANIM::SKILL_4, SPEED_ANIM_JUMP);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_1, SPEED_ANIM_JUMP);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_2, SPEED_ANIM_JUMP);

	//アニメーションリセット
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneGolem::InitSkill(void)
{
	//ここにスキルの数分格納させる
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);
	skills_.emplace(ATK_ACT::SKILL_TWO, SKILL_TWO);

	//ここにスキルの数分アニメーションを格納させる
	skillAnims_.emplace_back(ANIM::SKILL_1);
	skillAnims_.emplace_back(ANIM::SKILL_2);

	//初期スキルを設定しておく
	RandSkill();
}

void EneGolem::Skill_One(void)
{
	for (auto& nowSkill : nowSkill_)
	{
		//座標の設定
		nowSkill.pos_ = MV1GetFramePosition(trans_.modelId, FRAME_R_HAND);
	}
}

void EneGolem::Skill_Two(void)
{
	for (auto& nowSkill : nowSkill_)
	{
		//座標の設定
		nowSkill.pos_ = MV1GetFramePosition(trans_.modelId, FRAME_R_HAND);
	}
}

void EneGolem::ChangeStateAlert(void)
{
	//更新処理の中身初期化
	Enemy::ChangeStateAlert();

	//待機アニメーション
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}
