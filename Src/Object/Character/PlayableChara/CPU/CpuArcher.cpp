#include"../Archer.h"
#include "CpuArcher.h"

CpuArcher::CpuArcher(void)
{
}

CpuArcher::~CpuArcher(void)
{
}

void CpuArcher::Init(void)
{
	obj_ = new Archer();
	obj_->Init();
	SetInitPos(playerNum_);
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

void CpuArcher::AtkInput(void)
{
}

void CpuArcher::SkillOneInput(void)
{
}

void CpuArcher::SkillTwoInput(void)
{
}
