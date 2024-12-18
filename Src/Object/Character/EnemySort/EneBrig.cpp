#include"../Enemy.h"
#include "EneBrig.h"

void EneBrig::SetParam(void)
{
	//攻撃の遷移
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneBrig::Skill_One, this));

	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_BRIGANT));

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
}

void EneBrig::InitAnim(void)
{
	//共通アニメーション初期化
	Enemy::InitAnim();

	//固有アニメーション初期化
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);

	//アニメーション速度設定
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM);

	//アニメーションリセット
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneBrig::InitSkill(void)
{
	//ここにスキルの数分格納させる
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);

	//ここにスキルの数分アニメーションを格納させる
	skillAnims_.emplace_back(ANIM::SKILL_1);

	//初期スキルを設定しておく
	RandSkill();
}

void EneBrig::Attack(void)
{
	//対応スキル発動
	processSkill_();
}

void EneBrig::Skill_One(void)
{
	//前方向
	VECTOR dir = trans_.quaRot.GetForward();

	for (auto& nowSkill : nowSkill_)
	{
		//座標の設定
		nowSkill.pos_ = VAdd(colPos_, VScale(dir, nowSkill.radius_ + radius_));
	}
}

void EneBrig::ChangeStateAlert(void)
{
	//更新処理の中身初期化
	Enemy::ChangeStateAlert();

	//待機アニメーション
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}