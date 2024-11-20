#pragma once
#include"../Utility/AsoUtility.h"
#include"../Manager/SceneManager.h"
#include<map>
#include "../UnitBase.h"

#define DEBUG_ON
class PlayerBase:
    public UnitBase
{
public:
#ifdef DEBUG_ON
    void InitDebug(void);
    //�f�o�b�O�p�֐�
    void DrawDebug(void);
    unsigned int color_Col_;
    unsigned int color_Atk_;
    unsigned int color_skl1_;
    unsigned int color_skl2_;

#endif // DEBUG_ON
    //�f�o�b�O�p
   
    //�e�A�j���[�V�����ԍ�
    static constexpr int T_POSE_NUM = 64;
    static constexpr int IDLE_NUM = 36;
    static constexpr int WALK_NUM = 72;
    static constexpr int RUN_NUM = 49;
    static constexpr int DODGE_NUM = 28;
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_ONE_NUM = 2;
    static constexpr int SKILL_TWO_NUM = 10;
    static constexpr int DAMAGE_NUM = 35;
    static constexpr int DEATH_NUM = 23;

    //�A�j���[�V�����X�s�[�h
    static constexpr float SPEED_ANIM_IDLE = 20.0f;
    static constexpr float SPEED_ANIM_RUN = 80.0f;
    static constexpr float SPEED_ANIM_DODGE = 30.0f;
    static constexpr float SPEED_ANIM_ATK = 50.0f;

    //�U���̎�ނ̐�
    static constexpr int ATK_TOTAL = 3;


    // �ړ��X�s�[�h
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DEG = 5.0f;
    static constexpr float SPEED_DODGE = 15.0f;
    static constexpr float CALLED_MOVE_SPEED_SCALE = 3.0f;



    static constexpr float FRAME_DODGE_MAX = 1.0f*CHARACTER_SCALE;
    static constexpr float DODGE_CDT_MAX = 0.5f;

    static constexpr int MAX_HP = 100;
 
    //�v���C���[���g�̓����蔻��
    static constexpr VECTOR PLAYER_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //���g�̓����蔻�蔼�a
    static constexpr float MY_COL_RADIUS = 66.0f*CHARACTER_SCALE;

    //CPU
    //--------------------------------------------------------
    //�͈͊֌W
    static constexpr float SEARCH_RANGE = 800.0f * CHARACTER_SCALE;		//���G����̑傫��
    static constexpr float SEARCH_RADIUS = 400.0f;

    //�v���C���[�Ǐ]�͈�
    static constexpr float FOLLOW_PLAYER_RADIUS = 100.0f * CHARACTER_SCALE;

    PlayerBase(SceneManager::PLAY_MODE _mode) :mode_(_mode){}
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void);
    void Init(void)override;
   void Update(void)override;
   //virtual void UserUpdate(void);
   //Cpu����
   virtual void CpuUpdate(void);
    void Draw(void)override;

   

    enum class CPU_STATE
    {
        NORMAL			//�ʏ�
        , ATTACK		//�U��
        , BREAK			//�x�e
        , MAX
    };

   
    //�R���g���[���[�ύX�p�֐�
    void ChangeControll(SceneManager::CNTL _cntl);

    //

    //����֘A
   //---------------------------------------
   const  bool IsDodge(void) { return 0.0f < dodgeCnt_ && dodgeCnt_ < FRAME_DODGE_MAX; }

   //-------------------------------------------------------------
   //�_���[�W�֐�
   void Damage(void);

   //�Q�b�^
   //-----------------------------------------------

   //�U���J�n����
   const float GetAtkStartRange(void) { return atkStartRange_; }

   //���G����
   const float GetSearchRange(void) { return searchRange_; }

   //�v���C���[�ɌĂяo���ꂽ���ǂ���
   const bool GetIsCalledPlayer(void) { return isMove2CallPlayer_; }

   //��ԕύX
   void ChangeState(const CPU_STATE _state);

   //�Z�b�^
   //---------------------------------------------------

   //CPU�̏�ԃZ�b�^
   void SetState(const CPU_STATE _state) { cpuState_ = _state; }

   //CPU�̈ړ��Z�b�^
   void SetIsMove(const bool _isMove) { isMove_ = _isMove; }

   //�v���C���[�̃��[�h�Q�b�^(CPU��USER��)
   SceneManager::PLAY_MODE GetPlayMode(void) { return mode_; }

   //��ԃQ�b�^
   CPU_STATE GetState(void) { return cpuState_; }

   //�G�T�[�`�Z�b�^
   void SetisEnemySerch(const bool _isEnemySerch) { isEnemySerch_ = _isEnemySerch; }

   //�Ǐ]�Ώۂ��Z�b�g
   void SetTargetPos(const VECTOR _targetPos) { targetPos_ = _targetPos; }
    
protected:
   //*************************************************
   // �񋓌^
   //*************************************************

    //�U�����
    enum class ATK_ACT
    {
        ATK
        , SKILL1
        , SKILL2
        , MAX
    };

    //�X�L���ύX�p
    enum class SKILL_NUM
    {
        ONE = 1
        , TWO = 2
        , MAX
    };

    //*************************************************
    // �����o�ϐ�
    //*************************************************
    //�X�e�[�^�X�n
    VECTOR userOnePos_;                     //���[�U�[1�Ǐ]�p�̍��W   
    VECTOR colPos_;                         //�v���C���[�̓����蔻����W
    SceneManager::ROLE role_;               //����
    ATK_ACT act_;                           //�U�����
    float moveSpeed_;                       //�ړ��X�s�[�h
    float coolTime_[static_cast<int>(ATK_ACT::MAX)];      //���ꂼ��̃N�[���^�C���J�E���g
    bool isCool_[static_cast<int>(ATK_ACT::MAX)];         //���ꂼ��̍U���g���I�����i�[����
    //���ꂼ��̍ő�l�Z�b�g�p

    std::map<ATK_ACT,float> colRadius_;               //�����蔻��
    std::map<ATK_ACT, VECTOR> colLocalPos_;           //�U�����W
    std::map<ATK_ACT, float> dulationMax_;            //��������
    std::map<ATK_ACT, float> backLashMax_;            //�㌄
    float coolTimeMax_[static_cast<int>(ATK_ACT::MAX)];             //�N�[���^�C���ő�           //�N�[���^�C���ő�   
   
    //�R���g���[���[�n
    int padNo_;                 //�e�Q�[���p�b�h�̊��蓖�ėp
    int leftStickX_;            //�p�b�h�̃X�e�B�b�N��X�p�x  
    int leftStickY_;            //�p�b�h�̃X�e�B�b�N��Y�p�x
    float stickDeg_;            //�p�b�h�̃X�e�B�b�N�̊p�x

   //����n
    float dodgeCnt_;            //�h�b�W�J�E���g
    float dodgeCdt_;            //�h�b�W�̌㌄

    //�͈͌n
    int atkStartRange_;     //�U���J�n�͈�
    float searchRange_;      //���G�͈�

 
    //CPU�n
    CPU_STATE cpuState_;        //���
    bool isMove_;              //�����Ă��邩�ǂ���
    bool isCall_;             //�v���C���[�ɌĂяo���ꂽ��
    bool isMove2CallPlayer_;  //�����Ăяo���ꒆ���@true:�Ăяo����ăv���C���[�܂ňړ���
    bool isEnemySerch_;         //�G���T�[�`���������ĂȂ���
    float calledMoveSpeed_;   //�v���C���[�ɌĂяo���ꂽ�Ƃ��̉����p�ϐ�
    float moveStartDelay_;      //�ړ��̏��߂�x�点��
    //*************************************************
    //  // �����o�֐�
    //*************************************************
   VECTOR GetTargetVec(VECTOR _targetPos);


    //�v���C���[��CPU��USER������
    SceneManager::PLAY_MODE mode_;

    //�A�j��No������
    void InitAnimNum(void);

    //���ꂼ��̃A�N�V�����̏�����
    virtual void InitAct(void);

    //�U���ύX�p
    void ChangeAtk(const ATK_ACT _act);

    //�U���̍ő�l�̏�����(�e����)
    void ResetParam(ATK_ACT _act);

    //USER�֌W
    //------------------------------------------------
    //���[�U�[������Ƃ��̍X�V
    void UserUpdate(void);

    //����n�i�L�[�{�[�h)
    void KeyBoardControl(void);

    //�Q�[���p�b�h
    void GamePad(void);

    
    SceneManager::CNTL cntl_;

    SKILL_NUM skillNo_;     //�X�L���ύX�p

    //CPU
    //-------------------------------------------------

    //CPU�̃A�b�v�f�[�g
    void ActUpdate(ATK_ACT _act);

    //�e��Ԃ̍X�V
    void NmlUpdate(void);
    void AtkUpdate(void);
    void BreakUpdate(void);


   

    //�v���C���[(CPU�ƃ��[�U�[)���ʏ���
    void Common(void);

    //�U������
    virtual void AtkFunc(void);
    //�X�L��1
    virtual void Skill1Func(void);
    //�X�L��2
    virtual void Skill2Func(void);

  
   
    //�e�A�N�V�����̋��ʏ���
    void Action(void);

    //�U�����W�̓���
    void SyncActPos(VECTOR _localPos);


    //�ړ��֘A
    //-------------------------------------
    //�ړ�����
    void Move(float _deg, VECTOR _axis);

    //CPU�ړ�
    void CpuMove(VECTOR _targetPos);
    //��������
    void Turn(float _deg, VECTOR _axis);

   

    //�����Ă邩�ǂ���
    bool IsMove(void) { return moveSpeed_ > 0.0f; }

   
    //�U��
    //-------------------------------------
    //�U�������ǂ���(UnitBase�ŏC���\��)
    bool IsAtkAction(void) { return atk_.IsAttack() || atk_.IsBacklash(); }

    //�U���\���ǂ���(true:�\)
    bool IsAtkable(void) { return!IsAtkAction()&& !IsDodge(); }

    
    //����֘A
    //---------------------------------------
    //����\��
    //��𒆂��ǂ���

    bool IsDodgeable(void) { return !IsDodge() && !IsAtkAction() && !IsCoolDodge(); }
    //�N�[���^�C�������ǂ���
    bool IsCoolDodge(void){return dodgeCdt_ < DODGE_CDT_MAX;}
    void Dodge(void);

    //�h�b�W�J�E���g������
    void ResetDodgeFrame(void){dodgeCnt_ = 0.0f; }

   
    

    //�X�L�����z�֐�
    //-----------------------------------------
   
    //�X�L��1
    virtual void Skill_One(void);

     //�X�L��2
     virtual void Skill_Two(void);

     //�X�L��
     //virtual void Skill(void);
     /// <summary>
     /// �X�L�����t���O
     /// </summary>
     /// <param name="_frameSkillNo">�X�L���t���[���ő�l(���̓X�L��1��2)</param>
     /// <returns>�X�L����/�X�L�����łȂ�</returns>

     bool IsSkillAction(SKILL_NUM _num) { return atk_.IsAttack() 
           || atk_.IsBacklash(); }
     
     //�X�L���g�p�\���ǂ���
     bool IsSkillable(void) { return !IsAtkAction()&& !IsDodge(); }


     //�X�L���N�[���^�C�����t���O
     bool IsSkillCool(void);

     //�X�L�����ƂɍĐ�����A�j���[�V���������߂�
     void SkillAnim(void);




     //�Ƃ肠���������_���ɍU�������߂�
     void RandAct(void);

     //�N�[���^�C���̃J�E���g
     void CoolTimeCnt(void);
    
private:
    //CPU�̑O�̍U���i�[�p
    ATK_ACT preAtk_;

    //�x�e�J�E���g
    float breakCnt_;

    //�N���^�[�Q�b�g�ɂ��邩
    VECTOR targetPos_;

    //�O�ɂ��čs���Ă��^�[�Q�b�g�̕ۑ��p
    VECTOR preTargetPos_;
};

