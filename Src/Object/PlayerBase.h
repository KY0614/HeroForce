#pragma once
#include"../Utility/AsoUtility.h"
#include<map>
#include "UnitBase.h"
class PlayerBase:
    public UnitBase
{
public:
    // �ړ��X�s�[�h
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DEG = 5.0f;
    static constexpr float SPEED_DODGE = 15.0f;
    static constexpr float FRAME_ATK_DURATION = 1.0f;
    static constexpr float FRAME_ATK_BACKRASH = 0.3f;
    static constexpr float FRAME_DODGE_MAX = 0.5f;
    static constexpr float DODGE_CDT_MAX = 0.5f;
    static constexpr float FRAME_SKILL1 = 0.5f;
    static constexpr float FRAME_SKILL2 = 0.5f;

    //�e�A�j���[�V�����ԍ�
    static constexpr int IDLE_NUM = 36;
    static constexpr int WALK_NUM = 72;
    static constexpr int RUN_NUM = 49;
    static constexpr int DODGE_NUM = 28;
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_NUM = 2;





    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
    
protected:

    enum class STATE
    {
        IDLE
        ,MOVE
        ,ATK
        ,DODGE
    };

    enum class SKILL
    {
        ONE
        ,TWO
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

    //�ړ��X�s�[�h
    float speedMove_;

    bool IsMove(void) { return speedMove_ > 0; }

    


    //�U��
    //-------------------------------------

    //�U������
    void Attack(void);
    bool IsAtkAction(void) { return atk_.IsAttack() || atk_.IsBacklash_(); }


    //�U���t���[��
    

    ATK atk_;
    
    //����֘A
    //---------------------------------------
    bool IsDodge(void) { return 0.0f<frameDodge_&&frameDodge_ < FRAME_DODGE_MAX; }
    //�N�[���^�C�������ǂ���
    bool IsCoolDodge(void){return dodgeCdt_ < DODGE_CDT_MAX;}
    void Dodge(void);

    //�h�b�W�J�E���g������
    void ResetDodgeFrame(void){frameDodge_ = 0.0f; }

    float frameDodge_;
    float dodgeCdt_;

    

    //�X�L�����z�֐�
    //-----------------------------------------
    SKILL skill_;
    //�X�L��1
    virtual void Skill_1(void);

     //�X�L��2
     virtual void Skill_2(void);
     /// <summary>
     /// �X�L�����t���O
     /// </summary>
     /// <param name="_frameSkillNo">�X�L���t���[���ő�l(���̓X�L��1��2)</param>
     /// <returns>�X�L����/�X�L�����łȂ�</returns>
     bool IsSkill(SKILL _skill) { return skillCnt_[_skill] < FRAME_SKILL1; }

     //�X�L���N�[���^�C�����t���O
     bool IsSkillCool(void);

     //�X�L����������
     std::map<SKILL,float> skillCnt_;

     //�X�L���N�[���^�C��
     std::map<SKILL,float> skillCdt_;

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

