#pragma once
#include "../PlayerBase.h"

class PlAxe :
    public PlayerBase
{
public:
    //クールタイム
    static constexpr float ATK_COOLTIME = 2.0f;
    static constexpr float SKILL_ONE_COOLTIME = 6.0f;
    static constexpr float SKILL_TWO_COOLTIME = 5.0f;

    //各攻撃の持続時間
    static constexpr float FRAME_ATK_DURATION = 0.5f;
    static constexpr float FRAME_SKILL1_DURATION = 0.5f;
    static constexpr float FRAME_SKILL2_DURATION = 1.5f;

    //後隙
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr float FRAME_SKILL1_BACKRASH = 0.2f;
    static constexpr float FRAME_SKILL2_BACKRASH = 0.2f;

    //各攻撃の座標
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //攻撃範囲
    static constexpr float COL_ATK = CHARACTER_SCALE * 100.0f;
    static constexpr float COL_SKILL1 = CHARACTER_SCALE * 150.0f;
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 200.0f;

    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//攻撃開始判定の大きさ

    PlAxe(SceneManager::PLAY_MODE _mode);
    ~PlAxe(void) = default;
    void SetParam(void)override;
protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
    void InitAct(void)override;

    //void ResetParam(ATK_ACT _act)override;
};

