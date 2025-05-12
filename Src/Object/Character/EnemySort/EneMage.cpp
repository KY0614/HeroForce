#include"Enemy.h"
#include "EneMage.h"

EneMage::EneMage(const VECTOR& _spawnPos) : Enemy(_spawnPos)
{
	trans_.pos = _spawnPos;
}

void EneMage::Destroy(void)
{
	auto& eff = EffectManager::GetInstance();

	//共通
	Enemy::Destroy();

	eff.Stop(EffectManager::EFFECT::STATE_DOWN);
}

void EneMage::SetParam(void)
{
	//攻撃警告
	alertSkills_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneMage::AlertSkill_One, this));

	//攻撃の遷移
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneMage::Skill_One, this));

	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MAGE));

	//※個々で設定する
	trans_.scl = { CHARACTER_SCALE,CHARACTER_SCALE,CHARACTER_SCALE };
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	localCenterPos_ = LOCAL_CENTER_POS;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
	
	skillOneShot_ = AsoUtility::VECTOR_ZERO;
	skillOneDelayCnt_ = 0.0f;
	skillAllCnt_ = 0.0f;

	//外部からステータスを取得
	ParamLoad(CharacterParamData::UNIT_TYPE::E_MAGE);
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

void EneMage::InitEffect(void)
{
	auto& eff = EffectManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	eff.Add(EffectManager::EFFECT::STATE_DOWN,
		res.Load(ResourceManager::SRC::STATE_DOWN_EFE).handleId_);
}

void EneMage::InitSkill(void)
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

void EneMage::AlertSkill_One(void)
{
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
	//エフェクト
	auto& eff =	EffectManager::GetInstance();

	//終了判定
	if (skillAllCnt_ > SKILL_ONE_ALL_TIME)
	{
		//攻撃終了
		isEndAllAtk_ = true;

		//処理終了
		return;
	}

	//スキル１の生成上限
	if (skillOneShotCnt_ < SKILL_ONE_MAX_CNT)
	{
		//座標の設定
		skillOneShot_ = VAdd(skillOneShot_, GetMoveVec(skillOneShot_, targetPos_,SKILL_ONE_SPEED));

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

			//エフェクト再生
			eff.Play(EffectManager::EFFECT::STATE_DOWN,
				thisAtk.pos_,
				Quaternion(),
				SKILL_ONE_EFF_SIZE,
				SoundManager::SOUND::NONE);
		}
	}

	//カウンタ
	CntUp(skillOneDelayCnt_);
}

void EneMage::DrawDebug(void)
{
	Enemy::DrawDebug();
	
	if(!isEndAllAtk_)DrawSphere3D(skillOneShot_, 25.0f, 20, 0xf0f0f0, 0xf0f0f0, true);
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

void EneMage::ResetAtkJudge(void)
{
	//共通
	Enemy::ResetAtkJudge();

	//スキルの持続時間初期化
	skillOneDelayCnt_ = 0.0f;

	//攻撃回数初期化
	skillOneShotCnt_ = 0;

	//スキルのカウンタ初期化
	skillAllCnt_ = 0.0f;
}

void EneMage::ChangeStateAtk(void)
{
	//更新処理の中身初期化
	stateUpdate_ = std::bind(&EneMage::UpdateAtk, this);

	//向きを改めて設定
	trans_.quaRot = trans_.quaRot.LookRotation(GetMoveVec(trans_.pos, targetPos_));

	//座標の初期設定
	skillOneShot_ = MV1GetFramePosition(trans_.modelId, FRAME_ROD);
}