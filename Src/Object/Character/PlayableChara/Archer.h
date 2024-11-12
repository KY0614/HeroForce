#pragma once
#include "../PlayerBase.h"
class PlArcher :
    public PlayerBase
{
public:
    PlArcher(SceneManager::PLAY_MODE _mode);
    ~PlArcher(void) = default;
    void SetParam(void)override;

protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
};

