#pragma once
#include "PlayerBase.h"
class PlayerCpu :
    public PlayerBase
{
public:
    //CPU
 //--------------------------------------------------------
 //範囲関係
    static constexpr float SEARCH_RANGE = 800.0f * CHARACTER_SCALE;		//索敵判定の大きさ
    static constexpr float SEARCH_RADIUS = 400.0f;

    //プレイヤー追従範囲
    static constexpr float FOLLOW_PLAYER_RADIUS = 100.0f * CHARACTER_SCALE;

    //呼び出されたときのスピード
    static constexpr float CALLED_MOVE_SPEED_SCALE = 3.0f;

    enum class CPU_STATE
    {
        NORMAL			//通常
        , ATTACK		//攻撃
        , BREAK			//休憩
        , MAX
    };

    //コンストラクタ＆デストラクタ
    PlayerCpu(void);
    ~PlayerCpu(void)=default;

    void SetParam(void)override;
    void Update(void)override;





    //状態変更
    void ChangeState(const CPU_STATE _state);

    //ゲッタ
   //-----------------------------------------------
   //攻撃開始判定
    const int GetAtkStartRange(void) { return atkStartRange_; }

    //索敵判定
    const float GetSearchRange(void) { return searchRange_; }

    //プレイヤーに呼び出されたかどうか
    const bool GetIsCalledPlayer(void) { return isMove2CallPlayer_; }

    //状態ゲッタ
    CPU_STATE GetState(void) { return cpuState_; }

    //セッタ
    //---------------------------------------------------
    //CPUの状態セッタ
    void SetState(const CPU_STATE _state) { cpuState_ = _state; }

    //CPUの移動セッタ
    void SetIsMove(const bool _isMove) { isMove_ = _isMove; }

  

    //敵サーチセッタ
    void SetisEnemySerch(const bool _isEnemySerch) { isEnemySerch_ = _isEnemySerch; }

    //追従対象をセット
    void SetTargetPos(const VECTOR _targetPos) { targetPos_ = _targetPos; }

    void CpuReset(void);

private:
    //*************************************************
    //メンバ関数
    //*************************************************
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


    //CPUの前の攻撃格納用
    ATK_ACT preAtk_;

    //休憩カウント
    float breakCnt_;



    //前について行ってたターゲットの保存用
    VECTOR preTargetPos_;


    //CPUのアップデート
    void CpuStateUpdate(void);
    void CpuActUpdate(ATK_ACT _act);

    //各役割の初期化
    void CpuChangeNml(void);
    void CpuChangeAtk(void);
    void CpuChangeBreak(void);

    //各状態の更新
    void CpuNmlUpdate(void);
    void CpuAtkUpdate(void);
    void CpuBreakUpdate(void);

    //CPU移動
    void CpuMove(VECTOR _targetPos);


};

