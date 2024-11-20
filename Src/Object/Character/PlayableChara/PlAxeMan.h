#pragma once
#include "../PlayerBase.h"

class PlAxe :
    public PlayerBase
{
public:
    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//UŒ‚ŠJn”»’è‚Ì‘å‚«‚³

    PlAxe(SceneManager::PLAY_MODE _mode);
    ~PlAxe(void) = default;
    void SetParam(void)override;
protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
};

