#pragma once
#include "PlayerBase.h"
class Mage :
    public PlayerBase
{
public:
    //ステータス
    static constexpr float POW_ATK = 100.0f;
    static constexpr int DEF_MAX = 120;
    static constexpr int HP_MAX = 270;
    static constexpr float SPEED = PlayerBase::MOVE_SPEED_NORMAL;
    Mage(void);
    ~Mage(void) = default;
    void SetParam(void)override;

protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;

    //攻撃入力
    void NmlAtkInit(void)override;
    //スキル入力
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;
};

