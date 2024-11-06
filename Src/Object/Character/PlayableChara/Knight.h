#pragma once
#include "../PlayerBase.h"
class Knight :
    public PlayerBase
{
public:
    Knight(PLAY_MODE _mode);
    ~Knight(void) = default;
    void SetParam(void)override;

protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
};