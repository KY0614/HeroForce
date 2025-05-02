#pragma once
#include<map>
#include<functional>
#include"../Utility/AsoUtility.h"
#include"../../../Manager/Generic/SceneManager.h"
#include "../../UnitBase.h"
#include"../../../Manager/Generic/InputManager.h"
#include"../../../Manager/Decoration/EffectManager.h"
#include"../PlayerManager.h"
#include"../PlayerInput.h"
#include "../../UnitBase.h"

//デバッグ
//#define DEBUG_ON
//#define INPUT_DEBUG_ON
//#define DEBUG_COOL
class PlayerDodge;
class PlayerBase :
    public UnitBase
{
public:
#ifdef DEBUG_ON
    void InitDebug(void);
    unsigned int color_Col_;
    unsigned int color_Atk_;
    unsigned int color_skl1_;
    unsigned int color_skl2_;

    //デバッグ用関数
    virtual void DrawDebug(void);

#endif // DEBUG_ON
    //デバッグ用
      //エフェクトを最初の1フレームの時に再生させる用のFPS
    static constexpr float DELTATIME = 1.0f / 60.0f;


    //各アニメーション番号
    static constexpr int T_POSE_NUM = 64;
    static constexpr int IDLE_NUM = 36;
    static constexpr int WALK_NUM = 72;
    static constexpr int RUN_NUM = 49;
    static constexpr int DODGE_NUM = 28;
    static constexpr int DAMAGE_NUM = 35;
    static constexpr int DEATH_NUM = 23;

    //アニメーションスピード
    static constexpr float SPEED_ANIM_IDLE = 20.0f;
    static constexpr float SPEED_ANIM_RUN = 80.0f;
    static constexpr float SPEED_ANIM_DODGE = 30.0f;
    static constexpr float SPEED_ANIM_ATK = 50.0f;

    //死にアニメーションの止めるカウント
    static constexpr float DEATH_STEP_ANIM = 22.7;

    //攻撃の種類の数
    static constexpr int ATK_TOTAL = 3;

    // 移動スピード
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DODGE = 15.0f;
    static constexpr float MOVE_SPEED_SLOW = 3.0f;
    static constexpr float MOVE_SPEED_NORMAL = 5.0f;
    static constexpr float MOVE_SPEED_FAST = 8.0f;

    static constexpr float FRAME_DODGE_MAX = 1.0f * CHARACTER_SCALE;
    static constexpr float DODGE_CDT_MAX = 0.5f;

    //プレイヤー自身の当たり判定
    static constexpr VECTOR PLAYER_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //自身の当たり判定半径
    static constexpr float MY_COL_RADIUS = 66.0f * CHARACTER_SCALE;

    //アーチャーの索敵(敵に矢を向ける範囲)
    static constexpr float ARCHER_SEARCH_RANGE = CHARACTER_SCALE * 1300.0f;

    //フェーズ変わるときにプレイヤーが死んでた時のhp回復
    static constexpr float CHANGEFAZE_RECOVERY_PER = 0.2f;

    //プレイヤーの初期座標
    static constexpr VECTOR PLAYER1_POS = { -200.0f,0.0F,0.0f };
    static constexpr VECTOR PLAYER2_POS = { -100.0f,0.0F,0.0f };
    static constexpr VECTOR PLAYER3_POS = { 0.0F,0.0F,0.0f };
    static constexpr VECTOR PLAYER4_POS = { 100.0F,0.0F,0.0f };

    //音量
    static constexpr int SKILL_CHANGE_SE_VOL = 80;

    //*************************************************
    // 列挙型
    //*************************************************

    //攻撃種類
    enum class ATK_ACT
    {
        ATK
        , SKILL1
        , SKILL2
        , MAX
    };

    //スキル変更用
    enum class SKILL_NUM
    {
        ONE = 1
        , TWO = 2
        , MAX
    };

    enum class ATK_TYPE
    {
        ATTACK
        ,BUFF
        ,MAX
    };

    enum class STATUSBUFF_TYPE
    {
        ATK_BUFF
        ,DEF_BUFF
        ,SPD_BUFF
        ,MAX
    };

    //バフするスキル
    enum class SKILL_BUFF
    {
        BUFF_ARROW  //アーチャーのスキル2
        ,GUARD      //ナイトのガード
        ,MAX
    };

    struct BUFF
    {
        float cnt;
        float percent[static_cast<int>(STATUSBUFF_TYPE::MAX)];
        bool isBuff;            //バフしたかどうか(バフの重ね掛けを防ぐ)
        bool isBuffing;         //バフ中かどうか  (毎フレームバフするのを防ぐ)
    };

    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void) = 0;
    void Init(void)override;
    virtual void Update(void)override;
    virtual void Draw(void)override;


    /// <summary>
    /// 矢の当たり判定
    /// </summary>
    /// <param name="_chaser">攻撃する人</param>
    /// <param name="_target">攻撃される人</param>
    /// <param name="_num">当たり判定する矢の配列番号</param>
    /// <returns></returns>
    virtual const bool IsHitArrowAtk(const PlayerBase& _chaser, const UnitBase& _target, const int _num) { return false; }

    //動的配列のATKを返す
    //virtual std::vector<ATK>GetAtks(void) { return atks_; }

    virtual std::vector<ATK> GetAtks(ATK_TYPE _type) { return atks_[_type]; }

    virtual void SetIsArrowHit(ATK_TYPE _type, const bool _flg, int _num){ atks_[_type][_num].isHit_ = _flg; }

     //ダメージ関数
    void Damage(void);

    //リセット
    void Reset(void);

    //役職それぞれのバフ(使わない役職もあるためスタブ)
    virtual void Buff(PlayerBase& _target) {}
    void BuffPerAdd(STATUSBUFF_TYPE _type, float _per);
    //移動関連
     //-------------------------------------
    //移動処理
    void Move(float _deg, VECTOR _axis);

    //方向処理
    void Turn(float _deg, VECTOR _axis);

    //動いてるかどうか
    bool IsMove(void) { return speed_ > 0.0f; }

    //スキル使用可能かどうか
    bool IsSkillable(void);

    //スキル変更処理
    void SkillChange(void);

    //バフ更新
    void BuffUpdate(void);

    //*****************************************************
    //ゲッタ
    //*****************************************************
    //クール中
    const bool GetIsCool(ATK_ACT _act) { return isCool_[static_cast<int>(_act)]; }

    //クールタイム
    const float GetCoolTime(ATK_ACT _act) { return coolTime_[static_cast<int>(_act)]; }

    //現在の使いたいスキル
    const ATK_ACT GetSkillNo(void) { return skillNo_; }

    //攻撃中判定
    const bool GetIsAtk(void) { return isAtk_; }

    //スキル中判定
    const bool GetIsSkill(void) { return isSkill_; }

    //前隙
    const float GetAtkStartCnt(void) { return atkStartCnt_; }

    //アニメーションステップ
    const float GetStepAnim(void) { return stepAnim_; }

    //回避
    PlayerDodge* GetDodge(void) { return dodge_; }

    //クールタイム割合のゲッタ
    const float* GetCoolTimePer(void) { return coolTimePer_; }

    //バフされているかゲッタ
    //const bool GetIsBuff(void) { return isBuff_; }

    const bool GetIsBuff(SKILL_BUFF _skill) { return buffs_[_skill].isBuff; }

    //矢などの遠距離武器のゲッタ(KnightとArcherで使う)
    virtual const ATK GetArrowAtk(const int i) { return ATK(); }
    virtual const ATK GetArrowAtk(const ATK_TYPE _act, const int i) { return ATK(); }


    //遠距離武器の個数を獲得
    virtual const int GetArrowCnt(const int _act) { return 0; }

    //攻撃力ゲッタ
    float GetAtkPow(void) { return atkPow_; }

    SceneManager::ROLE GetRole(void) { return role_; }

    VECTOR GetInitPos(int _num) { return initPos_[_num]; }
    


    //**************************************************************
    //セッター
    //**************************************************************
    //攻撃系
    //-------------------------------------------------------------------------------------------------------------------
    //攻撃のそれぞれの値
    void SetAtk(const ATK _atk) { atk_ = _atk; }
    //前隙のカウンター
    void SetAtkStartCnt(const float _atkStartCnt) { atkStartCnt_ = _atkStartCnt; }

    //前隙の最大時間セッタ
    void SetAtkStartTime(const float _atkStartTime, const ATK_ACT _act) { atkStartTime_[static_cast<int>(_act)] = _atkStartTime; }

    //攻撃するかどうか
    void SetIsAtk(const bool _isAtk) { isAtk_ = _isAtk; }

    //攻撃発生中フラグ
    const bool IsAtkStart(void)const { return 0.0f < atkStartCnt_ && atkStartCnt_ <= atkStartTime_[static_cast<int>(act_)]; }

    //攻撃発生したのを確認する
    const bool IsFinishAtkStart(void)const { return atkStartCnt_ > atkStartTime_[static_cast<int>(act_)]; }

    //攻撃変更用(主に入力されたら変えるようにする)
    void ChangeAct(const ATK_ACT _act);

    //攻撃の最大値の初期化(弓矢とかの違うatkの配列とか使う用)
    void ResetParam(ATK& _atk);

    //近接攻撃のatk初期化用
    void ResetParam(void);

    //攻撃終わった後の初期化
    virtual void InitAtk(void);

    //持続時間セッタ
    void SetDulation(const float _dulation) { atk_.duration_ = _dulation; }

    //スキルするか
    void SetIsSkill(const bool _isSkill) { isSkill_ = _isSkill; }

    //移動可能かどうか
    void SetMoveAble(const bool _moveAble) { moveAble_ = _moveAble; }
    //クールにするかどうか
    void SetIsCool(const bool _isCool, const ATK_ACT _act) { isCool_[static_cast<int>(_act)] = _isCool; }

    //クールタイムセッタ
    void SetCoolTime(const float coolTime, ATK_ACT _act) { coolTime_[static_cast<int>(_act)] = coolTime; }

    //アーチャーのサーチセッタ
    void SetIsSerchArcher(const bool _isSerch) { isSerchArcher_ = _isSerch; }

    //その他
    //------------------------------------------------------------------------------------
    //アニメーションステップ
    void SetStepAnim(const float _stepAnim) { stepAnim_ = _stepAnim; }

    //スピード
    void SetSpeed(const float _speed) { speed_ = _speed; }

    //バフセッタ(時間制限付き)
    //void SetBuff(STATUSBUFF_TYPE _type, float _per,float _second);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="_type"></param>
    /// <param name="_skill"></param>
    /// <param name="_per"></param>
    /// <param name="_second"></param>
    void SetBuff(STATUSBUFF_TYPE _type,SKILL_BUFF _skill, float _per,float _second);

    //時間制限なし
    //void SetBuff(STATUSBUFF_TYPE _type, float _per);

    //前のステータス情報をセットする
    void SetPreStatus(void);

    //バフした判定セッタ
    //void SetIsBuff(const bool _isBuff) { isBuff_ = _isBuff; }

    /// <summary>
    /// バフしたかセッタ
    /// </summary>
    /// <param name="_isBuff">true:バフした</param>
    /// <param name="_skill">バフしたスキルは何か</param>
    void SetIsBuff(SKILL_BUFF _skill,const bool _isBuff) { buffs_[_skill].isBuff = _isBuff; }

    void SetIsBuff(PlayerBase& _player,SKILL_BUFF _skill, const bool _isBuff) { _player.SetIsBuff(_skill, _isBuff); }


    //ターゲットセッタ
    void SetTargetPos(const VECTOR _targetPos) { targetPos_ = _targetPos; } 

 

protected:
    //ポインタ
      //回避機能
    PlayerDodge* dodge_;

    //*************************************************
    // 列挙型
    //*************************************************
    struct PlayerAtk
    {
        ATK_ACT act_;
        float atkStartCnt_;
        ATK atk_;
        float CoolTime_[static_cast<int>(ATK_ACT::MAX)];
        float CoolTimeMax_[static_cast<int>(ATK_ACT::MAX)];                //クールタイム最大
        std::map<ATK_ACT, ATK>atkMax_;
        float atkStartTime_[static_cast<int>(ATK_ACT::MAX)];            //攻撃発生時間
        bool IsAtkStart(void) { return 0.0f < atkStartCnt_ && atkStartCnt_ <= atkStartTime_[static_cast<int>(act_)]; }
    };


    //*************************************************
    // メンバ変数
    //*************************************************
    //システム系
    //ステータス系
    ATK_ACT act_;                                               //攻撃種類
    std::map < ATK_ACT, std::function<void(void)>>changeAct_;   //攻撃の変更
    std::function<void(void)>actUpdate_;                        //攻撃ごとの更新処理
    float atkStartCnt_;                                         //攻撃が発生するまでのカウント
    ATK_TYPE atkTypes_[static_cast<int>(ATK_ACT::MAX)];         //攻撃のタイプ(チャージするかしないか)
    std::map<ATK_TYPE, std::function<void(void)>>changeAtkType_;//攻撃タイプ変更
    std::function<void(void)>atkTypeUpdate_;                    //攻撃タイプごとのアップデート
    ATK_TYPE atkType_;                                          //タイプ変数
    float coolTime_[static_cast<int>(ATK_ACT::MAX)];            //それぞれのクールタイムカウント
    bool isCool_[static_cast<int>(ATK_ACT::MAX)];               //それぞれの攻撃使い終わりを格納する
    float multiHitInterval_;                                    //多段ヒットのダメージ間隔
    VECTOR userOnePos_;                                         //ユーザー1追従用の座標   
    VECTOR colPos_;                                             //プレイヤーの当たり判定座標
    bool isSerchArcher_;                                              //アーチャーの射程圏内に入ったかどうか
    float speed_;                                               //プレイヤーのスピード(ステータスではなく1フレームに動くもの)



    //誰をターゲットにするか
    VECTOR targetPos_;

 


    bool isPush_;                                               //長押しスキル用のボタンを押しているかどうか  true:押している
    bool moveAble_;             //移動可能かを返す  true:移動可能
    bool isAtk_;                                                 //通常攻撃開始したかどうか
    bool isSkill_;                                                 //スキル開始したかどうか
    bool isBuff_;                                               //バフかそうでないか     true：バフである




    //それぞれの最大値セット用(攻撃の座標はローカル座標で設定してます)
    std::map<ATK_ACT, ATK>atkMax_;
    float coolTimeMax_[static_cast<int>(ATK_ACT::MAX)];             //クールタイム最大
    float atkStartTime_[static_cast<int>(ATK_ACT::MAX)];            //攻撃発生時間

    //コントローラー系
    InputManager::JOYPAD_NO padNum_;                //ゲームパッドの番号
    int leftStickX_;            //パッドのスティックのX角度  
    int leftStickY_;            //パッドのスティックのY角度
    float stickDeg_;            //パッドのスティックの角度


    //*************************************************
    //メンバ関数
    //*************************************************
    VECTOR GetTargetVec(VECTOR _targetPos,bool _isMove=true);



    //アニメNo初期化
    void InitAnimNum(void);

    //役割ごとに違うアニメーションの初期化
    virtual void InitCharaAnim(void);

    //それぞれのアクションの初期化
    virtual void InitAct(void);

    //スキル変更時の初期化
    void ChangeNmlAtk(void);
    void ChangeSkillOne(void);
    void ChangeSkillTwo(void);

    //USER関係
    //------------------------------------------------
    //ユーザーがいるときの更新
    void UserUpdate(void);

    SceneManager::ROLE role_;

    ATK_ACT skillNo_;     //スキル変更用


    //プレイヤー(CPUとユーザー)共通処理
    //--------------------------------------------------
    //攻撃処理
    virtual void AtkFunc(void) = 0;
    //スキル1
    virtual void Skill1Func(void) = 0;
    //スキル2
    virtual void Skill2Func(void) = 0;

    //攻撃座標の同期
    void SyncActPos(ATK& _atk);

    //スキルごとの操作更新
    void ChangeSkillControll(ATK_ACT _skill);

    //攻撃
    //-------------------------------------
    //攻撃中かどうか(UnitBaseで修正予定)
    const bool IsAtkAction(void)const { return IsAtkStart() || atk_.IsAttack() || atk_.IsBacklash(); }

    //攻撃可能かどうか(true:可能)
    const bool IsAtkable(void)const;


    //回避関連
    //---------------------------------------
    //回避可能か
    //回避中かどうか

    const bool IsDodgeable(void)const;

    //スキル
    //virtual void Skill(void);
    /// <summary>
    /// スキル中フラグ
    /// </summary>
    /// <param name="_frameSkillNo">スキルフレーム最大値(今はスキル1か2)</param>
    /// <returns>スキル中/スキル中でない</returns>
    bool IsSkillAction(SKILL_NUM _num) {
        return atk_.IsAttack()
            || atk_.IsBacklash();
    }

    //クールタイムのカウント
    void CoolTimeCnt(void);

private:
    //メンバ変数
    float coolTimePer_[static_cast<int>(ATK_ACT::MAX)];

#ifdef DEBUG_INPUT

    //******************************************************************************************
#endif // DEBUG_ON

    //アクションの発動条件
    void ProcessAct(void);
    void CoolTimePerCalc(void);
    //std::vector <ATK>atks_;

    std::map<ATK_TYPE, std::vector<ATK>>atks_;

    std::map<SKILL_BUFF, BUFF>buffs_;
    std::map<STATUSBUFF_TYPE, float>buffpers_;

    //バフ関係
    float buffCnt_[static_cast<int>(STATUSBUFF_TYPE::MAX)];                                          //バフのカウンター(攻撃力、防御力、スピード)  
    float buffPercent_[static_cast<int>(STATUSBUFF_TYPE::MAX)];                                      //バフの加算
    STATUSBUFF_TYPE buffType_;                                   
    bool isBuffing_;                                                                             //バフ中かどうか

    float bufAtk_;          //バフ後の攻撃力
    float bufDef_;           //バフ後の防御力
    float bufSpd_;           //バフ後のスピード

    float preAtkPow_;          //バフ後の攻撃力
    float preDef_;           //バフ後の防御力
    float preSpd_;           //バフ後のスピード

    VECTOR initPos_[PlayerManager::PLAYER_NUM];     //プレイヤーたちの初期座標を格納



    //攻撃入力
    virtual void NmlAtkInit(void) = 0;
    //スキル入力
    virtual void SkillOneInit(void) = 0;
    virtual void SkillTwoInit(void) = 0;

};