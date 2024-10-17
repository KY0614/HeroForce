#pragma once
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"
class PlayerBase:
    public UnitBase
{
public:
    // �ړ��X�s�[�h
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DEG = 5.0f;
    static constexpr float SPEED_DODGE = 15.0f;
    static constexpr float FRAME_ATK_MAX = 0.0f;
    static constexpr float FRAME_ATK_DURATION = 40.0f;
    static constexpr int FRAME_DODGE_MAX = 10;
    static constexpr int DODGE_CDT_MAX = 20;
    static constexpr int FRAME_SKILL1 = 40;
    static constexpr int FRAME_SKILL2 = 30;
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
    void Turn(float _deg, VECTOR _axis);


    //�U��
    //-------------------------------------
    //�U�����t���O
    bool IsAtk(void){return atk_.cnt_ < FRAME_ATK_MAX;}

    //�U������
    void Attack(void);

    //�U���t���[��
    

    ATK atk_;
    
    //����֘A
    //---------------------------------------
    bool IsDodge(void) { return frameDodge_ < FRAME_DODGE_MAX; }
    //�N�[���^�C�������ǂ���
    bool IsCoolDodge(void){return dodgeCdt_ < DODGE_CDT_MAX;}
    void Dodge(void);

    int frameDodge_;
    int dodgeCdt_;

    

    //�X�L�����z�֐�
    //-----------------------------------------
    //�X�L��1
    virtual void Skill_1(void);

     //�X�L��2
     virtual void Skill_2(void);
     /// <summary>
     /// �X�L�����t���O
     /// </summary>
     /// <param name="_frameSkillNo">�X�L���t���[���ő�l(���̓X�L��1��2)</param>
     /// <returns>�X�L����/�X�L�����łȂ�</returns>
     bool IsSkill(int _frameSkillNo) { return frameSkill_ < _frameSkillNo; }

     //�X�L���N�[���^�C�����t���O
     bool IsSkillCool(void);

     //�X�L����������
     int frameSkill_;

     //�X�L���N�[���^�C��
     int skillCdt_;

     std::string skillNum_;

     //�Q�b�^�[
     //--------------------------------------------
     //�ʒu
     const VECTOR GetPos(void)const override { return trans_.pos; }
     //�p�x
     const VECTOR GetRot(void)const override { return trans_.rot; }
     //�傫��
     const VECTOR GetScl(void)const override { return trans_.scl; }
     //�h���
     const float GetDef(void)const override { return def_; };
private:
};

