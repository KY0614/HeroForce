#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"
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
	//通常攻撃
	//---------------------------------------------------------



	//---------------------------------------------------------
	//スキル1
	//---------------------------------------------------------




	//---------------------------------------------------------
	//スキル2
	//---------------------------------------------------------





	//---------------------------------------------------------
}

void PlMage::Draw(void)
{
	obj_->Draw();
}

void PlMage::Release(void)
{
}

void PlMage::AtkInput(void)
{
}

void PlMage::SkillOneInput(void)
{
}

void PlMage::SkillTwoInput(void)
{
}
