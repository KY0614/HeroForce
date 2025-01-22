#include "PlMage.h"

PlMage::PlMage(SceneManager::PLAY_MODE _mode, InputManager::JOYPAD_NO _padNum) :PlayerBase(_mode, _padNum)
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
