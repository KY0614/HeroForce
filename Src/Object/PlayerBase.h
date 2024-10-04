#pragma once
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"
class PlayerBase :
    public UnitBase
{
public:
    // �ړ��X�s�[�h
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
    //�ړ�����
    void Move(DIR _dir);

    //�ړ�����i�R���g���[���[�j
    void KeyBoardControl(void);

    //��������
    void Turn(float deg, VECTOR axis);
    

    DIR dir_;

    //
private:
    //�|�C���^
    Transform transform_;
};

