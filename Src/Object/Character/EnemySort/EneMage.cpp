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
	skillAllCnt_ = 0.0f;
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

void EneMage::Attack(void)
{
	//共通処理
	Enemy::Attack();

	//スキル全体のカウント
	CntUp(skillAllCnt_);
}

void EneMage::Skill_One(void)
{
	//スキル１の生成上限
	if (skillOneShotCnt_ < SKILL_ONE_MAX_CNT)
	{
		//座標の設定
		skillOneShot_ = VAdd(skillOneShot_, GetTargetVec(skillOneShot_, SKILL_ONE_SPEED));

		//スキル１の発生間隔
		if (skillOneDelayCnt_ >= SKILL_ONE_SHOT_DELAY)
		{
			//攻撃発生
			//----------------------------

			//スキルの持続時間初期化
			skillOneDelayCnt_ = 0.0f;

			//攻撃回数増加
			skillOneShotCnt_++;

			//攻撃作成
			ATK& thisAtk = createSkill_();

			//生成した攻撃の位置を合わせる
			thisAtk.pos_ = skillOneShot_;
		}
	}

	//カウンタ
	CntUp(skillOneDelayCnt_);
}

void EneMage::DrawDebug(void)
{
	Enemy::DrawDebug();
	DrawSphere3D(skillOneShot_, 25.0f, 20, 0xf0f0f0, 0xf0f0f0, true);
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

	//向きを改めて設定
	trans_.quaRot = trans_.quaRot.LookRotation(GetTargetVec());

	//待機アニメーション
	ResetAnim(ANIM::UNIQUE_1, changeSpeedAnim_[ANIM::UNIQUE_1]);
}

void EneMage::ChangeStateAtk(void)
{
	//更新処理の中身初期化
	stateUpdate_ = std::bind(&EneMage::UpdateAtk, this);

	//向きを改めて設定
	trans_.quaRot = trans_.quaRot.LookRotation(GetTargetVec());

	//座標の初期設定
	skillOneShot_ = MV1GetFramePosition(trans_.modelId, FRAME_ROD);
}

void EneMage::UpdateAtk(void)
{
	//**********************************************************
	//終了処理
	//**********************************************************

	//攻撃が終わっているなら状態遷移
	if (skillAllCnt_ > SKILL_ONE_ALL_TIME)
	{
		//スキルの持続時間初期化
		skillOneDelayCnt_ = 0.0f;

		//攻撃回数初期化
		skillOneShotCnt_ = 0;

		//スキルのカウンタ初期化
		skillAllCnt_ = 0.0f;

		//休憩状態に遷移
		ChangeState(STATE::BREAK);
		return;
	}

	//**********************************************************
	//動作処理
	//**********************************************************

	//攻撃アニメーション
	ResetAnim(nowSkillAnim_, changeSpeedAnim_[nowSkillAnim_]);

	//攻撃処理
	Attack();
}
