#pragma once
#include "PlayerBase.h"
class AxeMan :
    public PlayerBase
{
public:

#ifdef DEBUG_COOL
    //クールタイム
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 1.0f;
    static constexpr float SKILL_TWO_COOLTIME = 1.0f;
#else // DEBUG_COOL
    //クールタイム
    static constexpr float ATK_COOLTIME = 2.0f;
    static constexpr float SKILL_ONE_COOLTIME = 10.0f;
    static constexpr float SKILL_TWO_COOLTIME = 5.0f;
#endif // DEBUG_COOL


    //攻撃発生の時間
    static constexpr float ATK_START = 0.2f;
    static constexpr float SKILL_ONE_START = 10.0f;
    static constexpr float SKILL_TWO_START = 0.2f;

    //スキル１の長押ししていなかった時の攻撃発生
    static constexpr float SKILL_ONE_START_NOCHARGE = 3.5f;

    //各攻撃の持続時間
    static constexpr float FRAME_ATK_DURATION = 0.5f - ATK_START;
    static constexpr float FRAME_SKILL1_DURATION = 0.7f;
    static constexpr float FRAME_SKILL2_DURATION = 3.0f - SKILL_TWO_START;
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.25f;

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

    //攻撃威力
    static constexpr float ATK_POW = 7.0f;
    static constexpr float SKILL_ONE_POW = 50.0f;
    static constexpr float SKILL_TWO_POW = 5.0f;

    //溜めモーション
    static constexpr float SKILL_CHARGE_STEPANIM = 16.9f;



    //通常攻撃の最大値
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS,COL_ATK,ATK_POW,FRAME_ATK_DURATION,FRAME_ATK_BACKRASH,0.0f,false };

    //スキル１
    static constexpr ATK SKILL_ONE_MAX{ SKILL1_COL_LOCAL_POS ,COL_SKILL1 ,SKILL_ONE_POW,FRAME_SKILL1_DURATION ,FRAME_SKILL1_BACKRASH ,0.0f,false };

    //スキル２
    static constexpr ATK SKILL_TWO_MAX{ SKILL2_COL_LOCAL_POS ,COL_SKILL2 ,SKILL_TWO_POW,FRAME_SKILL2_DURATION ,FRAME_SKILL2_BACKRASH ,0.0f,false };




    //固有アニメーション
    static constexpr int ATK_NUM = 9;
    static constexpr int SKILL_ONE_NUM = 8;
    static constexpr int SKILL_TWO_NUM = 9;
    static constexpr int SPIN_NUM = 11;

    //スキル２の回転攻撃の多段ヒットインターバル
    static constexpr float MULTIHIT_INTERVAL = 0.5f;
    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//攻撃開始判定の大きさ

    AxeMan(void);
    ~AxeMan(void) = default;
    void SetParam(void)override;
protected:

    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
    void InitAct(void)override;
    void InitSkill(ATK_ACT _act);

    //固有アニメーション番号の初期化
    void InitCharaAnim(void)override;

    //void ChargeAct(void)override;

    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;
    //void ResetParam(ATK_ACT _act)override;
};

