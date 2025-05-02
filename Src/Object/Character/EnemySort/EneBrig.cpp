#include"Enemy.h"
#include "EneBrig.h"

EneBrig::EneBrig(const VECTOR& _spawnPos) : Enemy(_spawnPos)
{
	trans_.pos = _spawnPos;
}

void EneBrig::SetParam(void)
{
	//攻撃警告
	alertSkills_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneBrig::AlertSkill_One, this));

	//攻撃の遷移
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneBrig::Skill_One, this));

	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_BRIGANT));

	//※個々で設定する
	trans_.scl = { CHARACTER_SCALE,CHARACTER_SCALE,CHARACTER_SCALE };
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	localCenterPos_ = LOCAL_CENTER_POS;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;

	//外部からステータスを取得
	ParamLoad(CharacterParamData::UNIT_TYPE::E_KNIGHT);
}

void EneBrig::InitAnim(void)
{
	//共通アニメーション初期化
	Enemy::InitAnim();

	//固有アニメーション初期化
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_PRE_SWING);

	//アニメーション速度設定
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_1, SPEED_ANIM);

	//アニメーションリセット
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneBrig::InitSkill(void)
{
	//ここにスキルの数分格納させる
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);

	//ここにスキルの数分アニメーションを格納させる
	//----------------------------------------------

	//予備動作アニメーション
	skillPreAnims_.emplace_back(ANIM::UNIQUE_1);

	//動作アニメーション
	skillAnims_.emplace_back(ANIM::SKILL_1);


	//初期スキルを設定しておく
	RandSkill();
}

void EneBrig::AlertSkill_One(void)
{
	//敵の前方
	VECTOR pos = trans_.GetForward();

	//攻撃範囲
	pos = VScale(pos, SKILL_ONE_COL_RADIUS);

	//座標合わせ
	pos = VAdd(trans_.pos, pos);

	//範囲作成
	CreateAlert(pos, SKILL_ONE_COL_RADIUS * 2, SKILL_ONE_COL_RADIUS * 2);
}

void EneBrig::Skill_One(void)
{
	//終了判定
	if (lastAtk_->IsFinishMotion())
	{
		//攻撃終了
		isEndAllAtk_ = true;

		//処理終了
		return;
	}

	for (auto& nowSkill : nowSkill_)
	{
		//座標の設定
		nowSkill.pos_ = MV1GetFramePosition(trans_.modelId, FRAME_KNIFE);
	}
}

void EneBrig::FinishAnim(void)
{
	//共通アニメーションの終了処理
	Enemy::FinishAnim();

	switch (anim_)
	{
	case UnitBase::ANIM::UNIQUE_1:
		break;
	}
}