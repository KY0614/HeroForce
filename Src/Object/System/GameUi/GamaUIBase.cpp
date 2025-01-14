#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/ResourceManager.h"
#include "GamaUIBase.h"

GamaUIBase::GamaUIBase()
{
	pos_ = AsoUtility::VECTOR_ZERO;
	hp_ = -1;
}

GamaUIBase::~GamaUIBase()
{
}

void GamaUIBase::Init()
{
	Load();

	SetParam();
}

void GamaUIBase::Update()
{
}

void GamaUIBase::Draw()
{
}

void GamaUIBase::SetParam()
{
}

void GamaUIBase::Load()
{
	imgHpGage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::HP_GAGE).handleId_;
}

void GamaUIBase::SetPos(const VECTOR pos)
{
	pos_ = pos;
}

void GamaUIBase::SetHP(const int hp)
{
	hp_ = hp;
}
