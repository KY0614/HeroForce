#pragma once
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"
class PlayerBase :
    public UnitBase
{
public:
    // �ړ��X�s�[�h
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
    //�ړ�����
    void Move(VECTOR _dir);

    //����n�i�R���g���[���[�j
    void KeyBoardControl(void);

    //��������
    void Turn(float deg, VECTOR axis);

    //�U������
    bool IsAtk(void);
    void Attack(void);
    int frameAtk_;

    //�ړ��t���O
    bool isMove_;

    

    //
private:
    //�|�C���^
    Transform transform_;
};

