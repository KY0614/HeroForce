#include"../Knight.h"
#include "PlKnight.h"

PlKnight::PlKnight(const SceneManager::CNTL _cntl)
{
}

PlKnight::PlKnight(const InputManager::JOYPAD_NO _padNum)
{
}

void PlKnight::Init(void)
{
	obj_ = new Knight();
	obj_->Init();
}

void PlKnight::Update(void)
{
	obj_->Update();
}

void PlKnight::Draw(void)
{
	obj_->Draw();
}

void PlKnight::Release(void)
{
}
