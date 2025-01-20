#pragma once
#include<map>
#include<functional>
#include<variant>
#include"../Utility/AsoUtility.h"
#include"../Manager/SceneManager.h"
#include"../Manager/InputManager.h"
#include"PlayerInput.h"
#include "../UnitBase.h"

//デバッグ
#define DEBUG_ON
//#define INPUT_DEBUG_ON
#define DEBUG_COOL
class PlayerBase :
    public UnitBase
{
public:
#ifdef DEBUG_ON
    void InitDebug(void);
    //デバッグ用関数
    virtual void DrawDebug(void);
    unsigned int color_Col_;
    unsigned int color_Atk_;
    unsigned int color_skl1_;
    unsigned int color_skl2_;

#endif // DEBUG_ON
    //デバッグ用

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

    static constexpr int MAX_HP = 100;

    //プレイヤー自身の当たり判定
    static constexpr VECTOR PLAYER_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //自身の当たり判定半径
    static constexpr float MY_COL_RADIUS = 66.0f * CHARACTER_SCALE;


    PlayerBase(const InputManager::JOYPAD_NO _padNum) :padNum_(_padNum) {}
    PlayerBase(const SceneManager::CNTL _cntl) :cntl_(_cntl) {}
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


protected:
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
    VECTOR userOnePos_;                                         //ユーザー1追従用の座標   
    VECTOR colPos_;                                             //プレイヤーの当たり判定座標
    ATK_ACT act_;                                               //攻撃種類
    std::map < ATK_ACT, std::function<void(void)>>changeAct_;   //攻撃の変更
    std::function<void(void)>actUpdate_;                        //攻撃ごとの更新処理
    float atkStartCnt_;                                         //攻撃が発生するまでのカウント
    float moveSpeed_;                                           //移動スピード
    float coolTime_[static_cast<int>(ATK_ACT::MAX)];            //それぞれのクールタイムカウント
    bool isCool_[static_cast<int>(ATK_ACT::MAX)];               //それぞれの攻撃使い終わりを格納する
    float multiHitInterval_;                                    //多段ヒットのダメージ間隔
    ATK_TYPE atkType_;                                          //タイプ変数
    ATK_TYPE atkTypes_[static_cast<int>(ATK_ACT::MAX)];         //攻撃のタイプ(チャージするかしないか)
    std::map<ATK_TYPE, std::function<void(void)>>changeAtkType_;//攻撃タイプ変更
    std::function<void(void)>atkTypeUpdate_;                    //攻撃タイプごとのアップデート
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

    float moveDeg_;             //移動方向

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

    //攻撃変更用
    void ChangeAct(const ATK_ACT _act);

    void ChangeNmlAtk(void);
    void ChangeSkillOne(void);
    void ChangeSkillTwo(void);



    //攻撃の最大値の初期化(各役割)
    void ResetParam(ATK& _atk);

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

    //短押し攻撃共通処理(攻撃カウントとか後隙とか)
    virtual void NmlActCommon(void);

    //短押し用atk更新
    void NmlAtkUpdate(void);


    //チャージ攻撃
    virtual void ChargeAct(void);
    //チャージ攻撃(キーボード)
    void ChargeActKeyBoard(void);
    //チャージ攻撃(パッド)
    void ChargeActPad(void);

    ////Change関数
    ////チャージ攻撃(キーボード)
    //void ChangeChargeActKeyBoard(void);
    ////チャージ攻撃(パッド)
    //void ChangeChargeActPad(void);

    ////変更点
    //void ChangeChargeActControll(void);

    //void ChangeAtkType(ATK_ACT _act);


    ////攻撃タイプ変更
    //void ChangeChargeAct(void);
    //void ChangeNmlAct(void);

    //攻撃発生中フラグ
    const bool IsAtkStart(void)const { return 0.0f < atkStartCnt_ && atkStartCnt_ <= atkStartTime_[static_cast<int>(act_)]; }

    //攻撃発生したのを確認する
    const bool IsFinishAtkStart(void)const { return atkStartCnt_ > atkStartTime_[static_cast<int>(act_)]; }

    //コントローラー変更
    void ChangeGamePad(void);
    //キーボード変更
    void ChangeKeyBoard(void);


    //攻撃座標の同期
    void SyncActPos(ATK& _atk);

    //スキルごとの操作更新
    void ChangeSkillControll(ATK_ACT _skill);

    //攻撃終わった後の初期化
    virtual void InitAtk(void);

    //移動関連
    //-------------------------------------
    //移動処理
    void Move(float _deg, VECTOR _axis);

    //方向処理
    void Turn(float _deg, VECTOR _axis);

    //動いてるかどうか
    bool IsMove(void) { return moveSpeed_ > 0.0f; }

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
    void Dodge(void);

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

    //スキル使用可能かどうか
    bool IsSkillable(void) { return !IsAtkAction() && !IsDodge(); }

    //とりあえずランダムに攻撃を決める
    void RandAct(void);

    //クールタイムのカウント
    void CoolTimeCnt(void);

private:

    //std::map<ACT_CNTL, std::function<void(void)>>changeActCntl_;        //アクションごとに返すボタンを変更
    //std::function<bool(void)>actCntlUpdate_;

#ifdef DEBUG_INPUT
//************************************************************************
//キーボードとパッドの共通各アクションボタン設定(テスト用で使ったので一応残しておきます)
//************************************************************************
//コントローラーのボタン番号(あくまでテスト用なのでprivateに置いておきます)
    static constexpr int RIGHT_BTN = PAD_INPUT_B;
    static constexpr int TOP_BTN = PAD_INPUT_X;
    static constexpr int R_BUTTON = PAD_INPUT_R;
    static constexpr int LEFT_BTN = PAD_INPUT_Y;
    //ボタン入力するための情報
    struct InputMapInfo
    {
        SceneManager::CNTL type;     //コントローラータイプ

        //ボタン番号
        //std::variant<int, InputManager::JOYPAD_IN_STATE> buttonId_;
        int buttonId_;
    };

    //パッドとキーボードの共通ボタンに名前を付ける
    using InputActionMap_t = std::map<std::string, std::vector<InputMapInfo>>;
    using InputType = SceneManager::CNTL;
    std::map<std::string, bool>currentInput_;                           //今押されてるボタンを返す
    std::map<std::string, bool>lastInput_;                              //前に押されたボタン
    InputActionMap_t inputActionMap_;                                   //アクションボタンをまとめて扱うための変数
    /// <summary>
    /// 入力チェック
    /// </summary>
    /// <param name="action">調べたいアクションの名前</param>
    /// <returns>true　押されてる　/　false　押されてない</returns>
    bool IsPressed(const std::string& action)const;

    //入力情報の更新
    void InputUpdate(void);
    //******************************************************************************************
#endif // DEBUG_ON
    //操作一元化
    void ProcessInput(void);
    
    //アクションの発動条件
    void ProcessAct(void);

    //スキル変更処理
    void SkillChange(void);

    //攻撃入力
    virtual void NmlAtkInit(void);
    //スキル入力
    virtual void SkillOneInit(void);
    virtual void SkillTwoInit(void);

};