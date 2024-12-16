#include "PlMage.h"

PlMage::PlMage(const SceneManager::PLAY_MODE _mode, const SceneManager::CNTL _cntl) :PlayerBase(_mode, _cntl)
{
	mode_ = _mode;
	cntl_ = _cntl;
}

PlMage::PlMage(const SceneManager::PLAY_MODE _mode, const InputManager::JOYPAD_NO _padNum) :PlayerBase(_mode, _padNum)
{
	mode_ = _mode;
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
