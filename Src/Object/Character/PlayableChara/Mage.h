#pragma once
#include "../PlayerBase.h"
class Mage :
    public PlayerBase
{
public:
    Mage(const SceneManager::CNTL _cntl);
    Mage(const InputManager::JOYPAD_NO _padNum);
    ~Mage(void) = default;
    void SetParam(void)override;

protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
};

