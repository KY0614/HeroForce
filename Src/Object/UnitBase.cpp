#include"../Application.h"
#include "../Lib/nlohmann/json.hpp"
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"

UnitBase::UnitBase(void)
{
	trans_.modelId = -1;
	hp_ = -1;
	trans_.pos = { 0.0f,0.0f,0.0f };
	trans_.scl = { 0.0f,0.0f,0.0f };
	trans_.rot = { 0.0f,0.0f,0.0f };	

	for (int i = 0; i < ARRAY_NUM;i++)
	{
		transArray_[i].pos = {0.0f,0.0f,0.0f};
		transArray_[i].scl = { 0.0f,0.0f,0.0f };
		transArray_[i].rot = { 0.0f,0.0f,0.0f };

		animArray_[i] = -1;
		animArrayTotalTime_[i] = -1;
		stepAnimArray_[i] = -1.0f;
		speedAnimArray_[i] = -1.0f;
	}

	anim_ = ANIM::NONE;
	atcAnim_ = -1;
	atkPow_ = -1.0f;
	damage_ = -1;
	animTotalTime_ = -1;
	stepAnim_ = -1.0f;
	speedAnim_ = 1.0f;

	prePos_ = AsoUtility::VECTOR_ZERO;
	def_ = -1.0f;
	radius_ = -1.0f;
	hpMax_ = -1;

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
	if (stepAnim_ > animTotalTime_ )
	{
		//アニメーション終了時処理（継承先で行動を決めておく）
		FinishAnim();
	}
	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(trans_.modelId, atcAnim_, stepAnim_);
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
}

void UnitBase::AnimArray(int i)
{
	// アニメーション再生
	// 経過時間の取得
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	stepAnimArray_[i] += (speedAnimArray_[i] * deltaTime);
	if (stepAnimArray_[i] > animArrayTotalTime_[i])
	{
		//アニメーション終了時処理（継承先で行動を決めておく）
		FinishAnimArray(i);
	}

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(transArray_[i].modelId, animArray_[i], stepAnimArray_[i]);

}

void UnitBase::SetIsHit(const bool _flag)
{
	atk_.isHit_ = _flag;
}

void UnitBase::SetDamage(const int attackerPower, const int skillPower)
{
	//与えるダメージを増やす
	damage_ += attackerPower * skillPower / defDef_;
}

void UnitBase::SubHp()
{
	//ダメージが0より大きいか調べる
	if(0 < damage_)
	{
		damage_--;

		hp_--;

		if (hp_ < 0) { hp_ = 0; }
	}
}

void UnitBase::ResetAnimArray(const ANIM _anim, const float _speed, int i)
{
	if (animStateArray_[i] == _anim)return;

	speedAnimArray_[i] = _speed;

	//デタッチ
	//実質atcAnimの初期化
	MV1DetachAnim(transArray_[i].modelId, animArray_[i]);

	animStateArray_[i] = _anim;

	//アタッチ
	//実質atcAnimの代入
	animArray_[i] = MV1AttachAnim(transArray_[i].modelId, animNumArray_[i][animStateArray_[i]]);

	animArrayTotalTime_[i] = MV1GetAttachAnimTotalTime(transArray_[i].modelId, animArray_[i]);
	stepAnimArray_[i] = 0.0f;

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(transArray_[i].modelId, animArray_[i], stepAnimArray_[i]);
}


float UnitBase::GetAnimArrayTime(int i)
{
	//float ret = MV1GetAttachAnimTime(transArray_[i].modelId, animArray_[i]);
	float ret = static_cast<float>(animArrayTotalTime_[i]);
	return ret;
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


void UnitBase::ParamLoad()
{
	//各キャラクターのパラメータのJSON読み込み

}

//アニメ終了時の動き
void UnitBase::FinishAnim(void)
{
	//ループ再生
	stepAnim_ = 0.0f;
}

void UnitBase::FinishAnimArray(int i)
{
	stepAnimArray_[i] = 0.0f;
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