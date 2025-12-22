#pragma once

#include "PlayerBase.h"
class Arrow;
class Archer :
    public PlayerBase
{
public:
#ifdef DEBUG_ON
    void DrawDebug(void)override;
#endif // DEBUG_ON

    //ステータス
    static constexpr int ATK_POW = 90;
    static constexpr int MAX_HP = 235;
    static constexpr int MAX_DEF = 90;
    static constexpr float SPEED = PlayerBase::MOVE_SPEED_FAST;

    //バフ強化値
    static constexpr float BUFF = 20.0f;

    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//攻撃開始判定の大きさ
    static constexpr float ATKABLE_TIME = 0.0f;                         //連射受付時間

    //エフェクトサイズ
    static constexpr float ARROW_EFFECT_SIZE = 20.0f;
    //通常攻撃
    //-----------------------------------------------------------
    //クールタイム
    static constexpr float NORMAL_ATK_COOLTIME = 0.3f;
    //前隙
    static constexpr float NORMAL_ATK_START = 0.3f;
    //矢の生存時間
    static constexpr float NORMAL_ATK_DURATION = 2.0f;
    //後隙
    static constexpr float NORMAL_ATK_BACKRASH = 0.1f;
    //攻撃座標の相対座標
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    //攻撃力
    static constexpr float NORMAL_ATK_POW = 7.0f;
    //当たり判定の大きさ
    static constexpr float NORMAL_ATK_COL = CHARACTER_SCALE * 100.0f;

    //プレイヤー自体の後隙
    static constexpr float NMLATK_BACKRASH_MAX = 0.2f;
    //-----------------------------------------------------------
    //スキル1
    //-----------------------------------------------------------
    //クールタイム
    static constexpr float SKILL_ONE_COOLTIME = 3.0f;
    //前隙
    static constexpr float SKILL_ONE_START = 2.0f;
    //威力が強くなる溜め時間
    static constexpr float SKILL_ONE_CHARGE_START_CNT = 1.0f;
    //攻撃時間
    static constexpr float SKILL_ONE_DURATION = 0.7f;
    //後隙
    static constexpr float SKILL_ONE_BACKRASH = 0.2f;
    //攻撃相対座標
    static constexpr VECTOR SKILL_ONE_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    //当たり判定の大きさ
    static constexpr float SKILL_ONE_COL = CHARACTER_SCALE * 150.0f;
    //プレイヤー自体の後隙
    static constexpr float SKILL_ONE_BACKRASH_MAX = 0.5f;

    //スキル２固有のもの
    //溜めた時の威力
    static constexpr float SKILL_ONE_POW_MIN = 15.0f;
    //溜めなかった時の威力
    static constexpr float SKILL_ONE_POW_MAX = 25.0f;

    //溜めアニメーションのステップ
    static constexpr float SKILL_CHARGE_STEPANIM = 9.5f;
    //-----------------------------------------------------------
    //スキル2
    //-----------------------------------------------------------
    //クールタイム
    static constexpr float SKILL_TWO_COOLTIME = 5.0f;
    //前隙
    static constexpr float SKILL_TWO_START = 0.2f;
    //行動時間
    static constexpr float SKILL_TWO_DURATION = 3.0f - SKILL_TWO_START;
    //後隙
    static constexpr float SKILL_TWO_BACKRASH = 0.2f;
    //攻撃の相対座標
    static constexpr VECTOR SKILL_TWO_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };
    //攻撃の当たり判定
    static constexpr float SKILL_TWO_COL = CHARACTER_SCALE * 200.0f;
    //バフスキルなので攻撃力は0
    static constexpr float SKILL_TWO_POW = 0.0f;
    //バフ持続時間
    static constexpr float SKILL_TWO_BUFF_TIME = 20.0f;
    //後隙
    static constexpr float SKILL_TWO_BACKRASH_MAX = 0.2f;
    //-----------------------------------------------------------
    // 
    //固有アニメーション
    static constexpr int ATK_NUM = 16;
    static constexpr int SKILL_ONE_NUM = 16;
    static constexpr int SKILL_TWO_NUM = 16;

    //通常攻撃の最大値
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS,NORMAL_ATK_COL,NORMAL_ATK_POW,NORMAL_ATK_DURATION,NORMAL_ATK_BACKRASH,0.0f,false };

    //スキル１
    static constexpr ATK SKILL_ONE_MAX{ SKILL_ONE_COL_LOCAL_POS ,SKILL_ONE_COL ,SKILL_ONE_POW_MIN,SKILL_ONE_DURATION ,SKILL_ONE_BACKRASH ,0.0f,false };

    //スキル２
    static constexpr ATK SKILL_TWO_MAX{ SKILL_TWO_COL_LOCAL_POS ,SKILL_TWO_COL ,SKILL_TWO_POW,SKILL_TWO_DURATION ,SKILL_TWO_BACKRASH ,0.0f,false };

    //弓矢関係
    //矢の最大保持数
    static constexpr int ARROW_SIZE_MAX = 5;		
    //矢のとぶスピード
    static constexpr float ARROW_SPEED = 10.0f;	

    //スキルチャージエフェクトの大きさ
    static constexpr float CHARGE_SKILL_EFF_SIZE = 50.0f;
 
    //関数
    //********************************************
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
    void CreateArrow(ATK_TYPE _type);

    //arrowAtkの作成
    void CreateAtk(ATK_TYPE _type);

    //矢関連のゲッタ
    //-------------------------------------------------
    //ATK
    const PlayerBase::ATK GetArrowAtk(const ATK_TYPE _type, const int i)override;
    //矢の個数
    const int GetArrowCnt(const int _type)override { return arrow_[static_cast<ATK_TYPE>(_type)].size(); }
    //矢のヒット
    void SetIsArrowHit(ATK_TYPE _type, const bool _flg, int _num)override;
    //矢の当たり判定で必要
    std::vector<ATK> GetAtks(ATK_TYPE _type)override { return arrowAtk_[_type]; }
    //-------------------------------------------------
    //バフ
    void Buff(PlayerBase& _target)override;

    //解放
    void Destroy(void)override;
    //更新
    void Update(void)override;
    //描画
    void Draw(void)override;
    //********************************************

protected:
    //メンバ関数
    //*************************************************
    //通常攻撃
    void AtkFunc(void)override;
    //スキル１
    void Skill1Func(void)override;
    //スキル２
    void Skill2Func(void)override;

    //各アクションの初期化処理
    void NmlAtkInit(void)override;
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;

    //矢と矢に対応した攻撃に対応する更新処理(スキルに名前を付けてやってみる)
    void ArrowUpdate(ATK_TYPE _type);

    //弓矢一つのatk初期化
    void InitArrowAtk(ATK& arrowAtk);
    //*************************************************
    //メンバ変数
    //*************************************************
    std::map<ATK_TYPE, std::vector<ATK>>arrowAtk_;  //矢のATK
    int arrowMdlId_;							//矢のモデル
    std::map<ATK_TYPE, std::vector<std::shared_ptr<Arrow>>> arrow_;	//弓矢
    bool isShotArrow_;							//矢を放ったかの判定(true:放った)

    //各攻撃の矢の個数カウント
    int arrowCnt_[static_cast<int>(ATK_TYPE::MAX)];	//矢の使用個数カウンタ
    float backrashCnt_;                             //アーチャー自体の後隙
    //*************************************************
};

