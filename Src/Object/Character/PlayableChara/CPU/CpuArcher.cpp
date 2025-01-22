#include"../Archer.h"
#include "CpuArcher.h"

void CpuArcher::Init(void)
{
	obj_ = new Archer();
	obj_->Init();
}

void CpuArcher::Update(void)
{
	obj_->Update();
}

void CpuArcher::Draw(void)
{
	obj_->Draw();
}

void CpuArcher::Release(void)
{
}
