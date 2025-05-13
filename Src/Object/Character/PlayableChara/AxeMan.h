#pragma once
#include "PlayerBase.h"
class AxeMan :
    public PlayerBase
{
public:
#ifdef DEBUG_ON
    void DrawDebug(void)override;
#endif // DEBUG_ON

    //通常攻撃
    //-------------------------------------------------
    //クールタイム
    static constexpr float NORMAL_ATK_COOLTIME = 1.5f;
    //前隙
    static constexpr float NORMAL_ATK_START = 0.2f;
    //攻撃時間
    static constexpr float NORMAL_ATK_DURATION = 0.5f - NORMAL_ATK_START;
    //後隙
    static constexpr float NORMAL_ATK_BACKRASH = 0.1f;
    //相対座標
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    //威力
    static constexpr float NORMAL_ATK_POW = 12.0f;
    //当たり判定
    static constexpr float NORMAL_ATK_COL = CHARACTER_SCALE * 100.0f;
    //-------------------------------------------------
    //スキル１
    //-------------------------------------------------
    //クールタイム
    static constexpr float SKILL_ONE_COOLTIME = 6.0f;
    //前隙
    static constexpr float SKILL_ONE_START = 10.0f;
    //持続時間
    static constexpr float SKILL_ONE_DURATION = 0.7f;
    //後隙
    static constexpr float SKILL_ONE_BACKRASH = 0.2f;
    //相対座標
    static constexpr VECTOR SKILL_ONE_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    //当たり判定
    static constexpr float SKILL_ONE_COL = CHARACTER_SCALE * 150.0f;

    //溜めてない時の威力
    static constexpr float SKILL_ONE_POW = 25.0f;
    //溜めたときの威力
    static constexpr float SKILL_ONE_CHARGE_POW = 50.0f;
    //チャージアニメーション
    static constexpr float SKILL_ONE_CHARGE_STEPANIM = 16.9f;
    //-------------------------------------------------
    //スキル２
    //-------------------------------------------------
    //クールタイム
    static constexpr float SKILL_TWO_COOLTIME = 3.5f;
    //前隙
    static constexpr float SKILL_TWO_START = 0.2f;
    //攻撃時間
    static constexpr float SKILL_TWO_DURATION = 3.0f - SKILL_TWO_START;
    //後隙
    static constexpr float SKILL_TWO_BACKRASH = 0.2f;
    //相対座標
    static constexpr VECTOR SKILL_TWO_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };
    //当たり判定の大きさ
    static constexpr float SKILL_TWO_COL = CHARACTER_SCALE * 200.0f;
    //威力
    static constexpr float SKILL_TWO_POW = 8.0f;

    //エフェクトサイズ
    static constexpr float CHARGE_AXE_EFF_SIZE = 20.0f;

    //アニメーション変更フレーム
    static constexpr float SKILL_TWO_CHANGE_ANIM_STEP = 14.0f;
    //アニメーションチェンジするまでのカウント
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.25f;
    //スキルチャージ
    static constexpr float CHARGE_SKILL_EFF_SIZE = 50.0f;
 
    



    //通常攻撃の最大値
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS,NORMAL_ATK_COL,NORMAL_ATK_POW,NORMAL_ATK_DURATION,NORMAL_ATK_BACKRASH,0.0f,false };

    //スキル１
    static constexpr ATK SKILL_ONE_MAX{ SKILL_ONE_COL_LOCAL_POS ,SKILL_ONE_COL ,SKILL_ONE_POW,SKILL_ONE_DURATION ,SKILL_ONE_BACKRASH ,0.0f,false };

    //スキル２
    static constexpr ATK SKILL_TWO_MAX{ SKILL_TWO_COL_LOCAL_POS ,SKILL_TWO_COL ,SKILL_TWO_POW,SKILL_TWO_DURATION ,SKILL_TWO_BACKRASH ,0.0f,false };




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

    //攻撃入力
    void NmlAtkInit(void)override;
    //スキル入力
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;
};

