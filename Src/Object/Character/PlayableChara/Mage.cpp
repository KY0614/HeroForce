#include "Mage.h"

Mage::Mage(const SceneManager::CNTL _cntl) :PlayerBase(_cntl)
{
	cntl_ = _cntl;
}

Mage::Mage(const InputManager::JOYPAD_NO _padNum) : PlayerBase(_padNum)
{
	padNum_ = _padNum;
}

void Mage::SetParam(void)
{
}

void Mage::AtkFunc(void)
{
}

void Mage::Skill1Func(void)
{
}

void Mage::Skill2Func(void)
{
}