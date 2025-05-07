#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "GamaUIBase.h"

GamaUIBase::GamaUIBase()
{
	imgHpBar_ = 0;
	imgHpGage_ = 0;
	typeHpBar_ = HPBAR_TYPE::MAX;
	pos_ = AsoUtility::VECTOR_ZERO;
	hp_ = -1;
}

void GamaUIBase::Init()
{
	Load();
}

void GamaUIBase::Draw()
{
}

void GamaUIBase::Load()
{
	imgHpGage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::HP_GAGE).handleId_;
}