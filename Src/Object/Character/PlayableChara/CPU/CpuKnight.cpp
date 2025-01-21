#include"../Knight.h"
#include "CpuKnight.h"

void CpuKnight::Init(void)
{
	obj_ = new Knight();
	obj_->Init();
}

void CpuKnight::Update(void)
{
	obj_->Update();
}

void CpuKnight::Draw(void)
{
	obj_->Draw();
}

void CpuKnight::Release(void)
{
}
