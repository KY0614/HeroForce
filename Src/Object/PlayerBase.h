#pragma once
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"
class PlayerBase :
    public UnitBase
{
public:
    // 移動スピード
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DEG = 5.0f;
    static constexpr float SPEED_DODGE = 15.0f;
    static constexpr int FRAME_ATK_MAX = 100;
    static constexpr int FRAME_DODGE_MAX = 10;
    static constexpr int DODGE_CDT_MAX = 20;
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

    //デバッグ用関数
    void DrawDebug(void);





    //移動関連
    //-------------------------------------
    //移動処理
    void Move(float _deg, VECTOR _axis);
    //方向処理
    void Turn(float deg, VECTOR axis);


    //攻撃
    //-------------------------------------
    //攻撃中フラグ
    bool IsAtk(void){return frameAtk_ < FRAME_ATK_MAX;}

    //攻撃処理
    void Attack(void);

    //攻撃フレーム
    int frameAtk_;
    
    //回避関連
    //---------------------------------------
    bool IsDodge(void) { return frameDodge_ < FRAME_DODGE_MAX&& dodgeCdt_ > DODGE_CDT_MAX;}
    void Dodge(void);

    int frameDodge_;
    int dodgeCdt_;

    

    //
private:
    //ポインタ
    Transform transform_;
};

