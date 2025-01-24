#pragma once

#include "PlayerBase.h"
class Arrow;
class Archer :
    public PlayerBase
{
public:
    //ステータス
    static constexpr int ATK_POW = 90;
    static constexpr int MAX_HP = 235;
    static constexpr int MAX_DEF = 90;
    static constexpr float SPEED = PlayerBase::MOVE_SPEED_FAST;


    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//攻撃開始判定の大きさ
    static constexpr float ATKABLE_TIME = 0.0f;                         //連射受付時間
    //通常攻撃
    //-----------------------------------------------------------
    static constexpr float ATK_COOLTIME = 0.3f;
    static constexpr float ATK_START = 0.3f;
    static constexpr float FRAME_ATK_DURATION = 2.0f;
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr float POW_ATK = 7.0f;

    static constexpr float COL_ATK = CHARACTER_SCALE * 100.0f;


    //-----------------------------------------------------------
    


    //スキル1
    //-----------------------------------------------------------
    static constexpr float SKILL_ONE_COOLTIME = 3.0f;
    static constexpr float SKILL_ONE_START =2.0f;
    static constexpr float FRAME_SKILL1_DURATION = 0.7f;
    static constexpr float FRAME_SKILL1_BACKRASH = 0.2f;
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr float COL_SKILL1 = CHARACTER_SCALE * 150.0f; 

    //溜めた時と溜めなかった時の威力
    static constexpr float SKILL_ONE_POW_MIN = 15.0f;
    static constexpr float SKILL_ONE_POW_MAX = 25.0f;

    //溜めアニメーションのステップ
    static constexpr float SKILL_CHARGE_STEPANIM = 9.5f;


    //プレイヤー自体の後隙
    static constexpr float BACKRASH_MAX = 0.5f;


    //-----------------------------------------------------------

    //スキル2
    //-----------------------------------------------------------
    //static constexpr float SKILL_TWO_COOLTIME = 5.0f;
    static constexpr float SKILL_TWO_COOLTIME = 0.5f;
    static constexpr float SKILL_TWO_START = 0.2f;
    static constexpr float FRAME_SKILL2_DURATION = 3.0f - SKILL_TWO_START;
    static constexpr float FRAME_SKILL2_BACKRASH = 0.2f;
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 200.0f;
    static constexpr float SKILL_TWO_POW = 0.0f;


    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.25f;


    //-----------------------------------------------------------
    // 
    //固有アニメーション
    static constexpr int ATK_NUM = 16;
    static constexpr int SKILL_ONE_NUM = 16;
    static constexpr int SKILL_TWO_NUM = 16;
    //static constexpr int AIMING_NUM = 14;

    //通常攻撃の最大値
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS,COL_ATK,ATK_POW,FRAME_ATK_DURATION,FRAME_ATK_BACKRASH,0.0f,false };

    //スキル１
    static constexpr ATK SKILL_ONE_MAX{ SKILL1_COL_LOCAL_POS ,COL_SKILL1 ,SKILL_ONE_POW_MIN,FRAME_SKILL1_DURATION ,FRAME_SKILL1_BACKRASH ,0.0f,false };

    //スキル２
    static constexpr ATK SKILL_TWO_MAX{ SKILL2_COL_LOCAL_POS ,COL_SKILL2 ,SKILL_TWO_POW,FRAME_SKILL2_DURATION ,FRAME_SKILL2_BACKRASH ,0.0f,false };


    //弓矢関係
    static constexpr int ARROW_SIZE_MAX = 5;	//矢の最大保持数
    static constexpr float RELOAD_TIME = 5.0f;	//矢のリロード時間
    static constexpr float ARROW_SPEED = 10.0f;	//矢のとぶスピード


    Archer(void);
    ~Archer(void) = default;
    void SetParam(void)override;

    //各アクションの初期化
    void InitAct(void)override;

    //固有アニメーション番号の初期化
    void InitCharaAnim(void)override;

    //攻撃が終わった後の初期化
    void InitAtk(void)override;

    //弓矢の作成
    void CreateArrow(void);

    //arrowAtkの作成
    void CreateAtk(void);

    //矢のゲッタ
    const ATK GetArrowAtk(const int i)override { return arrowAtk_[i]; }
    const int GetArrowCnt(void)override { return arrowCnt_; }

    

    void Update(void)override;

    void Draw(void)override;

protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;

    void NmlAtkInit(void)override;
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;

    //弓連射用
    //void NmlActCommon (void)override;

    //弓矢一つのatk初期化
    void InitArrowAtk(ATK & arrowAtk);

    std::vector<ATK>arrowAtk_;
    int arrowMdlId_;							//矢のモデル
    std::vector<std::shared_ptr<Arrow>> arrow_;	//弓矢
    bool isShotArrow_;							//矢を放ったかの判定(true:放った)
    int arrowCnt_;								//矢の使用個数カウンタ
    float reloadCnt_;							//矢のリロード時間

    float atkAbleCnt_;                          //矢の発射可能カウント
    float backrashCnt_;                            //アーチャー自体の後隙
};

