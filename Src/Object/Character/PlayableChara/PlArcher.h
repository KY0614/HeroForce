#pragma once
#include "../PlayerBase.h"
class PlArcher :
    public PlayerBase
{
public:
    //クールタイム
    static constexpr float ATK_COOLTIME = 2.0f;
    static constexpr float SKILL_ONE_COOLTIME = 6.0f;
    static constexpr float SKILL_TWO_COOLTIME = 5.0f;


    PlArcher(SceneManager::PLAY_MODE _mode);
    ~PlArcher(void) = default;
    void SetParam(void)override;

protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
};

