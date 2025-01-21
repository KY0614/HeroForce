#include"../AxeMan.h"
#include "PlAxeMan.h"


PlAxe::PlAxe(const SceneManager::CNTL _cntl)
{
}

PlAxe::PlAxe(const InputManager::JOYPAD_NO _padNum)
{
}

void PlAxe::Init(void)
{
	obj_ = new AxeMan();
	obj_->Init();
}

void PlAxe::Update(void)
{
	obj_->Update();
}

void PlAxe::Draw(void)
{
	obj_->Draw();
}

void PlAxe::Release(void)
{
	
}
