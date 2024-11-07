#include"../../../Manager/Resource.h"
#include"../../../Manager/ResourceManager.h"
#include"../Enemy.h"
#include "EneAxe.h"

void EneAxe::SetParam(void)
{
	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_AXEMAN));

	//※個々で設定する
	radius_ = MY_COL_RADIUS;
	localCenterPos_ = LOCAL_CENTER_POS;
	colPos_ = VAdd(trans_.pos, localCenterPos_);
	hp_ = 5;
	walkSpeed_ = WALK_SPEED;
	stunDefMax_ = 100;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
}

void EneAxe::InitAnimNum(void)
{
	//共通アニメーション初期化
	Enemy::InitAnimNum();

	//固有アニメーション初期化
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_1);
	animNum_.emplace(ANIM::SKILL_2, ANIM_SKILL_2);
}

void EneAxe::InitSkill(void)
{
	//ここにスキルの数分格納させる
	skills_.emplace_back(SKILL_1);
	skills_.emplace_back(SKILL_2);

	//ここにスキルの数分アニメーションを格納させる
	skillAnims_.emplace_back(ANIM::SKILL_1);
	skillAnims_.emplace_back(ANIM::SKILL_2);

	//初期スキルを設定しておく
	RandSkill();
}

void EneAxe::Attack(void)
{
	//現在のスキルの確認
	if (nowSkill_.front().radius_ == SKILL_1.radius_
		&& nowSkill_.front().backlash_ == SKILL_1.backlash_
		&& nowSkill_.front().duration_ == SKILL_1.duration_
		&& nowSkill_.front().pow_ == SKILL_1.pow_)
	{
		//スキル1発動
		Skill_1();
	}
	else if (nowSkill_.front().radius_ == SKILL_2.radius_
		&& nowSkill_.front().backlash_ == SKILL_2.backlash_
		&& nowSkill_.front().duration_ == SKILL_2.duration_
		&& nowSkill_.front().pow_ == SKILL_2.pow_)
	{
		//スキル2発動
		Skill_2();
	}
}

void EneAxe::Skill_1(void)
{
	//前方向
	VECTOR dir = trans_.quaRot.GetForward();

	for (auto& nowSkill : nowSkill_)
	{
		//座標の設定
		nowSkill.pos_ = VAdd(colPos_, VScale(dir, nowSkill.radius_));
	}
}

void EneAxe::Skill_2(void)
{
	//前方向
	VECTOR dir = trans_.quaRot.GetForward();

	for (auto& nowSkill : nowSkill_)
	{
		//座標の設定
		nowSkill.pos_ = VAdd(colPos_, VScale(dir, nowSkill.radius_));
	}
}
