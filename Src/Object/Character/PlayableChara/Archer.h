#pragma once
#include "../PlayerBase.h"
class Archer :
    public PlayerBase
{
public:
    Archer(PLAY_MODE _mode);
    ~Archer(void) = default;
    void SetParam(void)override;

protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
};

