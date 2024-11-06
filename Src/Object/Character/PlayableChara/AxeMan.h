#pragma once
#include "../PlayerBase.h"
class AxeMan :
    public PlayerBase
{
public:
    AxeMan(PLAY_MODE _mode);
    ~AxeMan(void) = default;
    void SetParam(void)override;
protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
};

