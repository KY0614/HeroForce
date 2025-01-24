#include "../../Application.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Utility/AsoUtility.h"
#include "StageManager.h"

StageManager::StageManager(void)
{
}

StageManager::~StageManager(void)
{
}

void StageManager::Destroy(void)
{
}

void StageManager::Init(void)
{
	//ƒ‚ƒfƒ‹‚Ì‰Šú‰»
	for (int i = 0; i < MODELS; i++) {
		ResourceManager::SRC src = ResourceManager::SRC::STAGE_01;
		if (i == static_cast<int>(MODEL_TYPE::DECO)) { src = ResourceManager::SRC::DECO_01; }
		trans_[i].SetModel(
			ResourceManager::GetInstance().
			LoadModelDuplicate(src));
		trans_[i].pos = { 0.0f, -100.0f, 0.0f };
		trans_[i].scl = SCALE;
		trans_[i].quaRot = Quaternion();
		trans_[i].quaRotLocal = Quaternion::Euler(
			0.0f,
			AsoUtility::Deg2RadF(180.0f),
			0.0f);	
		trans_[i].Update();
	}
}

void StageManager::Update(void)
{
}

void StageManager::Draw(void)
{
	for (int i = 0; i < MODELS; i++) {
		MV1DrawModel(trans_[i].modelId);
	}
}

void StageManager::DebugDraw()
{
}