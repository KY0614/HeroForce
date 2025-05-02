#pragma once
#include "PlayerBase.h"
class AxeMan :
    public PlayerBase
{
public:
#ifdef DEBUG_ON
    void DrawDebug(void)override;
#endif // DEBUG_ON

#ifdef DEBUG_COOL
    //クールタイム
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 1.0f;
    static constexpr float SKILL_TWO_COOLTIME = 1.0f;
#else // DEBUG_COOL
    //クールタイム
    static constexpr float ATK_COOLTIME = 1.5f;
    static constexpr float SKILL_ONE_COOLTIME = 6.0f;
    static constexpr float SKILL_TWO_COOLTIME = 3.5f;
#endif // DEBUG_COOL

    //ステータス
    static constexpr float POW_ATK = 160.0f;
    static constexpr int DEF_MAX = 110;
    static constexpr int HP_MAX = 265;
    static constexpr float SPEED = PlayerBase::MOVE_SPEED_SLOW;

    //攻撃発生の時間
    static constexpr float ATK_START = 0.2f;
    static constexpr float SKILL_ONE_START = 10.0f;
    static constexpr float SKILL_TWO_START = 0.2f;

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
    static constexpr float NMLATK_POW = 12.0f;
    static constexpr float SKILL_ONE_POW = 25.0f;
    static constexpr float SKILL_ONE_CHARGE_POW = 50.0f;
    static constexpr float SKILL_TWO_POW = 8.0f;

    //溜めモーション
    static constexpr float SKILL_CHARGE_STEPANIM = 16.9f;

    //エフェクトサイズ
    //----------------------------------------------------
    //スキル２攻撃の瞬間
    static constexpr float CHARGE_AXE_EFF_SIZE = 20.0f;

    //スキルチャージ
    static constexpr float CHARGE_SKILL_EFF_SIZE = 50.0f;



    //通常攻撃の最大値
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS,COL_ATK,NMLATK_POW,FRAME_ATK_DURATION,FRAME_ATK_BACKRASH,0.0f,false };

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

    void Draw(void)override;

    //固有アニメーション番号の初期化
    void InitCharaAnim(void)override;

    //void ChargeAct(void)override;
    //攻撃入力
    void NmlAtkInit(void)override;
    //スキル入力
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;
    //void ResetParam(ATK_ACT _act)override;
};

