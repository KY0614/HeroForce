#pragma once
#include"../Utility/AsoUtility.h"
#include"../Manager/SceneManager.h"
#include<map>
#include "../UnitBase.h"

#define DEBUG_ON
class PlayerBase:
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
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_ONE_NUM = 2;
    static constexpr int SKILL_TWO_NUM = 10;
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



    static constexpr float FRAME_DODGE_MAX = 1.0f*CHARACTER_SCALE;
    static constexpr float DODGE_CDT_MAX = 0.5f;

    static constexpr int MAX_HP = 100;
 
    //プレイヤー自身の当たり判定
    static constexpr VECTOR PLAYER_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //自身の当たり判定半径
    static constexpr float MY_COL_RADIUS = 66.0f*CHARACTER_SCALE;

    //CPU
    //--------------------------------------------------------
    //範囲関係
    static constexpr float SEARCH_RANGE = 800.0f * CHARACTER_SCALE;		//索敵判定の大きさ
    static constexpr float SEARCH_RADIUS = 400.0f;

    //プレイヤー追従範囲
    static constexpr float FOLLOW_PLAYER_RADIUS = 100.0f * CHARACTER_SCALE;

    PlayerBase(SceneManager::PLAY_MODE _mode) :mode_(_mode){}
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void);
    void Init(void)override;
   void Update(void)override;
   //virtual void UserUpdate(void);
   //Cpu処理
   virtual void CpuUpdate(void);
    void Draw(void)override;

   

    enum class CPU_STATE
    {
        NORMAL			//通常
        , ATTACK		//攻撃
        , BREAK			//休憩
        , MAX
    };

   
    //コントローラー変更用関数
    void ChangeControll(SceneManager::CNTL _cntl);

    //

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

    //*************************************************
    // メンバ変数
    //*************************************************
    //ステータス系
    VECTOR userOnePos_;                     //ユーザー1追従用の座標   
    VECTOR colPos_;                         //プレイヤーの当たり判定座標
    SceneManager::ROLE role_;               //役割
    ATK_ACT act_;                           //攻撃種類
    float moveSpeed_;                       //移動スピード
    float coolTime_[static_cast<int>(ATK_ACT::MAX)];      //それぞれのクールタイムカウント
    bool isCool_[static_cast<int>(ATK_ACT::MAX)];         //それぞれの攻撃使い終わりを格納する
    //それぞれの最大値セット用

    std::map<ATK_ACT,float> colRadius_;               //当たり判定
    std::map<ATK_ACT, VECTOR> colLocalPos_;           //攻撃座標
    std::map<ATK_ACT, float> dulationMax_;            //持続時間
    std::map<ATK_ACT, float> backLashMax_;            //後隙
    float coolTimeMax_[static_cast<int>(ATK_ACT::MAX)];             //クールタイム最大           //クールタイム最大   
   
    //コントローラー系
    int padNo_;                 //各ゲームパッドの割り当て用
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
    bool isMove_;              //動いているかどうか
    bool isCall_;             //プレイヤーに呼び出されたか
    bool isMove2CallPlayer_;  //強制呼び出され中か　true:呼び出されてプレイヤーまで移動中
    bool isEnemySerch_;         //敵をサーチしたかしてないか
    float calledMoveSpeed_;   //プレイヤーに呼び出されたときの加速用変数
    float moveStartDelay_;      //移動の初めを遅らせる
    //*************************************************
    //  // メンバ関数
    //*************************************************
   VECTOR GetTargetVec(VECTOR _targetPos);


    //プレイヤーがCPUかUSERか判別
    SceneManager::PLAY_MODE mode_;

    //アニメNo初期化
    void InitAnimNum(void);

    //それぞれのアクションの初期化
    virtual void InitAct(void);

    //攻撃変更用
    void ChangeAtk(const ATK_ACT _act);

    //攻撃の最大値の初期化(各役割)
    void ResetParam(ATK_ACT _act);

    //USER関係
    //------------------------------------------------
    //ユーザーがいるときの更新
    void UserUpdate(void);

    //操作系（キーボード)
    void KeyBoardControl(void);

    //ゲームパッド
    void GamePad(void);

    
    SceneManager::CNTL cntl_;

    SKILL_NUM skillNo_;     //スキル変更用

    //CPU
    //-------------------------------------------------

    //CPUのアップデート
    void ActUpdate(ATK_ACT _act);

    //各状態の更新
    void NmlUpdate(void);
    void AtkUpdate(void);
    void BreakUpdate(void);


   

    //プレイヤー(CPUとユーザー)共通処理
    void Common(void);

    //攻撃処理
    virtual void AtkFunc(void);
    //スキル1
    virtual void Skill1Func(void);
    //スキル2
    virtual void Skill2Func(void);

  
   
    //各アクションの共通処理
    void Action(void);

    //攻撃座標の同期
    void SyncActPos(VECTOR _localPos);


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
    bool IsAtkAction(void) { return atk_.IsAttack() || atk_.IsBacklash(); }

    //攻撃可能かどうか(true:可能)
    bool IsAtkable(void) { return!IsAtkAction()&& !IsDodge(); }

    
    //回避関連
    //---------------------------------------
    //回避可能か
    //回避中かどうか

    bool IsDodgeable(void) { return !IsDodge() && !IsAtkAction() && !IsCoolDodge(); }
    //クールタイム中かどうか
    bool IsCoolDodge(void){return dodgeCdt_ < DODGE_CDT_MAX;}
    void Dodge(void);

    //ドッジカウント初期化
    void ResetDodgeFrame(void){dodgeCnt_ = 0.0f; }

   
    

    //スキル仮想関数
    //-----------------------------------------
   
    //スキル1
    virtual void Skill_One(void);

     //スキル2
     virtual void Skill_Two(void);

     //スキル
     //virtual void Skill(void);
     /// <summary>
     /// スキル中フラグ
     /// </summary>
     /// <param name="_frameSkillNo">スキルフレーム最大値(今はスキル1か2)</param>
     /// <returns>スキル中/スキル中でない</returns>

     bool IsSkillAction(SKILL_NUM _num) { return atk_.IsAttack() 
           || atk_.IsBacklash(); }
     
     //スキル使用可能かどうか
     bool IsSkillable(void) { return !IsAtkAction()&& !IsDodge(); }


     //スキルクールタイム中フラグ
     bool IsSkillCool(void);

     //スキルごとに再生するアニメーションを決める
     void SkillAnim(void);




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
};

