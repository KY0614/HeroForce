#include"../Knight.h"
#include "CpuKnight.h"

CpuKnight::CpuKnight(void)
{
}

CpuKnight::~CpuKnight(void)
{
}

void CpuKnight::Init(void)
{
	obj_ = new Knight();
	obj_->Init();
	SetInitPos(playerNum_);
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

void CpuKnight::AtkInput(void)
{
}

void CpuKnight::SkillOneInput(void)
{
}

void CpuKnight::SkillTwoInput(void)
{
}
