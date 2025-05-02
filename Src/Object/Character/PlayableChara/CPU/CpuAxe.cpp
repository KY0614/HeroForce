#include"../AxeMan.h"
#include "CpuAxe.h"

CpuAxe::CpuAxe(void)
{
}

CpuAxe::~CpuAxe(void)
{
}

void CpuAxe::Init(void)
{
	obj_ = new AxeMan();
	obj_->Init();
	SetInitPos(playerNum_);
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

void CpuAxe::AtkInput(void)
{
}

void CpuAxe::SkillOneInput(void)
{
}

void CpuAxe::SkillTwoInput(void)
{
}
