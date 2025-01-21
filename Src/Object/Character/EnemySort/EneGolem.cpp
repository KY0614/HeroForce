#include"Enemy.h"
#include "EneGolem.h"

EneGolem::EneGolem(const VECTOR& _spawnPos) : Enemy(_spawnPos)
{
	trans_.pos = _spawnPos;
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
	radius_ = MY_COL_RADIUS;
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	hp_ = HP_MAX;
	atkPow_ = ATK_POW;
	def_ = DEF;
	exp_ = EXP;
	walkSpeed_ = WALK_SPEED;
	localCenterPos_ = LOCAL_CENTER_POS;
	stunDefMax_ = STUN_DEF_MAX;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
	skillThreeCnt_ = 0;
	skillThreeDelayCnt_ = 0.0f;
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
	animNum_.emplace(ANIM::UNIQUE_3, ANIM_PRE_PUNCH);
	animNum_.emplace(ANIM::UNIQUE_4, ANIM_PRE_MOWDOWN);
	animNum_.emplace(ANIM::UNIQUE_5, ANIM_PRE_SHOUT);

	//アニメーション速度設定
	changeSpeedAnim_.emplace(ANIM::IDLE, SPEED_ANIM_IDLE);
	changeSpeedAnim_.emplace(ANIM::WALK, SPEED_ANIM_WALK);
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

void EneGolem::AlertSkill_One(void)
{
}

void EneGolem::AlertSkill_Two(void)
{
}

void EneGolem::AlertSkill_Three(void)
{
}

void EneGolem::Skill_One(void)
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
	//TODO:最初を修正

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
		//間隔カウンタの初期化
		skillThreeDelayCnt_ = 0.0f;

		//攻撃生成できたかの判定
		bool isGenelateAttack = false;

		//カウンタ増加
		skillThreeCnt_++;

		//スキル生成
		ATK& thisAtk = createSkill_();

		//最後に生成された攻撃を格納
		lastAtk_ = &thisAtk;

		//生成できるまで繰り返す
		while (!isGenelateAttack)
		{
			//円範囲の中の一点を取る
			GetRandomPointInCircle(trans_.pos, SKILL_THREE_FALL_RADIUS, thisAtk.pos_);

			if (IsOverlap(thisAtk.pos_, SKILL_THREE_COL_RADIUS * 2))
			{
				//生成完了
				isGenelateAttack = true;
			}
		}
	}

	//間隔カウンタ
	CntUp(skillThreeDelayCnt_);
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
	//更新処理の中身初期化
	stateUpdate_ = std::bind(&EneGolem::UpdateAtk, this);

	//最初に攻撃を生成するか
	if (atkAct_ != ATK_ACT::SKILL_THREE)
		lastAtk_ = &createSkill_();
}

void EneGolem::GetRandomPointInCircle(const VECTOR _myPos, const int _r, VECTOR& _tPos)
{
	// ランダムな角度theta (0 ～ 2π)
	float theta = static_cast<float>(rand()) / RAND_MAX * 2 * DX_PI_F;

	// ランダムな半径r' (0 ～ r) で、均等に分布するように sqrt を使う
	float radius = sqrt(static_cast<float>(rand()) / RAND_MAX) * _r;

	// 円内の点を計算
	_tPos.x = static_cast<int>(_myPos.x + radius * cos(theta));
	_tPos.z = static_cast<int>(_myPos.z + radius * sin(theta));
}

bool EneGolem::IsOverlap(VECTOR _tPos, float _minDist)
{
	for (const auto& atk : nowSkill_) {
		int dx = _tPos.x - atk.pos_.x;
		int dz = _tPos.z - atk.pos_.z;
		double distance = std::sqrt(dx * dx + dz * dz);
		if (distance < _minDist) {
			return true; // 重なっている場合
		}
	}
	return false; // 重なっていない場合
}