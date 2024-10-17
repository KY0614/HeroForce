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
void UnitBase::Anim(const ANIM _anim)
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
	MV1SetAttachAnimTime(mdlId_, animNum_[_anim], stepAnim_);
}

/// <summary>
/// アニメーションリセット
/// </summary>
/// <param name="_anim">セットするアニメーション</param>
/// <param name="_attachNum">アニメーションナンバー</param>
void UnitBase::ResetAnim(const ANIM _anim)
{
	

	//デタッチ
	MV1DetachAnim(mdlId_, animNum_[anim_]);

	anim_ = _anim;
	//アタッチ
	animNum_[anim_] = MV1AttachAnim(mdlId_, animNum_[anim_]);


	animTotalTime_ = MV1GetAttachAnimTotalTime(mdlId_, animNum_[anim_]);
	stepAnim_ = 0;
}



