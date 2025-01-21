#include"../Application.h"
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"

UnitBase::UnitBase(void)
{
	trans_.modelId = -1;
	hp_ = -1;
	trans_.pos = { 0.0f,0.0f,0.0f };
	trans_.scl = { 0.0f,0.0f,0.0f };
	trans_.rot = { 0.0f,0.0f,0.0f };	

	for (auto& tran_ : transArray_)
	{
		tran_.pos = { 0.0f,0.0f,0.0f };
		tran_.scl = { 0.0f,0.0f,0.0f };
		tran_.rot = { 0.0f,0.0f,0.0f };
	}

	anim_ = ANIM::NONE;
	atcAnim_ = -1;
	animTotalTime_ = -1;
	stepAnim_ = -1.0f;
	speedAnim_ = 1.0f;
	prePos_ = AsoUtility::VECTOR_ZERO;

	defAtk_ = -1.0f;
	defDef_ = -1.0f;
	defSpeed_ = -1.0f;
	defHp_ = -1;

	atkUpPercent_ = -1.0f;
	defUpPercent_= -1.0f;
	speedUpPercent_ = -1.0f;
}

UnitBase::~UnitBase(void)
{
}

void UnitBase::Destroy(void)
{
}

void UnitBase::Init(void)
{
}

void UnitBase::Update(void)
{
}

void UnitBase::Draw(void)
{
}

const Transform& UnitBase::GetTransform(void) const
{
	return trans_;
}

const inline Transform* UnitBase::GetTransformEntity(void) const
{
	auto ret = new Transform();

	ret->pos = trans_.pos;
	ret->scl = trans_.scl;
	ret->rot = trans_.rot;
	ret->quaRot = trans_.quaRot;
	return ret;
	return ret;
}

const VECTOR UnitBase::GetPos(void) const
{
	return trans_.pos;
}

const VECTOR UnitBase::GetRot(void) const
{
	return trans_.rot;
}

const VECTOR UnitBase::GetScl(void) const
{
	return trans_.scl;
}

const float UnitBase::GetDef(void) const
{
	return def_;
}

const UnitBase::ATK UnitBase::GetAtk(void) const
{
	return atk_;
}

const float UnitBase::GetRadius(void) const
{
	return radius_;
}

const VECTOR UnitBase::GetPrePos() const
{
	return prePos_;
}

/// <summary>
/// アニメーション関数
/// </summary>
/// <param name="_num">再生するアニメーションナンバー</param>
void UnitBase::Anim(void)
{
	// アニメーション再生
	// 経過時間の取得
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	// アニメーション時間の進行
	stepAnim_ += (speedAnim_ * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		//アニメーション終了時処理（継承先で行動を決めておく）
		FinishAnim();
	}
	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(trans_.modelId, atcAnim_, stepAnim_);

	//for (auto& tran : transArray_)
	//{
	//	// 再生するアニメーション時間の設定
	//	MV1SetAttachAnimTime(tran.modelId, atcAnim_, stepAnim_);
	//}
}


/// <summary>
/// アニメーションセット
/// </summary>
/// <param name="_anim">アニメの指定</param>
/// <param name="_speed">アニメーションスピード</param>
void UnitBase::ResetAnim(const ANIM _anim, const float _speed)
{
	if (anim_ == _anim)return;

	//アニメーションスピードの変更
	speedAnim_ = _speed;

	//デタッチ
	//実質atcAnimの初期化
	MV1DetachAnim(trans_.modelId, atcAnim_);

	anim_ = _anim;
	//アタッチ
	//実質atcAnimの代入
	atcAnim_ = MV1AttachAnim(trans_.modelId, animNum_[anim_]);


	animTotalTime_ = MV1GetAttachAnimTotalTime(trans_.modelId, atcAnim_);
	stepAnim_ = 0.0f;

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(trans_.modelId, atcAnim_, stepAnim_);

	//for (auto& tran : transArray_)
	//{
	//	//デタッチ
	//	//実質atcAnimの初期化
	//	MV1DetachAnim(tran.modelId, atcAnim_);
	//	//アタッチ
	//	//実質atcAnimの代入
	//	atcAnim_ = MV1AttachAnim(tran.modelId, animNum_[anim_]);

	//	animTotalTime_ = MV1GetAttachAnimTotalTime(tran.modelId, atcAnim_);

	//	// 再生するアニメーション時間の設定
	//	MV1SetAttachAnimTime(tran.modelId, atcAnim_, stepAnim_);
	//}
}

void UnitBase::SetIsHit(const bool _flag)
{
	atk_.isHit_ = _flag;
}

void UnitBase::SetDamage(const int damage)
{
	//与えるダメージを増やす
	damage_ += damage;
}

void UnitBase::SubHp()
{
	//ダメージが0より大きいか調べる
	if(0 < damage_)
	{
		//残りダメージを減らす;
		damage_--;
		//Hpを減らす
		hp_--;
		//HP下限
		if (hp_ < 0) { hp_ = 0; }
	}
}

//座標の設定
void UnitBase::SetPos(const VECTOR pos)
{
	trans_.pos = pos;
}

void UnitBase::SetPrePos(void)
{
	trans_.pos = prePos_;
}

//攻撃力の強化
void UnitBase::SetAttack(const float percent)
{
	atkUpPercent_ += percent;			//強化％上昇
	atkPow_ = defAtk_ * atkUpPercent_;	//攻撃力を上昇
}
 
//防御力の強化
void UnitBase::SetDefense(const float percent)
{
	defUpPercent_ += percent;
	def_ = defDef_ * defUpPercent_;
}

//移動速度
void UnitBase::SetSpeed(const float percent)
{
	speedUpPercent_ += percent;
	moveSpeed_ = defSpeed_ * speedUpPercent_;
}

//体力強化
void UnitBase::SetHpMax(const float hp)
{
	hpMax_ += hp;
}

//アニメ終了時の動き
void UnitBase::FinishAnim(void)
{
	//ループ再生
	stepAnim_ = 0.0f;
}

void UnitBase::CntUp(float& _count)
{
	// 経過時間の取得
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	_count += deltaTime;
}

void UnitBase::CntDown(float& _count)
{
	// 経過時間の取得
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	_count -= deltaTime;
}