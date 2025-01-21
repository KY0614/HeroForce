#include"../Archer.h"
#include "PlArcher.h"

PlArcher::PlArcher(const SceneManager::CNTL _cntl)
{
}

PlArcher::PlArcher(const InputManager::JOYPAD_NO _padNum)
{
}

void PlArcher::Init(void)
{
	obj_ = new Archer();
	obj_->Init();
}

void PlArcher::Update(void)
{
	obj_->Update();
}

void PlArcher::Draw(void)
{
	obj_->Draw();
}

void PlArcher::Release(void)
{
}
