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
    static constexpr int SKILL_ONE_NUM = 2;
    static constexpr int SKILL_TWO_NUM = 10;

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
    static constexpr float FRAME_SKILL1_DURATION = 0.5f;
    static constexpr float FRAME_SKILL2_DURATION = 1.5f;
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr float FRAME_DODGE_MAX = 1.0f*SCALE;
    //static constexpr float FRAME_DODGE_MAX = 0.2f;
    static constexpr float DODGE_CDT_MAX = 0.5f;
    static constexpr float FRAME_SKILL1 = 1.5f;
    static constexpr float FRAME_SKILL2 = 0.5f;

    static constexpr int MAX_HP = 100;
    //�U�����W
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,20.0f,20.0f };
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,20.0f,20.0f };
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,20.0f,0.0f };

    //�U���͈�
    static constexpr float COL_ATK = SCALE * 100.0f;
    static constexpr float COL_SKILL1 = SCALE * 150.0f;
    static constexpr float COL_SKILL2 = SCALE * 200.0f;




    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void);
    void Init(void)override;
    virtual void Update(void)override;
    virtual void Draw(void)override;
    
protected:

    ATK skill1_;
    ATK skill2_;



    struct ACT
    {
        ATK atk;
        ATK skill;
    };

    enum class STATE
    {
        IDLE
        ,MOVE
        ,ATK
        ,DODGE
    };

    enum class SKILL_NUM
    {
        ONE
        ,TWO
        ,MAX
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

    //�v���C���[(CPU�ƃ��[�U�[)���ʏ���
    void Common(void);

    //�f�o�b�O�p�֐�
    void DrawDebug(void);

    //�J�E���g�֐�
    void Count(float& _cnt)
    { 
        _cnt += SceneManager::GetInstance().GetDeltaTime(); 
    }


    //�e�A�N�V�����̋��ʏ���
    void Action(ATK& _act);

    //�U�����W�̓���
    void SyncActPos(ATK& _act,VECTOR _colPos);


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
    SKILL_NUM skillNo_;
    //�X�L��1
    virtual void Skill_1(void);

     //�X�L��2
     virtual void Skill_2(void);

     //�X�L��
     //virtual void Skill(void);
     /// <summary>
     /// �X�L�����t���O
     /// </summary>
     /// <param name="_frameSkillNo">�X�L���t���[���ő�l(���̓X�L��1��2)</param>
     /// <returns>�X�L����/�X�L�����łȂ�</returns>
     bool IsSkill1Action(void) { return skills_[SKILL_NUM::ONE].IsAttack() || skills_[SKILL_NUM::ONE].IsBacklash(); }
     bool IsSkill2Action(void) { return skills_[SKILL_NUM::TWO].IsAttack() || skills_[SKILL_NUM::TWO].IsBacklash(); }

     bool IsSkillAction(SKILL_NUM _num) { return skills_[_num].IsAttack() || skills_[_num].IsBacklash(); }
     //���ׂẴX�L�����g�p�����ǂ���
     bool IsSkillAll(void) { return IsSkillAction(SKILL_NUM::ONE) || IsSkillAction(SKILL_NUM::TWO); }


     //�X�L���N�[���^�C�����t���O
     bool IsSkillCool(void);

     //�X�L�����ƂɍĐ�����A�j���[�V���������߂�
     void SkillAnim(void);

     //�X�L����������
     std::map<SKILL_NUM,float> skillCnt_;

     //�X�L���N�[���^�C��
     std::map<SKILL_NUM,float> skillCdt_;

     std::string skillNum_;

     //�_���[�W�֐�
     void Damage(void);

     int leftStickX_;

     int leftStickY_;

     float stickDeg_;

     std::map<SKILL_NUM, ATK> skills_;





     //�f�o�b�O�p
     void InitDebug(void);
     unsigned int color_Col_;
     unsigned int color_Atk_;
     unsigned int color_skl1_;
     unsigned int color_skl2_;
   
private:
};

