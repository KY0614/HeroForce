#pragma once
#include"../Utility/AsoUtility.h"
#include"../Manager/SceneManager.h"
#include<map>
#include "UnitBase.h"
class PlayerBase:
    public UnitBase
{
public:
    //�e�A�j���[�V�����ԍ�
    static constexpr int IDLE_NUM = 36;
    static constexpr int WALK_NUM = 72;
    static constexpr int RUN_NUM = 49;
    static constexpr int DODGE_NUM = 28;
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_NUM = 2;

    //�A�j���[�V�����X�s�[�h
    static constexpr int SPEED_ANIM_IDLE = 20.0f;
    static constexpr int SPEED_ANIM_RUN = 80.0f;
    static constexpr int SPEED_ANIM_DODGE = 30.0f;
    static constexpr int SPEED_ANIM_ATK = 50.0f;

    static constexpr float SCALE = 0.2f;


    // �ړ��X�s�[�h
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DEG = 5.0f;
    static constexpr float SPEED_DODGE = 15.0f;
    static constexpr float FRAME_ATK_DURATION = 0.5f;
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr float FRAME_DODGE_MAX = 0.43f;
    static constexpr float DODGE_CDT_MAX = 0.5f;
    static constexpr float FRAME_SKILL1 = 1.5f;
    static constexpr float FRAME_SKILL2 = 0.5f;

    static constexpr int MAX_HP = 100;
    //�U�����W
    static constexpr VECTOR COL_LOCAL_POS = { 0.0f,20.0f,0.0f };

    //�U���͈�
    static constexpr float COL_ATK = SCALE * 200.0f;




    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void);
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

    enum class CNTL
    {
        KEYBOARD
        ,PAD
    };

    //�A�j��No������
    void InitAnimNum(void);

    //����n�i�L�[�{�[�h)
    void KeyBoardControl(void);

    //�Q�[���p�b�h
    void GamePad(void);

    CNTL cntl_;

    //�R���g���[���[�ύX�p�֐�
    void ChangeControll(CNTL _cntl);

    //�f�o�b�O�p�֐�
    void DrawDebug(void);

    //�J�E���g�֐�
    void Count(float& _cnt)
    { 
        _cnt += SceneManager::GetInstance().GetDeltaTime(); 
    }


   



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
    bool IsAtkAction(void) { return atk_.IsAttack() || atk_.IsBacklash(); }


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
     bool IsSkill(SKILL _skill) { return skillCnt_[_skill]>0.0f && skillCnt_[_skill] < FRAME_SKILL1; }

     //�X�L���N�[���^�C�����t���O
     bool IsSkillCool(void);

     //�X�L����������
     std::map<SKILL,float> skillCnt_;

     //�X�L���N�[���^�C��
     std::map<SKILL,float> skillCdt_;

     std::string skillNum_;

     //�_���[�W�֐�
     void Damage(void);

     int leftStickX_;

     int leftStickY_;

     float stickDeg_;







     //�f�o�b�O�p
     void InitDebug(void);
     unsigned int color_Col_;
     unsigned int color_Atk_;
   
private:
};

