#pragma once
#include"../Manager/SceneManager.h"
#include "../PlayerBase.h"
class PlKnight:
    public PlayerBase
{
public:
    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//UŒ‚ŠJn”»’è‚Ì‘å‚«‚³
    PlKnight(SceneManager::PLAY_MODE _mode);
    ~PlKnight(void) = default;
    void SetParam(void)override;

protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
};