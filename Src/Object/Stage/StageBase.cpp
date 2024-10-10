#include "../../Manager/ResourceManager.h"
#include "../../Manager/InputManager.h"
#include "../../Utility/AsoUtility.h"
#include "StageBase.h"

StageBase::StageBase(void)
{
}

StageBase::~StageBase(void)
{
}

void StageBase::Destroy(void)
{
}

void StageBase::Init(void)
{
	// モデル制御の基本情報
	trans_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::STAGE));

	// モデル大きさ
	float scale = 1.0;
	trans_.scl = { scale, scale, scale };

	// モデル位置
	trans_.pos = { 0.0f, 0.0f, 0.0f };

	// モデル角度
	trans_.quaRot = Quaternion::Euler(
					0.0f,
					AsoUtility::Deg2RadF(180.0f),
					0.0f);
	trans_.quaRotLocal = Quaternion();

	// モデル設定
	trans_.Update();


	// 最初の惑星
	//------------------------------------------------------------------------------
	//Transform planetTrans;
	//planetTrans.SetModel(
	//	resMng_.LoadModelDuplicate(ResourceManager::SRC::MAIN_PLANET));
	//planetTrans.scl = AsoUtility::VECTOR_ONE;
	//planetTrans.quaRot = Quaternion();
	//planetTrans.pos = { 0.0f, -100.0f, 0.0f };

	//// 当たり判定(コライダ)作成
	//planetTrans.MakeCollider(Collider::TYPE::STAGE);

	//planetTrans.Update();

	//NAME name = NAME::MAIN_PLANET;
	//std::shared_ptr<Planet> planet =
	//	std::make_shared<Planet>(
	//		name, Planet::TYPE::GROUND, planetTrans);
	//planet->Init();
	//planets_.emplace(name, std::move(planet));
	////------------------------------------------------------------------------------
}

void StageBase::Update(void)
{
}

void StageBase::Draw(void)
{
	MV1DrawModel(trans_.modelId);
}
