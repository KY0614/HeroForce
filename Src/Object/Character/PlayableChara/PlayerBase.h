#pragma once
#include<map>
#include<functional>
#include"../Utility/AsoUtility.h"
#include"../../../Manager/Generic/SceneManager.h"
#include "../../UnitBase.h"
#include"../../../Manager/Generic/InputManager.h"
#include"../PlayerInput.h"
#include "../../UnitBase.h"

//デバッグ
#define DEBUG_ON
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

#endif // DEBUG_ON
    //デバッグ用


    //デバッグ用関数
    virtual void DrawDebug(void);

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

    //攻撃の種類の数
    static constexpr int ATK_TOTAL = 3;

    // 移動スピード
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DEG = 5.0f;
    static constexpr float SPEED_DODGE = 15.0f;
   



    static constexpr float FRAME_DODGE_MAX = 1.0f * CHARACTER_SCALE;
    static constexpr float DODGE_CDT_MAX = 0.5f;



    //プレイヤー自身の当たり判定
    static constexpr VECTOR PLAYER_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //自身の当たり判定半径
    static constexpr float MY_COL_RADIUS = 66.0f * CHARACTER_SCALE;

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

    //PlayerBase(const InputManager::JOYPAD_NO _padNum) :padNum_(_padNum) {}
    //PlayerBase(const SceneManager::CNTL _cntl) :cntl_(_cntl) {}
    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void) = 0;
    void Init(void)override;
    virtual void Update(void)override;
    virtual void Draw(void)override;





    //コントローラー変更用関数
    void ChangeControll(SceneManager::CNTL _cntl);

    //回避関連
   //---------------------------------------
    const bool IsDodge(void)const { return 0.0f < dodgeCnt_ && dodgeCnt_ < FRAME_DODGE_MAX; }

    //-------------------------------------------------------------
    //ダメージ関数
    void Damage(void);

    //リセット
    void Reset(void);

    //移動関連
     //-------------------------------------
    //移動処理
    void Move(float _deg, VECTOR _axis);

    //方向処理
    void Turn(float _deg, VECTOR _axis);

    //動いてるかどうか
    bool IsMove(void) { return moveSpeed_ > 0.0f; }

    //スキル使用可能かどうか
    bool IsSkillable(void) { return !IsAtkAction() && !IsDodge(); }
    //スキル変更処理
    void SkillChange(void);

    //*****************************************************
    //ゲッタ
    //*****************************************************
    //クール中
    const bool GetIsCool(ATK_ACT _act) { return isCool_[static_cast<int>(_act)]; }

    //ツールタイム
    const float GetCoolTime(ATK_ACT _act) { return coolTime_[static_cast<int>(_act)]; }

    //現在の使いたいスキル
    const ATK_ACT GetSkillNo(void) { return skillNo_; }

    //攻撃中判定
    const bool GetIsAtk(void){ return isAtk_; }

    //スキル中判定
    const bool GetIsSkill(void){ return isSkill_; }

    //前隙
    const float GetAtkStartCnt(void) { return atkStartCnt_; }

    //アニメーションステップ
    const float GetStepAnim(void) { return stepAnim_; }

    //回避
    PlayerDodge* GetDodge(void) { return dodge_; }


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

    //その他
    //------------------------------------------------------------------------------------
    //アニメーションステップ
    void SetStepAnim(const float _stepAnim) { stepAnim_ = _stepAnim; }

    //スピード
    void SetMoveSpeed(const float _speed) { moveSpeed_ = _speed; }








protected:
    //*************************************************
    // 列挙型
    //*************************************************



    enum class ATK_TYPE
    {
        NORMALATK
        , CHARGEATK
    };
    struct PlayerAtk
    {
        ATK_ACT act_;
        float atkStartCnt_;
        ATK atk_;
        float coolCnt_;
        float CoolTime_[static_cast<int>(ATK_ACT::MAX)];
        std::map<ATK_ACT, ATK>atkMax_;
        float coolTimeMax_[static_cast<int>(ATK_ACT::MAX)];             //クールタイム最大
        float atkStartTime_[static_cast<int>(ATK_ACT::MAX)];            //攻撃発生時間
        bool IsAtkStart(void){ return 0.0f < atkStartCnt_ && atkStartCnt_ <= atkStartTime_[static_cast<int>(act_)]; }
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
    float moveSpeed_;                                           //移動スピード
    float coolTime_[static_cast<int>(ATK_ACT::MAX)];            //それぞれのクールタイムカウント
    bool isCool_[static_cast<int>(ATK_ACT::MAX)];               //それぞれの攻撃使い終わりを格納する
    float multiHitInterval_;                                    //多段ヒットのダメージ間隔
    VECTOR userOnePos_;                                         //ユーザー1追従用の座標   
    VECTOR colPos_;                                             //プレイヤーの当たり判定座標

    bool isPush_;                                               //長押しスキル用のボタンを押しているかどうか  true:押している
    bool moveAble_;             //移動可能かを返す  true:移動可能
    bool isAtk_;                                                 //通常攻撃開始したかどうか
    bool isSkill_;                                                 //スキル開始したかどうか


 

    //それぞれの最大値セット用(攻撃の座標はローカル座標で設定してます)
    std::map<ATK_ACT, ATK>atkMax_;
    float coolTimeMax_[static_cast<int>(ATK_ACT::MAX)];             //クールタイム最大
    float atkStartTime_[static_cast<int>(ATK_ACT::MAX)];            //攻撃発生時間

    //コントローラー系
    InputManager::JOYPAD_NO padNum_;                //ゲームパッドの番号
    int leftStickX_;            //パッドのスティックのX角度  
    int leftStickY_;            //パッドのスティックのY角度
    float stickDeg_;            //パッドのスティックの角度

   //回避系
    float dodgeCnt_;            //ドッジカウント
    float dodgeCdt_;            //ドッジの後隙

 
    //*************************************************
    //メンバ関数
    //*************************************************
    VECTOR GetTargetVec(VECTOR _targetPos);


    ////プレイヤーがCPUかUSERか判別
    //SceneManager::PLAY_MODE mode_;
    ////モード変更しないけどデバッグしやすいようにする
    //std::map < SceneManager::PLAY_MODE, std::function<void(void)>>changeMode_;
    //std::function<void(void)>modeUpdate_;       //モードごとの処理

    std::map<SceneManager::CNTL, std::function<void(void)>>changeNmlActControll_;       //通常スキル
    std::function<void(void)>nmlActUpdate_;                                             //通常スキル更新
     //チャージ攻撃
    std::map<SceneManager::CNTL, std::function<void(void)>>changeChargeActCntl_;       //コントローラーごとのスキル変更
    std::function<void(void)>chargeActUpdate_;




    //アニメNo初期化
    void InitAnimNum(void);

    //役割ごとに違うアニメーションの初期化
    virtual void InitCharaAnim(void);

    //それぞれのアクションの初期化
    virtual void InitAct(void);

    //ほぼガード用
    virtual void ResetGuardCnt(void);


    void ChangeNmlAtk(void);
    void ChangeSkillOne(void);
    void ChangeSkillTwo(void);



 

    //USER関係
    //------------------------------------------------
    //ユーザーがいるときの更新
    void UserUpdate(void);

    ////デバッグしやすいようにチェンジ作る
    //void ChangeMode(SceneManager::PLAY_MODE _mode);
    //void ChangeUser(void);
    //void ChangeCpu(void);

    //操作系（キーボード)
    void KeyBoardControl(void);

    //ゲームパッド
    void GamePad(void);


    SceneManager::CNTL cntl_;
    std::map < SceneManager::CNTL, std::function<void(void)>> changeCntl_;//コントローラー変更用
    std::function<void(void)> cntlUpdate_;                                 //それぞれの更新

    ATK_ACT skillNo_;     //スキル変更用

 
    //プレイヤー(CPUとユーザー)共通処理
    //--------------------------------------------------
    //攻撃処理
    virtual void AtkFunc(void) = 0;
    //スキル1
    virtual void Skill1Func(void) = 0;
    //スキル2
    virtual void Skill2Func(void) = 0;

    //各アクションの共通処理
    void Action(void);

    //チャージなしの攻撃
    void NmlAct(void);
    //チャージなし攻撃(キーボード)
    void NmlActKeyBoard(void);
    //チャージなし攻撃(パッド)
    void NmlActPad(void);

    //変更(キーボードとゲームパッド)
    void ChangeNmlActControll(void);
    void ChangeNmlActKeyBoard(void);
    void ChangeNmlActPad(void);



  
 

    //コントローラー変更
    void ChangeGamePad(void);
    //キーボード変更
    void ChangeKeyBoard(void);


    //攻撃座標の同期
    void SyncActPos(ATK& _atk);

    //スキルごとの操作更新
    void ChangeSkillControll(ATK_ACT _skill);



 

    //攻撃
    //-------------------------------------
    //攻撃中かどうか(UnitBaseで修正予定)
    const bool IsAtkAction(void)const { return IsAtkStart() || atk_.IsAttack() || atk_.IsBacklash(); }

    //攻撃可能かどうか(true:可能)
    const bool IsAtkable(void)const { return!IsAtkAction() && !IsDodge(); }


    //回避関連
    //---------------------------------------
    //回避可能か
    //回避中かどうか

    const bool IsDodgeable(void)const { return !IsDodge() && !IsAtkAction() && !IsCoolDodge(); }
    //クールタイム中かどうか
    const bool IsCoolDodge(void)const { return dodgeCdt_ < DODGE_CDT_MAX; }

    //ドッジカウント初期化
    void ResetDodgeFrame(void) { dodgeCnt_ = 0.0f; }


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



    //とりあえずランダムに攻撃を決める
    void RandAct(void);

    //クールタイムのカウント
    void CoolTimeCnt(void);

  

private:

    //std::map<ACT_CNTL, std::function<void(void)>>changeActCntl_;        //アクションごとに返すボタンを変更
    //std::function<bool(void)>actCntlUpdate_;

    //回避機能
    PlayerDodge* dodge_;

#ifdef DEBUG_INPUT

    //******************************************************************************************
#endif // DEBUG_ON
    
    //アクションの発動条件
    void ProcessAct(void);

 

    //攻撃入力
    virtual void NmlAtkInit(void);
    //スキル入力
    virtual void SkillOneInit(void);
    virtual void SkillTwoInit(void);

};