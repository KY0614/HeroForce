#pragma once
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"
class PlayerBase :
    public UnitBase
{
public:
    // 移動スピード
    static constexpr float SPEED_MOVE = 10.0f;
    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    
protected:
    enum class DIR
    {
        FLONT
        ,BACK
        ,LEFT
        ,RIGHT
    };
    //移動処理
    void Move(DIR _dir);

    //移動操作（コントローラー）
    void KeyBoardControl(void);

    //方向処理
    void Turn(float deg, VECTOR axis);
    

    DIR dir_;

    //
private:
    //ポインタ
    Transform transform_;
};

