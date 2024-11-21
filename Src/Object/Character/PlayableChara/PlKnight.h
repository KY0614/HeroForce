#pragma once
#include "../PlayerBase.h"
class PlKnight :
    public PlayerBase
{
public:
    PlKnight(SceneManager::PLAY_MODE _mode);
    ~PlKnight(void) = default;
    void SetParam(void)override;

protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
};