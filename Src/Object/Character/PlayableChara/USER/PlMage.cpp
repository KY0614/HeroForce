#include"../../../../Manager/Generic/SceneManager.h"
#include"../../../../Manager/Generic/InputManager.h"
#include"../Mage.h"
#include "PlMage.h"

PlMage::PlMage(const SceneManager::CNTL _cntl)
{

}

PlMage::PlMage(const InputManager::JOYPAD_NO _padNum)
{

}

void PlMage::Init(void)
{
	obj_ = new Mage();
	obj_->Init();
}

void PlMage::Update(void)
{
	obj_->Update();
}

void PlMage::Draw(void)
{
	obj_->Draw();
}

void PlMage::Release(void)
{
}
