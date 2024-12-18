#include"../../../Manager/Resource.h"
#include"../../../Manager/ResourceManager.h"
#include"../Enemy.h"
#include "EneMage.h"

void EneMage::SetParam(void)
{
	//攻撃の遷移
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneMage::Skill_One, this));

	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MAGE));

	//※個々で設定する
	trans_.scl = { CHARACTER_SCALE,CHARACTER_SCALE,CHARACTER_SCALE };
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
	
	skillOneShot_ = AsoUtility::VECTOR_ZERO;
	skillOneDelayCnt_ = 0.0f;
}

void EneMage::InitAnim(void)
{
	//共通アニメーション初期化
	Enemy::InitAnim();

	//固有アニメーション初期化
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_CHARGE);

	//アニメーション速度設定
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_1, SPEED_ANIM);

	//アニメーションリセット
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneMage::InitSkill(void)
{
	//ここにスキルの数分格納させる
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);

	//ここにスキルの数分アニメーションを格納させる
	skillAnims_.emplace_back(ANIM::SKILL_1);

	//初期スキルを設定しておく
	RandSkill();
}

void EneMage::Skill_One(void)
{
	//初期値設定
	if (skillOneDelayCnt_ == 0.0f)
		skillOneShot_ = trans_.pos;

	//カウンタ
	if(skillOneDelayCnt_ <= SKILL_ONE_MAX_TIME)
	CntUp(skillOneDelayCnt_);

	//座標の設定
	skillOneShot_ = VAdd(skillOneShot_, GetTargetVec(skillOneShot_, SKILL_ONE_SPEED));

	if (static_cast<int>(skillOneDelayCnt_) % 7 == 0.0f)
	{
		//攻撃作成
		//createSkill_();
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

void EneMage::ChangeStateAlert(void)
{
	//更新処理の中身初期化
	Enemy::ChangeStateAlert();

	//待機アニメーション
	ResetAnim(ANIM::UNIQUE_1, changeSpeedAnim_[ANIM::UNIQUE_1]);
}