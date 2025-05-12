#include"../Mage.h"
#include "CpuMage.h"

CpuMage::CpuMage(void)
{
}

CpuMage::~CpuMage(void)
{
}

void CpuMage::Init(void)
{
	obj_ = new Mage();
	obj_->Init();
	SetInitPos(playerNum_);
}

void CpuMage::Update(void)
{
	obj_->Update();
}

void CpuMage::Draw(void)
{
	obj_->Draw();
}

void CpuMage::Release(void)
{
}

void CpuMage::AtkInput(void)
{
}

void CpuMage::SkillOneInput(void)
{
}

void CpuMage::SkillTwoInput(void)
{
}
