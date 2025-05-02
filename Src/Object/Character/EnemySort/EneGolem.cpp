#include<random>
#include"Enemy.h"
#include "EneGolem.h"

EneGolem::EneGolem(const VECTOR& _spawnPos) : Enemy(_spawnPos)
{
	trans_.pos = _spawnPos;
}

void EneGolem::Destroy(void)
{
	auto& eff = EffectManager::GetInstance();

	//共通
	Enemy::Destroy();

	//eff.Stop(EffectManager::EFFECT::BOSS_PUNCH);
	eff.Stop(EffectManager::EFFECT::BOSS_SHOUT);
	eff.Stop(EffectManager::EFFECT::BOSS_SHOUT_ATK);
}

void EneGolem::SetParam(void)
{
	//攻撃警告
	alertSkills_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneGolem::AlertSkill_One, this));
	alertSkills_.emplace(ATK_ACT::SKILL_TWO, std::bind(&EneGolem::AlertSkill_Two, this));
	alertSkills_.emplace(ATK_ACT::SKILL_THREE, std::bind(&EneGolem::AlertSkill_Three, this));

	//攻撃の遷移
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneGolem::Skill_One, this));
	changeSkill_.emplace(ATK_ACT::SKILL_TWO, std::bind(&EneGolem::Skill_Two, this));
	changeSkill_.emplace(ATK_ACT::SKILL_THREE, std::bind(&EneGolem::Skill_Three, this));

	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_GOLEM));

	//※個々で設定する
	trans_.scl = { SCALE,SCALE,SCALE };
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	localCenterPos_ = LOCAL_CENTER_POS;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
	skillThreeCnt_ = 0;
	skillThreeDelayCnt_ = 0.0f;
	isPreSkillThreeAtk_ = false;

	//外部からステータスを取得
	ParamLoad(CharacterParamData::UNIT_TYPE::BOSS);
}

void EneGolem::InitAnim(void)
{
	//※ゴーレムは全部固有アニメーション

	//固有アニメーション初期化
	animNum_.emplace(ANIM::IDLE, EneGolem::ANIM_IDLE);
	animNum_.emplace(ANIM::WALK, EneGolem::ANIM_WALK);
	animNum_.emplace(ANIM::RUN, EneGolem::ANIM_WALK);
	animNum_.emplace(ANIM::DEATH, EneGolem::ANIM_KNOCK);
	animNum_.emplace(ANIM::SKILL_1, ANIM_PUNCH);
	animNum_.emplace(ANIM::SKILL_2, ANIM_MOWDOWN);
	animNum_.emplace(ANIM::SKILL_3, ANIM_SHOUT);
	animNum_.emplace(ANIM::SKILL_4, ANIM_JUMP_ATK);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_JUMP);
	animNum_.emplace(ANIM::UNIQUE_2, ANIM_LANDING);
	animNum_.emplace(ANIM::UNIQUE_3, ANIM_PRE_PUNCH);
	animNum_.emplace(ANIM::UNIQUE_4, ANIM_PRE_MOWDOWN);
	animNum_.emplace(ANIM::UNIQUE_5, ANIM_PRE_SHOUT);

	//アニメーション速度設定
	changeSpeedAnim_.emplace(ANIM::IDLE, SPEED_ANIM_IDLE);
	changeSpeedAnim_.emplace(ANIM::WALK, SPEED_ANIM_WALK);
	changeSpeedAnim_.emplace(ANIM::RUN, SPEED_ANIM_WALK);
	changeSpeedAnim_.emplace(ANIM::DEATH, SPEED_ANIM_KNOCK);
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM_PUNCH);
	changeSpeedAnim_.emplace(ANIM::SKILL_2, SPEED_ANIM_MOWDOWN);
	changeSpeedAnim_.emplace(ANIM::SKILL_3, SPEED_ANIM_SHOUT);
	changeSpeedAnim_.emplace(ANIM::SKILL_4, SPEED_ANIM_JUMP);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_1, SPEED_ANIM_JUMP);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_2, SPEED_ANIM_JUMP);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_3, SPEED_ANIM_PRE_PUNCH);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_4, SPEED_ANIM_PRE_MOWDOWN);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_5, SPEED_ANIM_PRE_SHOUT);

	//アニメーションリセット
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneGolem::InitEffect(void)
{
	auto& eff = EffectManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	//パンチエフェクト
	//eff.Add(EffectManager::EFFECT::BOSS_PUNCH,
	//	res.Load(ResourceManager::SRC::BOSS_PUNCH_EFE).handleId_);

	//叫びエフェクト
	eff.Add(EffectManager::EFFECT::BOSS_SHOUT,
		res.Load(ResourceManager::SRC::BOSS_SHOUT_EFE).handleId_);

	//叫び(攻撃)エフェクト
	eff.Add(EffectManager::EFFECT::BOSS_SHOUT_ATK,
		res.Load(ResourceManager::SRC::BOSS_SHOUT_ATK_EFE).handleId_);
}

void EneGolem::InitSkill(void)
{
	//ここにスキルの数分格納させる
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);
	skills_.emplace(ATK_ACT::SKILL_TWO, SKILL_TWO);
	skills_.emplace(ATK_ACT::SKILL_THREE, SKILL_THREE);

	//ここにスキルの数分アニメーションを格納させる
	//----------------------------------------------

	//予備動作アニメーション
	skillPreAnims_.emplace_back(ANIM::UNIQUE_3);
	skillPreAnims_.emplace_back(ANIM::UNIQUE_4);
	skillPreAnims_.emplace_back(ANIM::UNIQUE_5);

	//動作アニメーション
	skillAnims_.emplace_back(ANIM::SKILL_1);
	skillAnims_.emplace_back(ANIM::SKILL_2);
	skillAnims_.emplace_back(ANIM::SKILL_3);

	//初期スキルを設定しておく
	RandSkill();
}

void EneGolem::Alert(void)
{
	//警告
	alertNowSkill_();

	//クールダウンカウンタ
	CntUp(alertCnt_);

	//スキル３以外は通常通り
	if (!IsAlertTime() && atkAct_ != ATK_ACT::SKILL_THREE)isEndAlert_ = true;
}

void EneGolem::AlertSkill_One(void)
{
	//エフェクトマネージャー
	auto& eff = EffectManager::GetInstance();

	//座標
	VECTOR pos = trans_.pos;

	//相対座標合わせ
	pos = VAdd(pos, Quaternion::PosAxis(trans_.quaRot, SKILL_ONE_ALERT));

	//範囲作成
	CreateAlert(pos, SKILL_ONE_COL_RADIUS * 2, SKILL_ONE_COL_RADIUS * 2);
}

void EneGolem::AlertSkill_Two(void)
{
	//座標
	VECTOR pos = trans_.pos;

	//相対座標合わせ
	pos = VAdd(pos, Quaternion::PosAxis(trans_.quaRot, SKILL_TWO_ALERT));

	//範囲作成
	CreateAlert(pos, SKILL_TWO_COL_RADIUS * 2 + SKILL_TWO_ALERT_RANGE_X, SKILL_TWO_COL_RADIUS * 2);
}

void EneGolem::AlertSkill_Three(void)
{
	//攻撃の数分予兆発生
	if (skillThreeCnt_ >= SKILL_THREE_MAX_CNT
		&& skillThreeDelayCnt_ >= SKILL_THREE_DELAY)
	{
		//生成終了
		isEndAlert_ = true;

		//処理終了
		return;
	}

	if (skillThreeDelayCnt_ < SKILL_THREE_DELAY)
	{
		//カウント
		CntUp(skillThreeDelayCnt_);

		return;
	}
	
	//攻撃生成できたかの判定
	bool isGenelateAttack = false;

	//生成できるまで繰り返す
	while (!isGenelateAttack)
	{
		//円範囲の中の一点を取る
		GetRandomPointInCircle(trans_.pos, SKILL_THREE_FALL_RADIUS, skillThreePreAtk_[skillThreeCnt_].pos_);

		if (!IsOverlap(skillThreePreAtk_[skillThreeCnt_], SKILL_THREE_COL_RADIUS * 2))
		{
			//生成完了
			isGenelateAttack = true;
		}
	}

	//生成された判定用
	CntUp(skillThreePreAtk_[skillThreeCnt_].cnt_);

	//範囲作成
	CreateAlert(skillThreePreAtk_[skillThreeCnt_].pos_, SKILL_THREE_COL_RADIUS * 2, SKILL_THREE_COL_RADIUS * 2);

	//初期化
	skillThreeDelayCnt_ = 0.0f;

	//カウント
	skillThreeCnt_++;
}

void EneGolem::Skill_One(void)
{
	//エフェクトマネージャー
	auto& eff = EffectManager::GetInstance();

	//if (stepAnim_ == 20.0f)
		//エフェクト再生
		/*eff.Play(EffectManager::EFFECT::BOSS_PUNCH,
			MV1GetFramePosition(trans_.modelId, FRAME_R_HAND),
			trans_.quaRot,
			SKILL_ONE_EFF_SIZE,
			SoundManager::SOUND::NONE);*/


	//攻撃の再生成
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
		nowSkill.pos_ = MV1GetFramePosition(trans_.modelId, FRAME_R_HAND);
	}
}

void EneGolem::Skill_Two(void)
{
	//攻撃の再生成
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
		nowSkill.pos_ = MV1GetFramePosition(trans_.modelId, FRAME_R_HAND);
	}
}

void EneGolem::Skill_Three(void)
{
	//エフェクトマネージャー
	auto& eff = EffectManager::GetInstance();

	//攻撃終了判定
	if (lastAtk_ == nullptr ? false : lastAtk_->IsFinishMotion()
		&& skillThreeCnt_ >= SKILL_THREE_MAX_CNT)
	{
		//攻撃終了
		isEndAllAtk_ = true;

		//処理終了
		return;
	}

	//攻撃の間隔と生成上限
	if (skillThreeDelayCnt_ > SKILL_THREE_DELAY
		&& skillThreeCnt_ < SKILL_THREE_MAX_CNT)
	{
		if(skillThreeCnt_ == 0)
			//エフェクト再生
			eff.Play(EffectManager::EFFECT::BOSS_SHOUT,
				MV1GetFramePosition(trans_.modelId, FRAME_HEAD),
				trans_.quaRot,
				SKILL_ONE_EFF_SIZE,
				SoundManager::SOUND::NONE);


		//間隔カウンタの初期化
		skillThreeDelayCnt_ = 0.0f;

		//スキル生成
		ATK& thisAtk = createSkill_();

		//最後に生成された攻撃を格納
		lastAtk_ = &thisAtk;

		//保持していた座標情報をもとにセット
		thisAtk.pos_ = skillThreePreAtk_[skillThreeCnt_].pos_;

		//エフェクトの再生
		eff.Play(EffectManager::EFFECT::BOSS_SHOUT_ATK,
			thisAtk.pos_,
			trans_.quaRot,
			SKILL_THREE_ATK_EFF_SIZE,
			SoundManager::SOUND::NONE);

		//カウンタ増加
		skillThreeCnt_++;
	}

	//間隔カウンタ
	CntUp(skillThreeDelayCnt_);
}

void EneGolem::ResetAlertJudge(void)
{
	//共通
	Enemy::ResetAlertJudge();

	//スキル３の生成カウンタ初期化
	skillThreeCnt_ = 0;

	//スキル３の生成間隔カウンタ初期化
	skillThreeDelayCnt_ = 0.0f;

	//スキル３の発生予兆生成判定初期化
	isPreSkillThreeAtk_ = false;
}

void EneGolem::ResetAtkJudge(void)
{
	//共通
	Enemy::ResetAtkJudge();

	//スキル３の生成カウンタ初期化
	skillThreeCnt_ = 0;

	//スキル３の生成間隔カウンタ初期化
	skillThreeDelayCnt_ = 0.0f;
}

void EneGolem::ChangeStateAtk(void)
{
	//エフェクトマネージャー
	auto& eff = EffectManager::GetInstance();

	//更新処理の中身初期化
	stateUpdate_ = std::bind(&EneGolem::UpdateAtk, this);

	//最初に攻撃を生成するか
	if (atkAct_ != ATK_ACT::SKILL_THREE)
		//攻撃生成
		lastAtk_ = &createSkill_();
}

void EneGolem::GetRandomPointInCircle(const VECTOR _myPos, const int _r, VECTOR& _tPos)
{
	// ランダムエンジンを生成
	std::random_device rd;
	std::mt19937 gen(rd());

	//ランダム角度の範囲(0 ～ 2π)
	std::uniform_real_distribution<> dis(0, 2 * DX_PI_F);

	//ランダム半径の範囲(0 ～ r)
	std::uniform_real_distribution<> randRadius(0, _r);

	// ランダムな角度theta
	float theta = dis(gen);

	// ランダムな半径r' (0 ～ r) で、均等に分布するように sqrt を使う
	float radius = randRadius(gen);

	// 円内の点を計算
	_tPos.x = static_cast<int>(_myPos.x + radius * cos(theta));
	_tPos.z = static_cast<int>(_myPos.z + radius * sin(theta));
}

bool EneGolem::IsOverlap(ATK& _thisAtk, float _minDist)
{
	for (const auto& atk : skillThreePreAtk_) {
		//自分自身とは判定しない
		if (atk.cnt_ == _thisAtk.cnt_)
			continue;

		int dx = _thisAtk.pos_.x - atk.pos_.x;
		int dz = _thisAtk.pos_.z - atk.pos_.z;
		double distance = std::sqrt(dx * dx + dz * dz);
		if (distance < _minDist) {
			return true; // 重なっている場合
		}
	}
	return false; // 重なっていない場合
}