#include"../Manager/SceneManager.h"
#include "UnitBase.h"

UnitBase::UnitBase(void)
{
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
	return VECTOR();
}

const VECTOR UnitBase::GetRot(void) const
{
	return VECTOR();
}

const VECTOR UnitBase::GetScl(void) const
{
	return VECTOR();
}

const float UnitBase::GetDef(void) const
{
	return 0.0f;
}

/// <summary>
/// アニメーション関数
/// </summary>
/// <param name="_num">再生するアニメーションナンバー</param>
void UnitBase::Anim(const int _num)
{
	// アニメーション再生
// 経過時間の取得
	float deltaTime = 1.0f / SceneManager::DEFAULT_FPS;
	// アニメーション時間の進行
	stepAnim_ += (SPEED_ANIM * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		// ループ再生
		stepAnim_ = 0.0f;
	}
	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(mdlId_, _num, stepAnim_);
}

/// <summary>
/// アニメリセット
/// </summary>
/// <param name="_totalAnim">アニメ総再生時間</param>
void UnitBase::ResetAnim(const int _totalAnim)
{
	animTotalTime_ = _totalAnim;
	stepAnim_ = 0;
}

