#include "../../Common/Quaternion.h"
#include "../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"
#include "SkyDome.h"

SkyDome::SkyDome()
{
}

//SkyDome::SkyDome(const Transform& syncTransform) : syncTransform_(syncTransform)
//{
//	state_ = STATE::NONE;
//
//	// 状態管理
//	stateChanges_.emplace(STATE::NONE, std::bind(&SkyDome::ChangeStateNone, this));
//	stateChanges_.emplace(STATE::STAY, std::bind(&SkyDome::ChangeStateStay, this));
//	stateChanges_.emplace(STATE::FOLLOW, std::bind(&SkyDome::ChangeStateFollow, this));
//
//}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Init(void)
{
	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SKY_DOME));
	transform_.scl = SCALES;
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	//Zバッファを無効
	MV1SetUseZBuffer(transform_.modelId, false);
	MV1SetWriteZBuffer(transform_.modelId, false);
}

void SkyDome::Update(void)
{
	//transform_.pos = follow_.pos;
	transform_.Update();
}

void SkyDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void SkyDome::Release(void)
{
	MV1DeleteModel(transform_.modelId);
}

//void SkyDome::ChangeState(STATE state)
//{
//
//	// 状態変更
//	state_ = state;
//
//	// 各状態遷移の初期処理
//	stateChanges_[state_]();
//
//}
//
//void SkyDome::ChangeStateNone(void)
//{
//	stateUpdate_ = std::bind(&SkyDome::UpdateNone, this);
//}
//
//void SkyDome::ChangeStateStay(void)
//{
//	stateUpdate_ = std::bind(&SkyDome::UpdateStay, this);
//}
//
//void SkyDome::ChangeStateFollow(void)
//{
//	stateUpdate_ = std::bind(&SkyDome::UpdateFollow, this);
//	transform_.pos = syncTransform_.pos;
//	transform_.Update();
//}
//
//void SkyDome::UpdateNone(void)
//{
//}
//
//void SkyDome::UpdateStay(void)
//{
//}
//
//void SkyDome::UpdateFollow(void)
//{
//	transform_.pos = syncTransform_.pos;
//	transform_.Update();
//}


const Transform& SkyDome::GetTransform() const
{
	return transform_;
}
