#include "PlMage.h"

PlMage::PlMage(const SceneManager::CNTL _cntl) :PlayerBase(_cntl)
{
	cntl_ = _cntl;
}

PlMage::PlMage(const InputManager::JOYPAD_NO _padNum) :PlayerBase(_padNum)
{
	padNum_ = _padNum;
}

void PlMage::SetParam(void)
{
}

void PlMage::AtkFunc(void)
{
}

void PlMage::Skill1Func(void)
{
}

void PlMage::Skill2Func(void)
{
}
