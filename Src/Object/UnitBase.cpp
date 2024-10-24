#include"../Manager/SceneManager.h"
#include "UnitBase.h"

UnitBase::UnitBase(void)
{
	trans_.modelId = -1;
	hp_ = -1;
	trans_.pos = { 0.0f,0.0f,0.0f };
	trans_.scl = { 0.0f,0.0f,0.0f };
	trans_.rot = { 0.0f,0.0f,0.0f };
	anim_ = ANIM::IDLE;
	atcAnim_ = -1;
	animTotalTime_ = -1;
	stepAnim_ = -1.0f;
	speedAnim_ = 1.0f;
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

/// <summary>
/// アニメーション関数
/// </summary>
/// <param name="_num">再生するアニメーションナンバー</param>
void UnitBase::Anim(void)
{
	// アニメーション再生
// 経過時間の取得
	float deltaTime = 1.0f / SceneManager::DEFAULT_FPS;
	// アニメーション時間の進行
	stepAnim_ += (speedAnim_ * deltaTime);
	if (stepAnim_ > animTotalTime_)
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

//アニメ終了時の動き
void UnitBase::FinishAnim(void)
{
	//ループ再生
	stepAnim_ = 0.0f;
}



