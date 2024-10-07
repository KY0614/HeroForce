#pragma once
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"
class PlayerBase :
    public UnitBase
{
public:
    // 移動スピード
    static constexpr float SPEED_MOVE = 10.0f;
    static constexpr int FRAME_ATK_MAX = 100;
    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    
protected:
    //enum class DIR
    //{
    //    FLONT
    //    ,BACK
    //    ,LEFT
    //    ,RIGHT
    //};

    enum class ANIM
    {
        T_POSE
        ,IDLE
        ,WALK
        ,RUN
        ,RUN_R
        ,RUN_L
        ,JUMP
        ,HAPPY
        ,DAMAGE
        ,DEATH
        ,ATK
    };

    enum class STATE
    {
        IDLE
        ,MOVE
        ,ATK
    };
    //移動処理
    void Move(VECTOR _dir);

    //操作系（コントローラー）
    void KeyBoardControl(void);

    //方向処理
    void Turn(float deg, VECTOR axis);

    //攻撃処理
    bool IsAtk(void);
    void Attack(void);
    int frameAtk_;

    //移動フラグ
    bool isMove_;

    

    //
private:
    //ポインタ
    Transform transform_;
};

