#include "PlArcher.h"
PlArcher::PlArcher(SceneManager::PLAY_MODE _mode, InputManager::JOYPAD_NO _padNum) :PlayerBase(_mode, _padNum)
{
	mode_ = _mode;
	padNum_ = _padNum;
}

void PlArcher::SetParam(void)
{
}

void PlArcher::AtkFunc(void)
{
}

void PlArcher::Skill1Func(void)
{
}

void PlArcher::Skill2Func(void)
{
}
