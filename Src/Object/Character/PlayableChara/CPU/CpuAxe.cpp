#include"../AxeMan.h"
#include "CpuAxe.h"

void CpuAxe::Init(void)
{
	obj_ = new AxeMan();
	obj_->Init();
}

void CpuAxe::Update(void)
{
	obj_->Update();
}

void CpuAxe::Draw(void)
{
	obj_->Draw();
}

void CpuAxe::Release(void)
{
}
