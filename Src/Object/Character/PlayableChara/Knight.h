#pragma once
#include "PlayerBase.h"
class Arrow;
class Knight :
    public PlayerBase
{
public:
    //ステータス
    static constexpr float POW_ATK = 120.0f;
    static constexpr float MAX_DEF = 150.0f;
    static constexpr float MAX_HP = 250;
    static constexpr float SPEED = PlayerBase::MOVE_SPEED_NORMAL;

    //攻撃開始判定の大きさ
    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	
    //スキル使用開始時にクールタイムを増やす時間

    static constexpr float SKILL_TWO_START_COOLTIME = 3.0f;

    //ガード可能になる残りクールタイム(ガード時間最低1秒確保したいから)
    static constexpr float GUARD_STARTABLE_COOL = 4.0f;

    //ガードエフェクトサイズ
    static constexpr float GUARD_EFFECT_SIZE = 20.0f;
#ifdef DEBUG_COOL
    //クールタイム
    static constexpr float NORMAL_ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 1.0f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;

   

#else // DEBUG_COOL
#ifdef DEBUG_ON
    void DrawDebug(void)override;
#endif // DEBUG_ON

    //クールタイム
#endif // DEBUG_COOL
    //通常攻撃
    //------------------------------------------------------------------
    //クールタイム
    static constexpr float NORMAL_ATK_COOLTIME = 0.75f;
    //前隙
    static constexpr float NORMAL_ATK_START = 0.2f; 
    //持続時間
    static constexpr float NORMAL_ATK_DURATION = 0.4f - NORMAL_ATK_START;  
    //後隙
    static constexpr float NORMAL_ATK_BACKRASH = 0.1f;       
    //ローカル座標
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f }; 
    //当たり判定の大きさ
    static constexpr float NORMAL_ATK_COL = CHARACTER_SCALE * 100.0f;
    //威力 
    static constexpr float NORMAL_ATK_POW = 9.0f;
    //------------------------------------------------------------------
    //スキル1
    //------------------------------------------------------------------
    //クールタイム
    static constexpr float SKILL_ONE_COOLTIME = 3.3f;
    //前隙
    static constexpr float SKILL_ONE_START = 0.2f;
    //持続時間
    static constexpr float SKILL_ONE_DURATION = 0.3f;
    //後隙
    static constexpr float SKILL_ONE_BACKRASH = 0.2f;
    //当たり判定の大きさ
    static constexpr float SKILL_ONE_COL = CHARACTER_SCALE * 150.0f;
    //相対座標
    static constexpr VECTOR SKILL_ONE_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    //威力
    static constexpr float SKILL_ONE_POW = 30.0f;   

    //------------------------------------------------------------------
    //スキル2
    //------------------------------------------------------------------
    //クールタイム
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;
    //前隙
    static constexpr float SKILL_TWO_START = 0.3f;
    //持続時間
    static constexpr float SKILL_TWO_DURATION = 3.0f;
    //後隙
    static constexpr float SKILL_TWO_BACKRASH = 0.0f;
    //当たり判定
    static constexpr float SKILL_TWO_COL = CHARACTER_SCALE * 0.0f;
    //相対座標
    static constexpr VECTOR SKILL_TWO_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };
    //威力(ガードスキルなので威力はない)
    static constexpr float SKILL_TWO_POW = 0.0f;

    //ガード時のステップ
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.7f;
    //------------------------------------------------------------------
    //通常攻撃の最大値
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS
        ,NORMAL_ATK_COL
        ,NORMAL_ATK_POW
        ,NORMAL_ATK_DURATION
        ,NORMAL_ATK_BACKRASH
        ,0.0f,false };

    //スキル１
    static constexpr ATK SKILL_ONE_MAX{ SKILL_ONE_COL_LOCAL_POS
        ,SKILL_ONE_COL
        ,SKILL_ONE_POW
        ,SKILL_ONE_DURATION
        ,SKILL_ONE_BACKRASH
        ,0.0f,false };

    //スキル２
    static constexpr ATK SKILL_TWO_MAX{ SKILL_TWO_COL_LOCAL_POS
        ,SKILL_TWO_COL
        ,SKILL_TWO_POW
        ,SKILL_TWO_DURATION
        ,SKILL_TWO_BACKRASH
        ,0.0f,false };

    //固有アニメーション
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_ONE_NUM = 2;
    static constexpr int SKILL_TWO_NUM = 18;
    static constexpr int BLOCKING_NUM = 21;
    Knight(void);
    ~Knight(void) = default;
    void SetParam(void)override;
    void Update(void)override;
    void Draw(void)override;



protected:
    //攻撃アクションのパラメータの初期化
    void InitAct(void)override;
    void AtkFunc(void)override;

    //void ResetGuardCnt(void)override;

    //攻撃入力
    void NmlAtkInit(void)override;
    //スキル入力
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;

    //各スキルの処理
    void InitSkill(ATK_ACT _act);
    void Skill1Func(void)override;
    void Skill2Func(void)override;

    //固有アニメーション番号の初期化
    void InitCharaAnim(void)override;

private:
};

