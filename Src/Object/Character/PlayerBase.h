#pragma once
#include"../Utility/AsoUtility.h"
#include"../Manager/SceneManager.h"
#include<map>
#include "../UnitBase.h"
class AxeMan;
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
    static constexpr float SPEED_ANIM_IDLE = 20.0f;
    static constexpr float SPEED_ANIM_RUN = 80.0f;
    static constexpr float SPEED_ANIM_DODGE = 30.0f;
    static constexpr float SPEED_ANIM_ATK = 50.0f;


    // �ړ��X�s�[�h
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DEG = 5.0f;
    static constexpr float SPEED_DODGE = 15.0f;

    //�e�U���̎�������
    static constexpr float FRAME_ATK_DURATION = 0.5f;
    static constexpr float FRAME_SKILL1_DURATION = 0.5f;
    static constexpr float FRAME_SKILL2_DURATION = 1.5f;

    //�㌄
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr float FRAME_SKILL1_BACKRASH = 0.2f;
    static constexpr float FRAME_SKILL2_BACKRASH = 0.2f;


    static constexpr float FRAME_DODGE_MAX = 1.0f*CHARACTER_SCALE;
    static constexpr float DODGE_CDT_MAX = 0.5f;

    static constexpr int MAX_HP = 100;
    //�U�����W
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //�v���C���[���g�̓����蔻��
    static constexpr VECTOR PLAYER_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //�U���͈�
    static constexpr float COL_ATK = CHARACTER_SCALE * 100.0f;
    static constexpr float COL_SKILL1 = CHARACTER_SCALE * 150.0f;
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 200.0f;
    //static constexpr float COL_ATK = SCALE * 200.0f;
    //���g�̓����蔻�蔼�a
    static constexpr float MY_COL_RADIUS = 20.0f;

    //CPU
    //--------------------------------------------------------
    //�͈͊֌W
    static constexpr float SEARCH_RANGE = 800.0f * CHARACTER_SCALE;		//���G����̑傫��
    static constexpr float SEARCH_RADIUS = 400.0f;



    //�v���C���[���[�h
    enum class PLAY_MODE
    {
        USER
        , CPU
        , MAX
    };

    enum class ROLE
    {
        KNIGHT
        , AXEMAN
        , MAGE
        , ARCHER
    };


    PlayerBase(PLAY_MODE _mode) :mode_(_mode){}
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void);
    void Init(void)override;
   void Update(void)override;
   virtual void UserUpdate(void);
   virtual void CpuUpdate(void);
    void Draw(void)override;

   

    enum class STATE
    {
        NORMAL			//�ʏ�
        , ATTACK		//�U��
        , BREAK			//�x�e
        , MAX
    };

   


    //

    //����֘A
   //---------------------------------------
   const  bool IsDodge(void) { return 0.0f < frameDodge_ && frameDodge_ < FRAME_DODGE_MAX; }

   //�_���[�W�֐�
   void Damage(void);

   //�U���J�n����
   float GetAtkStartRange(void) { return atkStartRange_; }

   //���G����
   float GetSearchRange(void) { return searchRange_; }

   //��ԕύX
   void ChangeState(const STATE _state);

   //CPU�̏�ԃZ�b�^
   void SetState(STATE _state) { state_ = _state; }

   //�f�o�b�O�p�֐�
   void DrawDebug(void);

   //CPU�̈ړ��Z�b�^
   void SetIsMove(bool _isMove) { isMove_ = _isMove; }

   //�v���C���[�̃��[�h�Q�b�^(CPU��USER��)
   PLAY_MODE GetPlayMode(void) { return mode_; }

   //��ԃQ�b�^
   STATE GetState(void) { return state_; }
    
protected:
    VECTOR GetTargetVec(void);
    VECTOR targetPos_;

    AxeMan* axeMan_;
    //���
    STATE state_;

    //����
    ROLE role_;

    ATK skill1_;
    ATK skill2_;

    //�U���J�n�͈�
    int atkStartRange_;

    //���G�͈�
    float searchRange_;		

    //�v���C���[��CPU��USER������
    PLAY_MODE mode_;

    enum class ATK_ACT
    {
        ATK
        , SKILL1
        , SKILL2
        , MAX
    };

    enum class SKILL_NUM
    {
        ONE = 1
        , TWO = 2
        ,MAX
    };

   

    enum class CNTL
    {
        KEYBOARD
        ,PAD
    };

    //�A�j��No������
    void InitAnimNum(void);

    //���ꂼ��̃A�N�V�����̏�����
    void InitAct(ATK_ACT _act,float _dulation,float _backlash);

    //USER�֌W
    //------------------------------------------------
    //���[�U�[������Ƃ��̍X�V
    void ModeUserUpdate(void);

    //����n�i�L�[�{�[�h)
    void KeyBoardControl(void);

    //�Q�[���p�b�h
    void GamePad(void);

    CNTL cntl_;

    //�R���g���[���[�ύX�p�֐�
    void ChangeControll(const CNTL _cntl);
    //------------------------------------------------

    //CPU
    //-------------------------------------------------

    //CPU�̃A�b�v�f�[�g
    void ModeCpuUpdate(void);

    void ActUpdate(ATK_ACT _act);


    virtual void AtkFunc(void);
    //�X�L��1
    virtual void Skill1Func(void);

    //�X�L��2
    virtual void Skill2Func(void);

   

    //�v���C���[(CPU�ƃ��[�U�[)���ʏ���
    void Common(void);

   

    //�J�E���g�֐�
    void Count(float& _cnt)
    { 
         _cnt += SceneManager::GetInstance().GetDeltaTime(); 
    }


    //�e�A�N�V�����̋��ʏ���
    void Action(ATK_ACT _act);

    //�U�����W�̓���
    void SyncActPos(ATK& _act,const VECTOR _colPos);


    //�ړ��֘A
    //-------------------------------------
    //�ړ�����
    void Move(float _deg, VECTOR _axis);

    //CPU�ړ�
    void CpuMove(void);
    //��������
    void Turn(float _deg, VECTOR _axis);

    //�ړ��X�s�[�h
    float moveSpeed_;

    //�����Ă邩�ǂ���
    bool IsMove(void) { return moveSpeed_ > 0.0f; }

    //CPU�p
    bool isMove_;

    //�v���C���[�̓����蔻����W
    VECTOR colPos_;

    //�e�Q�[���p�b�h�̊��蓖�ėp
    int padNo_;

    


    //�U��
    //-------------------------------------
    //�U�������ǂ���(UnitBase�ŏC���\��)
    bool IsAtkAction(void) { return acts_[ATK_ACT::ATK].IsAttack() || acts_[ATK_ACT::ATK].IsBacklash(); }

    //�U���\���ǂ���(true:�\)
    bool IsAtkable(void) { return!IsAtkAction() && !IsSkillAll() && !IsDodge(); }

    
    //����֘A
    //---------------------------------------
    //����\��
    //��𒆂��ǂ���

    bool IsDodgeable(void) { return !IsDodge() && !IsAtkAction() && !IsCoolDodge(); }
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

     bool IsSkillAction(SKILL_NUM _num) { return acts_[static_cast<ATK_ACT>(_num)].IsAttack() 
           || acts_[static_cast<ATK_ACT>(_num)].IsBacklash(); }
     //���ׂẴX�L�����g�p�����ǂ���
     bool IsSkillAll(void) { return IsSkillAction(SKILL_NUM::ONE) || IsSkillAction(SKILL_NUM::TWO); }

     //�X�L���g�p�\���ǂ���
     bool IsSkillable(void) { return !IsAtkAction() && !IsSkillAll() && !IsDodge(); }


     //�X�L���N�[���^�C�����t���O
     bool IsSkillCool(void);

     //�X�L�����ƂɍĐ�����A�j���[�V���������߂�
     void SkillAnim(void);


     int leftStickX_;

     int leftStickY_;

     float stickDeg_;

     std::map<SKILL_NUM, ATK> skills_;

     std::map<ATK_ACT, ATK> acts_;

     //�Ƃ肠���������_���ɍU�������߂�
     void RandAct(void);

     //�e��Ԃ̍X�V
     void NmlUpdate(void);
     void AtkUpdate(void);
     void BreakUpdate(void);






     //�f�o�b�O�p
     void InitDebug(void);
     unsigned int color_Col_;
     unsigned int color_Atk_;
     unsigned int color_skl1_;
     unsigned int color_skl2_;
   
private:
    ATK_ACT preAtk_;

    float breakCnt_;
};

