#include "../../Common/Quaternion.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Utility/AsoUtility.h"
#include "SkyDome.h"

SkyDome::SkyDome()
{
	state_ = STATE::NONE;
}

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
	Quaternion yRot = Quaternion::AngleAxis(AsoUtility::Deg2RadF(ROT_SPEED), AsoUtility::AXIS_Y);
	transform_.quaRot = Quaternion::Mult(transform_.quaRot, yRot);
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

const Transform& SkyDome::GetTransform() const
{
	return transform_;
}
