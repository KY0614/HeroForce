#include"../../../Manager/Resource.h"
#include"../../../Manager/ResourceManager.h"
#include"../Enemy.h"
#include "EneMage.h"

void EneMage::SetParam(void)
{
	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MAGE));

	//※個々で設定する
	radius_ = MY_COL_RADIUS;
	localCenterPos_ = LOCAL_CENTER_POS;
	colPos_ = VAdd(trans_.pos, localCenterPos_);
	hp_ = HP_MAX;
	walkSpeed_ = WALK_SPEED;
	stunDefMax_ = STUN_DEF_MAX;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
}

void EneMage::InitAnimNum(void)
{
	//共通アニメーション初期化
	Enemy::InitAnimNum();

	//固有アニメーション初期化
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
	animNum_.emplace(ANIM::SKILL_2, ANIM_SKILL_TWO);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_CHARGE);
}

void EneMage::InitSkill(void)
{
	//ここにスキルの数分格納させる
	skills_.emplace_back(SKILL_ONE);
	skills_.emplace_back(SKILL_TWO);

	//ここにスキルの数分アニメーションを格納させる
	skillAnims_.emplace_back(ANIM::SKILL_1);
	skillAnims_.emplace_back(ANIM::SKILL_2);

	//初期スキルを設定しておく
	RandSkill();
}

void EneMage::Attack(void)
{
	//現在のスキルの確認
	if (nowSkill_.front().radius_ == SKILL_ONE.radius_
		&& nowSkill_.front().backlash_ == SKILL_ONE.backlash_
		&& nowSkill_.front().duration_ == SKILL_ONE.duration_
		&& nowSkill_.front().pow_ == SKILL_ONE.pow_)
	{
		//スキル1発動
		Skill_One();
	}
	else if (nowSkill_.front().radius_ == SKILL_TWO.radius_
		&& nowSkill_.front().backlash_ == SKILL_TWO.backlash_
		&& nowSkill_.front().duration_ == SKILL_TWO.duration_
		&& nowSkill_.front().pow_ == SKILL_TWO.pow_)
	{
		//スキル2発動
		Skill_Two();
	}
}

void EneMage::Skill_One(void)
{
	//前方向
	VECTOR dir = trans_.quaRot.GetForward();

	for (auto& nowSkill : nowSkill_)
	{
		//座標の設定
		nowSkill.pos_ = VAdd(colPos_, VScale(dir, nowSkill.radius_ + radius_));
	}
}

void EneMage::Skill_Two(void)
{
	//前方向
	VECTOR dir = trans_.quaRot.GetForward();

	for (auto& nowSkill : nowSkill_)
	{
		//座標の設定
		nowSkill.pos_ = VAdd(colPos_, VScale(dir, nowSkill.radius_ + radius_));
	}
}

void EneMage::FinishAnim(void)
{
	//共通アニメーションの終了処理
	Enemy::FinishAnim();

	switch (anim_)
	{
	case UnitBase::ANIM::UNIQUE_1:
		//ループ再生
		stepAnim_ = 0;
		break;
	}
}

void EneMage::InitChangeState(void)
{
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		break;

	case Enemy::STATE::ALERT:
		//向きを改めて設定
		trans_.quaRot = trans_.quaRot.LookRotation(GetTargetVec());

		//溜めアニメーション
		ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM);

		//警告カウンタ初期化
		alertCnt_ = 0.0f;
		break;

	case Enemy::STATE::ATTACK:
		break;

	case Enemy::STATE::BREAK:
		//攻撃休憩時間の初期化
		breakCnt_ = 0;
		break;
	}
}