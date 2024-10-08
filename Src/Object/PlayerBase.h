#pragma once
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"
class PlayerBase :
    public UnitBase
{
public:
    // 移動スピード
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DODGE = 10.0f;
    static constexpr int FRAME_ATK_MAX = 100;
    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    
protected:
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
        ,DODGE
    };
    //球体の色
    unsigned int color_;
    //操作系（コントローラー）
    void KeyBoardControl(void);





    //移動関連
    //-------------------------------------
    //移動処理
    void Move(VECTOR _dir);
    //方向処理
    void Turn(float deg, VECTOR axis);


    //攻撃
    //-------------------------------------
    //攻撃中フラグ
    bool IsAtk(void){return frameAtk_ > 0;}

    //攻撃処理
    void Attack(void);

    //攻撃フレーム
    int frameAtk_;
    
    //回避関連
    bool IsDodge(void) { return frameDodge_ > 0; }
    void Dodge(void);

    int frameDodge_;

    

    //
private:
    //ポインタ
    Transform transform_;
};

