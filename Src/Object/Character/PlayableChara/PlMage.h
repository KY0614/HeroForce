#pragma once
#include "../PlayerBase.h"
class PlMage :
    public PlayerBase
{
public:
    PlMage(SceneManager::PLAY_MODE _mode, InputManager::JOYPAD_NO _padNum);
    ~PlMage(void) = default;
    void SetParam(void)override;

protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
};
