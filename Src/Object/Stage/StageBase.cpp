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
	// ���f������̊�{���
	trans_.SetModel(
			ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::STAGE));

	// ���f���傫��
	float scale = 1.0;
	trans_.scl = { scale, scale, scale };

	// ���f���ʒu
	trans_.pos = { 0.0f, 0.0f, 0.0f };

	// ���f���p�x
	trans_.quaRot = Quaternion::Euler(
					0.0f,
					AsoUtility::Deg2RadF(180.0f),
					0.0f);
	trans_.quaRotLocal = Quaternion();

	// ���f���ݒ�
	trans_.Update();


	// �ŏ��̘f��
	//------------------------------------------------------------------------------
	//Transform planetTrans;
	//planetTrans.SetModel(
	//	resMng_.LoadModelDuplicate(ResourceManager::SRC::MAIN_PLANET));
	//planetTrans.scl = AsoUtility::VECTOR_ONE;
	//planetTrans.quaRot = Quaternion();
	//planetTrans.pos = { 0.0f, -100.0f, 0.0f };

	//// �����蔻��(�R���C�_)�쐬
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
