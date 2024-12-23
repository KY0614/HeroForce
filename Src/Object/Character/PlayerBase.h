#pragma once
#include<map>
#include<functional>
#include<variant>
#include"../Utility/AsoUtility.h"
#include"../Manager/SceneManager.h"
#include"../Manager/InputManager.h"
#include "../UnitBase.h"

#define DEBUG_ON
//#define DEBUG_COOL
class PlayerBase :
    public UnitBase
{
public:
#ifdef DEBUG_ON
    void InitDebug(void);
    //デバッグ用関数
    void DrawDebug(void);
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
    static constexpr float CALLED_MOVE_SPEED_SCALE = 3.0f;



    static constexpr float FRAME_DODGE_MAX = 1.0f * CHARACTER_SCALE;
    static constexpr float DODGE_CDT_MAX = 0.5f;

    static constexpr int MAX_HP = 100;

    //プレイヤー自身の当たり判定
    static constexpr VECTOR PLAYER_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //自身の当たり判定半径
    static constexpr float MY_COL_RADIUS = 66.0f * CHARACTER_SCALE;

    //CPU
    //--------------------------------------------------------
    //範囲関係
    static constexpr float SEARCH_RANGE = 800.0f * CHARACTER_SCALE;		//索敵判定の大きさ
    static constexpr float SEARCH_RADIUS = 400.0f;

    //プレイヤー追従範囲
    static constexpr float FOLLOW_PLAYER_RADIUS = 100.0f * CHARACTER_SCALE;

    //*************************************************
    //各アクション操作ボタン
    //*************************************************
    //移動
    static constexpr int MOVE_FRONT_KEY = KEY_INPUT_W;
    static constexpr int MOVE_LEFT_KEY = KEY_INPUT_A;
    static constexpr int MOVE_BACK_KEY = KEY_INPUT_S;
    static constexpr int MOVE_RIGHT_KEY = KEY_INPUT_D;

    //攻撃
    static constexpr int ATK_KEY = KEY_INPUT_E;
    static constexpr InputManager::JOYPAD_BTN ATK_BTN = InputManager::JOYPAD_BTN::RIGHT;

    //スキル
    static constexpr int SKILL_KEY = KEY_INPUT_Q;
    static constexpr InputManager::JOYPAD_BTN SKILL_BTN = InputManager::JOYPAD_BTN::TOP;

    //スキル変更キー
    static constexpr int SKILL_CHANGE_KEY = KEY_INPUT_J;
    static constexpr InputManager::JOYPAD_BTN SKILL_CHANGE_BTN = InputManager::JOYPAD_BTN::R_BUTTON;

    //回避
    static constexpr int DODGE_KEY = KEY_INPUT_N;
    static constexpr InputManager::JOYPAD_BTN DODGE_BTN = InputManager::JOYPAD_BTN::LEFT;



    PlayerBase(const SceneManager::PLAY_MODE _mode, const InputManager::JOYPAD_NO _padNum) :mode_(_mode), padNum_(_padNum) {}
    PlayerBase(const SceneManager::PLAY_MODE _mode, const SceneManager::CNTL _cntl) :mode_(_mode), cntl_(_cntl) {}
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void) = 0;
    void Init(void)override;
    virtual void Update(void)override;
    //virtual void UserUpdate(void);

    //Cpu処理
    virtual void CpuUpdate(void);
    virtual void Draw(void)override;



    enum class CPU_STATE
    {
        NORMAL			//通常
        , ATTACK		//攻撃
        , BREAK			//休憩
        , MAX
    };

    enum class ACT_CNTL
    {
        NONE    //何もしていないとき
        ,MOVE   //移動
        ,DODGE             //回避
        ,NMLATK            //通常攻撃    
        ,CHANGE_SKILL      //スキル切り替え
        ,NMLSKILL          //短押しスキル
        ,CHARGE_SKILL_DOWN  //長押しスキル(初期化用)
        ,CHARGE_SKILL_KEEP  //長押しスキル(押しっぱなし)
        ,CHARGE_SKILL_UP    //長押しスキル(離す)
    };


    //コントローラー変更用関数
    void ChangeControll(SceneManager::CNTL _cntl);

    //回避関連
   //---------------------------------------
    const  bool IsDodge(void) { return 0.0f < dodgeCnt_ && dodgeCnt_ < FRAME_DODGE_MAX; }

    //-------------------------------------------------------------
    //ダメージ関数
    void Damage(void);

    //ゲッタ
    //-----------------------------------------------

    //攻撃開始判定
    const float GetAtkStartRange(void) { return atkStartRange_; }

    //索敵判定
    const float GetSearchRange(void) { return searchRange_; }

    //プレイヤーに呼び出されたかどうか
    const bool GetIsCalledPlayer(void) { return isMove2CallPlayer_; }

    //状態変更
    void ChangeState(const CPU_STATE _state);

    //セッタ
    //---------------------------------------------------

    //CPUの状態セッタ
    void SetState(const CPU_STATE _state) { cpuState_ = _state; }

    //CPUの移動セッタ
    void SetIsMove(const bool _isMove) { isMove_ = _isMove; }

    //プレイヤーのモードゲッタ(CPUかUSERか)
    SceneManager::PLAY_MODE GetPlayMode(void) { return mode_; }

    //状態ゲッタ
    CPU_STATE GetState(void) { return cpuState_; }

    //敵サーチセッタ
    void SetisEnemySerch(const bool _isEnemySerch) { isEnemySerch_ = _isEnemySerch; }

    //追従対象をセット
    void SetTargetPos(const VECTOR _targetPos) { targetPos_ = _targetPos; }

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
    ATK_TYPE atkType_;                                           //タイプ変数
    ATK_TYPE atkTypes_[static_cast<int>(ATK_ACT::MAX)];          //攻撃のタイプ(チャージするかしないか)
    std::map<ATK_TYPE, std::function<void(void)>>changeAtkType_;//攻撃タイプ変更
    std::function<void(void)>atkTypeUpdate_;                    //攻撃タイプごとのアップデート
    bool isPush_;                                               //長押しスキル用のボタンを押しているかどうか  true:押している




    //操作管理用
    ACT_CNTL actCntl_;

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

    //範囲系
    int atkStartRange_;     //攻撃開始範囲
    float searchRange_;      //索敵範囲


    //CPU系
    CPU_STATE cpuState_;        //状態
    std::map < CPU_STATE, std::function<void(void)>> cpuStateChanges_;  //状態ごとの初期化
    std::function<void(void)> cpuStateUpdate_;                          //状態ごとの更新
    bool isMove_;               //動いているかどうか
    bool isCall_;               //プレイヤーに呼び出されたか
    bool isMove2CallPlayer_;    //強制呼び出され中か　true:呼び出されてプレイヤーまで移動中
    bool isEnemySerch_;         //敵をサーチしたかしてないか
    float calledMoveSpeed_;     //プレイヤーに呼び出されたときの加速用変数
    float moveStartDelay_;      //移動の初めを遅らせる
    bool moveAble_;             //移動可能かを返す  true:移動可能
    //*************************************************
    //メンバ関数
    //*************************************************
    VECTOR GetTargetVec(VECTOR _targetPos);


    //プレイヤーがCPUかUSERか判別
    SceneManager::PLAY_MODE mode_;
    //モード変更しないけどデバッグしやすいようにする
    std::map < SceneManager::PLAY_MODE, std::function<void(void)>>changeMode_;
    std::function<void(void)>modeUpdate_;       //モードごとの処理

    std::map<SceneManager::CNTL, std::function<void(void)>>changeNmlActControll_;              //通常スキル
    std::function<void(void)>nmlActUpdate_;                                             //通常スキル更新

    std::map<SceneManager::CNTL, std::function<void(void)>>changeChargeActCntl_;       //コントローラーごとのスキル変更
    std::function<void(void)>chargeActUpdate_;                                         //チャージ攻撃

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

    //デバッグしやすいようにチェンジ作る
    void ChangeMode(SceneManager::PLAY_MODE _mode);
    void ChangeUser(void);
    void ChangeCpu(void);

    //操作系（キーボード)
    void KeyBoardControl(void);

    //ゲームパッド
    void GamePad(void);


    SceneManager::CNTL cntl_;
    std::map < SceneManager::CNTL, std::function<void(void)>> changeCntl_;//コントローラー変更用
    std::function<void(void)> cntlUpdate_;                                 //それぞれの更新

    SKILL_NUM skillNo_;     //スキル変更用

    //CPU
    //-------------------------------------------------

    //CPUのアップデート
    void CpuActUpdate(ATK_ACT _act);

    //各役割の初期化
    void CpuChangeNml(void);
    void CpuChangeAtk(void);
    void CpuChangeBreak(void);

    //各状態の更新
    void CpuNmlUpdate(void);
    void CpuAtkUpdate(void);
    void CpuBreakUpdate(void);

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

    //どのアクションが入力されているかの確認
    bool CheckAct(ACT_CNTL _actCntl) { return actCntl_ == _actCntl ? true : false; }

    //チャージなしの攻撃
    void NmlAct(void);
    //チャージなし攻撃(キーボード)
    void NmlActKeyBoard(void);
    //チャージなし攻撃(パッド)
    void NmlActPad(void);

    //変更
    void ChangeNmlActControll(void);
    void ChangeNmlActKeyBoard(void);
    void ChangeNmlActPad(void);

    //短押し攻撃共通処理(攻撃カウントとか後隙とか)
    void NmlActCommon(void);

    //チャージ攻撃
    virtual void ChargeAct(void);
    //チャージ攻撃(キーボード)
    void ChargeActKeyBoard(void);
    //チャージ攻撃(パッド)
    void ChargeActPad(void);

    //Change関数
    //チャージ攻撃(キーボード)
    void ChangeChargeActKeyBoard(void);
    //チャージ攻撃(パッド)
    void ChangeChargeActPad(void);

    //変更点
    void ChangeChargeActControll(void);

    void ChangeAtkType(ATK_ACT _act);


    //攻撃タイプ変更
    void ChangeChargeAct(void);
    void ChangeNmlAct(void);

    //攻撃発生中フラグ
    bool IsAtkStart(void) { return 0.0f < atkStartCnt_ && atkStartCnt_ <= atkStartTime_[static_cast<int>(act_)]; }

    //攻撃発生したのを確認する
    bool IsFinishAtkStart(void) { return atkStartCnt_ > atkStartTime_[static_cast<int>(act_)]; }

    //コントローラー変更
    void ChangeGamePad(void);
    //キーボード変更
    void ChangeKeyBoard(void);


    //攻撃座標の同期
    void SyncActPos(ATK& _atk);

    //スキルごとの操作更新
    void ChangeSkillControll(SKILL_NUM _skill);

    //攻撃終わった後の初期化
    virtual void InitAtk(void);

    //移動関連
    //-------------------------------------
    //移動処理
    void Move(float _deg, VECTOR _axis);

    //CPU移動
    void CpuMove(VECTOR _targetPos);

    //方向処理
    void Turn(float _deg, VECTOR _axis);

    //動いてるかどうか
    bool IsMove(void) { return moveSpeed_ > 0.0f; }

    //攻撃
    //-------------------------------------
    //攻撃中かどうか(UnitBaseで修正予定)
    bool IsAtkAction(void) { return IsAtkStart() || atk_.IsAttack() || atk_.IsBacklash(); }

    //攻撃可能かどうか(true:可能)
    bool IsAtkable(void) { return!IsAtkAction() && !IsDodge(); }


    //回避関連
    //---------------------------------------
    //回避可能か
    //回避中かどうか

    bool IsDodgeable(void) { return !IsDodge() && !IsAtkAction() && !IsCoolDodge(); }
    //クールタイム中かどうか
    bool IsCoolDodge(void) { return dodgeCdt_ < DODGE_CDT_MAX; }
    void Dodge(void);

    //ドッジカウント初期化
    void ResetDodgeFrame(void) { dodgeCnt_ = 0.0f; }

    //スキル仮想関数
    //-----------------------------------------
    //スキル1の操作
    virtual void SkillOneControll(void);

    //スキル2の操作
    virtual void SkillTwoControll(void);

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
    //CPUの前の攻撃格納用
    ATK_ACT preAtk_;

    //休憩カウント
    float breakCnt_;

    //誰をターゲットにするか
    VECTOR targetPos_;

    //前について行ってたターゲットの保存用
    VECTOR preTargetPos_;

    //移動する角度
    float moveDeg_;

    std::map<ACT_CNTL, std::function<void(void)>>changeActCntl_;        //アクションごとに返すボタンを変更
    std::function<bool(void)>actCntlUpdate_;  

  

    void ProcessAct(void);

    //スキル変更処理
    void SkillChange(void);
   
};