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

    //アニメーションスピード
    static constexpr float SPEED_ANIM_IDLE = 20.0f;
    static constexpr float SPEED_ANIM_RUN = 80.0f;
    static constexpr float SPEED_ANIM_DODGE = 30.0f;
    static constexpr float SPEED_ANIM_ATK = 50.0f;


    // 移動スピード
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DEG = 5.0f;
    static constexpr float SPEED_DODGE = 15.0f;

    //各攻撃の持続時間
    static constexpr float FRAME_ATK_DURATION = 0.5f;
    static constexpr float FRAME_SKILL1_DURATION = 0.5f;
    static constexpr float FRAME_SKILL2_DURATION = 1.5f;

    //後隙
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr float FRAME_SKILL1_BACKRASH = 0.2f;
    static constexpr float FRAME_SKILL2_BACKRASH = 0.2f;


    static constexpr float FRAME_DODGE_MAX = 1.0f*CHARACTER_SCALE;
    static constexpr float DODGE_CDT_MAX = 0.5f;

    static constexpr int MAX_HP = 100;
    //攻撃座標
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //プレイヤー自身の当たり判定
    static constexpr VECTOR PLAYER_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //攻撃範囲
    static constexpr float COL_ATK = CHARACTER_SCALE * 100.0f;
    static constexpr float COL_SKILL1 = CHARACTER_SCALE * 150.0f;
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 200.0f;
    //static constexpr float COL_ATK = SCALE * 200.0f;
    //自身の当たり判定半径
    static constexpr float MY_COL_RADIUS = 20.0f;

    //CPU
    //--------------------------------------------------------
    //範囲関係
    static constexpr float SEARCH_RANGE = 800.0f * CHARACTER_SCALE;		//索敵判定の大きさ
    static constexpr float SEARCH_RADIUS = 400.0f;

    //プレイヤー追従範囲
    static constexpr float FOLLOW_PLAYER_RADIUS = 100.0f * CHARACTER_SCALE;



    //プレイヤーモード
    //SceneManangerに移動
    enum class PLAY_MODE
    {
        USER
        , CPU
        , MAX
    };


    //SceneManangerに移動
    enum class ROLE
    {
        KNIGHT
        , AXEMAN
        , MAGE
        , ARCHER
    };


    PlayerBase(PLAY_MODE _mode) :mode_(_mode){}
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void);
    void Init(void)override;
   void Update(void)override;
   //virtual void UserUpdate(void);
   virtual void CpuUpdate(void);
    void Draw(void)override;

   

    enum class STATE
    {
        NORMAL			//通常
        , ATTACK		//攻撃
        , BREAK			//休憩
        , MAX
    };

   


    //

    //回避関連
   //---------------------------------------
   const  bool IsDodge(void) { return 0.0f < frameDodge_ && frameDodge_ < FRAME_DODGE_MAX; }

   //ダメージ関数
   void Damage(void);

   //攻撃開始判定
   const float GetAtkStartRange(void) { return atkStartRange_; }

   //索敵判定
   const float GetSearchRange(void) { return searchRange_; }

   //状態変更
   void ChangeState(const STATE _state);

   //CPUの状態セッタ
   void SetState(const STATE _state) { state_ = _state; }

   //デバッグ用関数
   void DrawDebug(void);

   //CPUの移動セッタ
   void SetIsMove(const bool _isMove) { isMove_ = _isMove; }

   //プレイヤーのモードゲッタ(CPUかUSERか)
   PLAY_MODE GetPlayMode(void) { return mode_; }

   //状態ゲッタ
   STATE GetState(void) { return state_; }

   //敵サーチセッタ
   void SetisEnemySerch(const bool _isEnemySerch) { isEnemySerch_ = _isEnemySerch; }

   //追従対象をセット
   void SetTargetPos(const VECTOR _targetPos) { targetPos_ = _targetPos; }
    
protected:
   VECTOR GetTargetVec(VECTOR _targetPos);

    //ユーザー1追従用の座標
    VECTOR userOnePos_;

    //敵をサーチしたかしてないか
    bool isEnemySerch_;

    //状態
    STATE state_;

    //役割
    ROLE role_;

    ATK skill1_;
    ATK skill2_;

    //攻撃開始範囲
    int atkStartRange_;

    //索敵範囲
    float searchRange_;		

    //プレイヤーがCPUかUSERか判別
    PLAY_MODE mode_;

    enum class ATK_ACT
    {
        ATK
        , SKILL1
        , SKILL2
        , MAX
    };

    enum class SKILL_NUM
    {
        ONE = 1
        , TWO = 2
        ,MAX
    };

   

    enum class CNTL
    {
        KEYBOARD
        ,PAD
    };

    //アニメNo初期化
    void InitAnimNum(void);

    //それぞれのアクションの初期化
    void InitAct(ATK_ACT _act,float _dulation,float _backlash);

    //攻撃変更用
    void ChangeAtk(const ATK_ACT _act);

    //USER関係
    //------------------------------------------------
    //ユーザーがいるときの更新
    void UserUpdate(void);

    //操作系（キーボード)
    void KeyBoardControl(void);

    //ゲームパッド
    void GamePad(void);

    CNTL cntl_;

    //コントローラー変更用関数
    void ChangeControll(const CNTL _cntl);
    //------------------------------------------------

    //CPU
    //-------------------------------------------------

    //CPUのアップデート

    void ActUpdate(ATK_ACT _act);


    virtual void AtkFunc(void);
    //スキル1
    virtual void Skill1Func(void);

    //スキル2
    virtual void Skill2Func(void);

   

    //プレイヤー(CPUとユーザー)共通処理
    void Common(void);

   

    //カウント関数
    void Count(float& _cnt)
    { 
        _cnt += SceneManager::GetInstance().GetDeltaTime(); 
    }


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

    //移動スピード
    float moveSpeed_;

    //動いてるかどうか
    bool IsMove(void) { return moveSpeed_ > 0.0f; }

    //CPU用
    bool isMove_;

    //プレイヤーの当たり判定座標
    VECTOR colPos_;

    //各ゲームパッドの割り当て用
    int padNo_;

    


    //攻撃
    //-------------------------------------
    //攻撃中かどうか(UnitBaseで修正予定)
    bool IsAtkAction(void) { return atk_.IsAttack() || atk_.IsBacklash(); }

    //攻撃可能かどうか(true:可能)
    bool IsAtkable(void) { return!IsAtkAction() && !IsSkillAll() && !IsDodge(); }

    
    //回避関連
    //---------------------------------------
    //回避可能か
    //回避中かどうか

    bool IsDodgeable(void) { return !IsDodge() && !IsAtkAction() && !IsCoolDodge(); }
    //クールタイム中かどうか
    bool IsCoolDodge(void){return dodgeCdt_ < DODGE_CDT_MAX;}
    void Dodge(void);

    //ドッジカウント初期化
    void ResetDodgeFrame(void){frameDodge_ = 0.0f; }

    float frameDodge_;
    float dodgeCdt_;

    

    //スキル仮想関数
    //-----------------------------------------
    SKILL_NUM skillNo_;
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
     //すべてのスキルが使用中かどうか
     bool IsSkillAll(void) { return IsSkillAction(SKILL_NUM::ONE) || IsSkillAction(SKILL_NUM::TWO); }

     //スキル使用可能かどうか
     bool IsSkillable(void) { return !IsAtkAction() && !IsSkillAll() && !IsDodge(); }


     //スキルクールタイム中フラグ
     bool IsSkillCool(void);

     //スキルごとに再生するアニメーションを決める
     void SkillAnim(void);


     int leftStickX_;

     int leftStickY_;

     float stickDeg_;

     std::map<SKILL_NUM, ATK> skills_;

     ATK_ACT act_;

     //とりあえずランダムに攻撃を決める
     void RandAct(void);

     //各状態の更新
     void NmlUpdate(void);
     void AtkUpdate(void);
     void BreakUpdate(void);






     
private:
    ATK_ACT preAtk_;

    float breakCnt_;

    VECTOR targetPos_;
};

