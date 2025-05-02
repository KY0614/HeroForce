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



    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//攻撃開始判定の大きさ
    //スキル使用開始時にクールタイムを増やす時間
    static constexpr float SKILL_TWO_START_COOLTIME = 3.0f;

    //ガード可能になる残りクールタイム(ガード時間最低1秒確保したいから)
    static constexpr float GUARD_STARTABLE_COOL = 4.0f;

    //ガードエフェクトサイズ
    static constexpr float GUARD_EFFECT_SIZE = 20.0f;
#ifdef DEBUG_COOL
    //クールタイム
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 1.0f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;

   

#else // DEBUG_COOL
#ifdef DEBUG_ON
    void DrawDebug(void)override;
#endif // DEBUG_ON

    //クールタイム
    static constexpr float ATK_COOLTIME = 0.75f;
    static constexpr float SKILL_ONE_COOLTIME = 3.3f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;
#endif // DEBUG_COOL
    //通常攻撃のステータス
    //------------------------------------------------------------------
    static constexpr float ATK_START = 0.2f;                               //前隙
    static constexpr float FRAME_ATK_DURATION = 0.4f - ATK_START;          //持続時間
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;                      //後隙
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };    //ローカル座標
    static constexpr float COL_ATK = CHARACTER_SCALE * 100.0f;             //当たり判定の半径
    static constexpr float NMLATK_POW = 9.0f;          //通常攻撃             //技威力
    //------------------------------------------------------------------

    //スキル1のステータス
    //------------------------------------------------------------------
    //近接
    static constexpr float SKILL_ONE_START = 0.2f;
    static constexpr float FRAME_SKILL1_DURATION = 0.3f;
    static constexpr float FRAME_SKILL1_BACKRASH = 0.2f;
    static constexpr float COL_SKILL1 = CHARACTER_SCALE * 150.0f;
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr float SKILL_ONE_POW = 30.0f;   

    //斬撃
    static constexpr float COL_SLASH = CHARACTER_SCALE * 100.0f;
    static constexpr VECTOR SLASH_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr float FRAME_SLASH_DURATION = 2.0f;
    static constexpr float SLASH_POW = 30.0f;       //スキル1の斬撃
    static constexpr float SLASH_SPEED = 15.0f;



    //------------------------------------------------------------------

    //スキル2のステータス
    //------------------------------------------------------------------
    //ATKのステータス
    static constexpr float SKILL_TWO_START = 0.3f;
    static constexpr float FRAME_SKILL2_DURATION = 3.0f;
    static constexpr float FRAME_SKILL2_BACKRASH = 0.0f;
    //static constexpr float COL_SKILL2 = CHARACTER_SCALE * 400.0f;
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 0.0f;
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };
    static constexpr float SKILL_TWO_POW = 0.0f;

    //ガード時のステップ
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.7f;
    //------------------------------------------------------------------
    //通常攻撃の最大値
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS
        ,COL_ATK
        ,NMLATK_POW
        ,FRAME_ATK_DURATION
        ,FRAME_ATK_BACKRASH
        ,0.0f,false };

    //スキル１
    static constexpr ATK SKILL_ONE_MAX{ SKILL1_COL_LOCAL_POS
        ,COL_SKILL1
        ,SKILL_ONE_POW
        ,FRAME_SKILL1_DURATION
        ,FRAME_SKILL1_BACKRASH
        ,0.0f,false };

    //斬撃
    static constexpr ATK SLASH_MAX{ SLASH_LOCAL_POS
        ,COL_SLASH
        ,SLASH_POW
        ,FRAME_SLASH_DURATION
        ,0.0f
        ,0.0f
        ,false
    };

    //スキル２
    static constexpr ATK SKILL_TWO_MAX{ SKILL2_COL_LOCAL_POS
        ,COL_SKILL2
        ,SKILL_TWO_POW
        ,FRAME_SKILL2_DURATION
        ,FRAME_SKILL2_BACKRASH
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
    //メンバー関数
    void InitSlashAtk(ATK & arrowAtk);
    void CreateSlash(void);
    void CreateSlashAtk(void);




    //メンバ変数
    float guardCnt_;        //ガード時間
    //std::vector<ATK>slashArrow_;
    ATK slashArrow_;                            //斬撃(一つしか生成しないため普通の変数にする)
    int arrowMdlId_;							//矢のモデル
    std::vector<std::shared_ptr<Arrow>> arrow_;	//弓矢
    bool isShotArrow_;							//矢を放ったかの判定(true:放った)
    int arrowCnt_;								//矢の使用個数カウンタ
    float reloadCnt_;							//矢のリロード時間

    float atkAbleCnt_;                          //矢の発射可能カウント
};

