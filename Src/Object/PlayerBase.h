#pragma once
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"
class PlayerBase :
    public UnitBase
{
public:
    // �ړ��X�s�[�h
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
    //���̂̐F
    unsigned int color_;
    //����n�i�R���g���[���[�j
    void KeyBoardControl(void);

    //�f�o�b�O�p�֐�
    void DrawDebug(void);





    //�ړ��֘A
    //-------------------------------------
    //�ړ�����
    void Move(float _deg, VECTOR _axis);
    //��������
    void Turn(float deg, VECTOR axis);


    //�U��
    //-------------------------------------
    //�U�����t���O
    bool IsAtk(void){return frameAtk_ < FRAME_ATK_MAX;}

    //�U������
    void Attack(void);

    //�U���t���[��
    int frameAtk_;
    
    //����֘A
    //---------------------------------------
    bool IsDodge(void) { return frameDodge_ < FRAME_DODGE_MAX&& dodgeCdt_ > DODGE_CDT_MAX;}
    void Dodge(void);

    int frameDodge_;
    int dodgeCdt_;

    

    //
private:
    //�|�C���^
    Transform transform_;
};

