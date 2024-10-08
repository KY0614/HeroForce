#pragma once
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"
class PlayerBase :
    public UnitBase
{
public:
    // �ړ��X�s�[�h
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
    //���̂̐F
    unsigned int color_;
    //����n�i�R���g���[���[�j
    void KeyBoardControl(void);





    //�ړ��֘A
    //-------------------------------------
    //�ړ�����
    void Move(VECTOR _dir);
    //��������
    void Turn(float deg, VECTOR axis);


    //�U��
    //-------------------------------------
    //�U�����t���O
    bool IsAtk(void){return frameAtk_ > 0;}

    //�U������
    void Attack(void);

    //�U���t���[��
    int frameAtk_;
    
    //����֘A
    bool IsDodge(void) { return frameDodge_ > 0; }
    void Dodge(void);

    int frameDodge_;

    

    //
private:
    //�|�C���^
    Transform transform_;
};

