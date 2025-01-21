#include"../Mage.h"
#include "CpuMage.h"

void CpuMage::Init(void)
{
	obj_ = new Mage();
	obj_->Init();
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
