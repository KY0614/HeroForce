#pragma once
#include<map>
#include<functional>
#include<variant>
#include"../Utility/AsoUtility.h"
#include"../Manager/SceneManager.h"
#include"../Manager/InputManager.h"
#include "../UnitBase.h"

#define DEBUG_ON
//#define DEBUG_COOL
class PlayerBase :
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



    static constexpr float FRAME_DODGE_MAX = 1.0f * CHARACTER_SCALE;
    static constexpr float DODGE_CDT_MAX = 0.5f;

    static constexpr int MAX_HP = 100;

    //�v���C���[���g�̓����蔻��
    static constexpr VECTOR PLAYER_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //���g�̓����蔻�蔼�a
    static constexpr float MY_COL_RADIUS = 66.0f * CHARACTER_SCALE;

    //CPU
    //--------------------------------------------------------
    //�͈͊֌W
    static constexpr float SEARCH_RANGE = 800.0f * CHARACTER_SCALE;		//���G����̑傫��
    static constexpr float SEARCH_RADIUS = 400.0f;

    //�v���C���[�Ǐ]�͈�
    static constexpr float FOLLOW_PLAYER_RADIUS = 100.0f * CHARACTER_SCALE;

    //*************************************************
    //�e�A�N�V��������{�^��
    //*************************************************
    //�ړ�
    static constexpr int MOVE_FRONT_KEY = KEY_INPUT_W;
    static constexpr int MOVE_LEFT_KEY = KEY_INPUT_A;
    static constexpr int MOVE_BACK_KEY = KEY_INPUT_S;
    static constexpr int MOVE_RIGHT_KEY = KEY_INPUT_D;

    //�U��
    static constexpr int ATK_KEY = KEY_INPUT_E;
    static constexpr InputManager::JOYPAD_BTN ATK_BTN = InputManager::JOYPAD_BTN::RIGHT;

    //�X�L��
    static constexpr int SKILL_KEY = KEY_INPUT_Q;
    static constexpr InputManager::JOYPAD_BTN SKILL_BTN = InputManager::JOYPAD_BTN::TOP;

    //�X�L���ύX�L�[
    static constexpr int SKILL_CHANGE_KEY = KEY_INPUT_J;
    static constexpr InputManager::JOYPAD_BTN SKILL_CHANGE_BTN = InputManager::JOYPAD_BTN::R_BUTTON;

    //���
    static constexpr int DODGE_KEY = KEY_INPUT_N;
    static constexpr InputManager::JOYPAD_BTN DODGE_BTN = InputManager::JOYPAD_BTN::LEFT;



    PlayerBase(const SceneManager::PLAY_MODE _mode, const InputManager::JOYPAD_NO _padNum) :mode_(_mode), padNum_(_padNum) {}
    PlayerBase(const SceneManager::PLAY_MODE _mode, const SceneManager::CNTL _cntl) :mode_(_mode), cntl_(_cntl) {}
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void) = 0;
    void Init(void)override;
    virtual void Update(void)override;
    //virtual void UserUpdate(void);

    //Cpu����
    virtual void CpuUpdate(void);
    virtual void Draw(void)override;



    enum class CPU_STATE
    {
        NORMAL			//�ʏ�
        , ATTACK		//�U��
        , BREAK			//�x�e
        , MAX
    };

    enum class ACT_CNTL
    {
        NONE    //�������Ă��Ȃ��Ƃ�
        ,MOVE   //�ړ�
        ,DODGE             //���
        ,NMLATK            //�ʏ�U��    
        ,CHANGE_SKILL      //�X�L���؂�ւ�
        ,NMLSKILL          //�Z�����X�L��
        ,CHARGE_SKILL_DOWN  //�������X�L��(�������p)
        ,CHARGE_SKILL_KEEP  //�������X�L��(�������ςȂ�)
        ,CHARGE_SKILL_UP    //�������X�L��(����)
    };


    //�R���g���[���[�ύX�p�֐�
    void ChangeControll(SceneManager::CNTL _cntl);

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

    //���Z�b�g
    void Reset(void);

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

    enum class ATK_TYPE
    {
        NORMALATK
        , CHARGEATK
    };

    //*************************************************
    // �����o�ϐ�
    //*************************************************
    //�V�X�e���n
    //�X�e�[�^�X�n
    VECTOR userOnePos_;                                         //���[�U�[1�Ǐ]�p�̍��W   
    VECTOR colPos_;                                             //�v���C���[�̓����蔻����W
    ATK_ACT act_;                                               //�U�����
    std::map < ATK_ACT, std::function<void(void)>>changeAct_;   //�U���̕ύX
    std::function<void(void)>actUpdate_;                        //�U�����Ƃ̍X�V����
    float atkStartCnt_;                                         //�U������������܂ł̃J�E���g
    float moveSpeed_;                                           //�ړ��X�s�[�h
    float coolTime_[static_cast<int>(ATK_ACT::MAX)];            //���ꂼ��̃N�[���^�C���J�E���g
    bool isCool_[static_cast<int>(ATK_ACT::MAX)];               //���ꂼ��̍U���g���I�����i�[����
    float multiHitInterval_;                                    //���i�q�b�g�̃_���[�W�Ԋu
    ATK_TYPE atkType_;                                           //�^�C�v�ϐ�
    ATK_TYPE atkTypes_[static_cast<int>(ATK_ACT::MAX)];          //�U���̃^�C�v(�`���[�W���邩���Ȃ���)
    std::map<ATK_TYPE, std::function<void(void)>>changeAtkType_;//�U���^�C�v�ύX
    std::function<void(void)>atkTypeUpdate_;                    //�U���^�C�v���Ƃ̃A�b�v�f�[�g
    bool isPush_;                                               //�������X�L���p�̃{�^���������Ă��邩�ǂ���  true:�����Ă���




    //����Ǘ��p
    ACT_CNTL actCntl_;

    //���ꂼ��̍ő�l�Z�b�g�p(�U���̍��W�̓��[�J�����W�Őݒ肵�Ă܂�)
    std::map<ATK_ACT, ATK>atkMax_;
    float coolTimeMax_[static_cast<int>(ATK_ACT::MAX)];             //�N�[���^�C���ő�
    float atkStartTime_[static_cast<int>(ATK_ACT::MAX)];            //�U����������
    //�R���g���[���[�n
    InputManager::JOYPAD_NO padNum_;                //�Q�[���p�b�h�̔ԍ�
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
    std::map < CPU_STATE, std::function<void(void)>> cpuStateChanges_;  //��Ԃ��Ƃ̏�����
    std::function<void(void)> cpuStateUpdate_;                          //��Ԃ��Ƃ̍X�V
    bool isMove_;               //�����Ă��邩�ǂ���
    bool isCall_;               //�v���C���[�ɌĂяo���ꂽ��
    bool isMove2CallPlayer_;    //�����Ăяo���ꒆ���@true:�Ăяo����ăv���C���[�܂ňړ���
    bool isEnemySerch_;         //�G���T�[�`���������ĂȂ���
    float calledMoveSpeed_;     //�v���C���[�ɌĂяo���ꂽ�Ƃ��̉����p�ϐ�
    float moveStartDelay_;      //�ړ��̏��߂�x�点��
    bool moveAble_;             //�ړ��\����Ԃ�  true:�ړ��\
    //*************************************************
    //�����o�֐�
    //*************************************************
    VECTOR GetTargetVec(VECTOR _targetPos);


    //�v���C���[��CPU��USER������
    SceneManager::PLAY_MODE mode_;
    //���[�h�ύX���Ȃ����ǃf�o�b�O���₷���悤�ɂ���
    std::map < SceneManager::PLAY_MODE, std::function<void(void)>>changeMode_;
    std::function<void(void)>modeUpdate_;       //���[�h���Ƃ̏���

    std::map<SceneManager::CNTL, std::function<void(void)>>changeNmlActControll_;              //�ʏ�X�L��
    std::function<void(void)>nmlActUpdate_;                                             //�ʏ�X�L���X�V

    std::map<SceneManager::CNTL, std::function<void(void)>>changeChargeActCntl_;       //�R���g���[���[���Ƃ̃X�L���ύX
    std::function<void(void)>chargeActUpdate_;                                         //�`���[�W�U��

    //�A�j��No������
    void InitAnimNum(void);

    //�������ƂɈႤ�A�j���[�V�����̏�����
    virtual void InitCharaAnim(void);

    //���ꂼ��̃A�N�V�����̏�����
    virtual void InitAct(void);

    //�قڃK�[�h�p
    virtual void ResetGuardCnt(void);

    //�U���ύX�p
    void ChangeAct(const ATK_ACT _act);

    void ChangeNmlAtk(void);
    void ChangeSkillOne(void);
    void ChangeSkillTwo(void);



    //�U���̍ő�l�̏�����(�e����)
    void ResetParam(ATK& _atk);

    //USER�֌W
    //------------------------------------------------
    //���[�U�[������Ƃ��̍X�V
    void UserUpdate(void);

    //�f�o�b�O���₷���悤�Ƀ`�F���W���
    void ChangeMode(SceneManager::PLAY_MODE _mode);
    void ChangeUser(void);
    void ChangeCpu(void);

    //����n�i�L�[�{�[�h)
    void KeyBoardControl(void);

    //�Q�[���p�b�h
    void GamePad(void);


    SceneManager::CNTL cntl_;
    std::map < SceneManager::CNTL, std::function<void(void)>> changeCntl_;//�R���g���[���[�ύX�p
    std::function<void(void)> cntlUpdate_;                                 //���ꂼ��̍X�V

    SKILL_NUM skillNo_;     //�X�L���ύX�p

    //CPU
    //-------------------------------------------------

    //CPU�̃A�b�v�f�[�g
    void CpuActUpdate(ATK_ACT _act);

    //�e�����̏�����
    void CpuChangeNml(void);
    void CpuChangeAtk(void);
    void CpuChangeBreak(void);

    //�e��Ԃ̍X�V
    void CpuNmlUpdate(void);
    void CpuAtkUpdate(void);
    void CpuBreakUpdate(void);

    //�v���C���[(CPU�ƃ��[�U�[)���ʏ���
    //--------------------------------------------------
    //�U������
    virtual void AtkFunc(void) = 0;
    //�X�L��1
    virtual void Skill1Func(void) = 0;
    //�X�L��2
    virtual void Skill2Func(void) = 0;

    //�e�A�N�V�����̋��ʏ���
    void Action(void);

    //�ǂ̃A�N�V���������͂���Ă��邩�̊m�F
    bool CheckAct(ACT_CNTL _actCntl) { return actCntl_ == _actCntl ? true : false; }

    //�`���[�W�Ȃ��̍U��
    void NmlAct(void);
    //�`���[�W�Ȃ��U��(�L�[�{�[�h)
    void NmlActKeyBoard(void);
    //�`���[�W�Ȃ��U��(�p�b�h)
    void NmlActPad(void);

    //�ύX
    void ChangeNmlActControll(void);
    void ChangeNmlActKeyBoard(void);
    void ChangeNmlActPad(void);

    //�Z�����U�����ʏ���(�U���J�E���g�Ƃ��㌄�Ƃ�)
    void NmlActCommon(void);

    //�`���[�W�U��
    virtual void ChargeAct(void);
    //�`���[�W�U��(�L�[�{�[�h)
    void ChargeActKeyBoard(void);
    //�`���[�W�U��(�p�b�h)
    void ChargeActPad(void);

    //Change�֐�
    //�`���[�W�U��(�L�[�{�[�h)
    void ChangeChargeActKeyBoard(void);
    //�`���[�W�U��(�p�b�h)
    void ChangeChargeActPad(void);

    //�ύX�_
    void ChangeChargeActControll(void);

    void ChangeAtkType(ATK_ACT _act);


    //�U���^�C�v�ύX
    void ChangeChargeAct(void);
    void ChangeNmlAct(void);

    //�U���������t���O
    bool IsAtkStart(void) { return 0.0f < atkStartCnt_ && atkStartCnt_ <= atkStartTime_[static_cast<int>(act_)]; }

    //�U�����������̂��m�F����
    bool IsFinishAtkStart(void) { return atkStartCnt_ > atkStartTime_[static_cast<int>(act_)]; }

    //�R���g���[���[�ύX
    void ChangeGamePad(void);
    //�L�[�{�[�h�ύX
    void ChangeKeyBoard(void);


    //�U�����W�̓���
    void SyncActPos(ATK& _atk);

    //�X�L�����Ƃ̑���X�V
    void ChangeSkillControll(SKILL_NUM _skill);

    //�U���I�������̏�����
    virtual void InitAtk(void);

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
    bool IsAtkAction(void) { return IsAtkStart() || atk_.IsAttack() || atk_.IsBacklash(); }

    //�U���\���ǂ���(true:�\)
    bool IsAtkable(void) { return!IsAtkAction() && !IsDodge(); }


    //����֘A
    //---------------------------------------
    //����\��
    //��𒆂��ǂ���

    bool IsDodgeable(void) { return !IsDodge() && !IsAtkAction() && !IsCoolDodge(); }
    //�N�[���^�C�������ǂ���
    bool IsCoolDodge(void) { return dodgeCdt_ < DODGE_CDT_MAX; }
    void Dodge(void);

    //�h�b�W�J�E���g������
    void ResetDodgeFrame(void) { dodgeCnt_ = 0.0f; }

    //�X�L�����z�֐�
    //-----------------------------------------
    //�X�L��1�̑���
    virtual void SkillOneControll(void);

    //�X�L��2�̑���
    virtual void SkillTwoControll(void);

    //�X�L��
    //virtual void Skill(void);
    /// <summary>
    /// �X�L�����t���O
    /// </summary>
    /// <param name="_frameSkillNo">�X�L���t���[���ő�l(���̓X�L��1��2)</param>
    /// <returns>�X�L����/�X�L�����łȂ�</returns>
    bool IsSkillAction(SKILL_NUM _num) {
        return atk_.IsAttack()
            || atk_.IsBacklash();
    }

    //�X�L���g�p�\���ǂ���
    bool IsSkillable(void) { return !IsAtkAction() && !IsDodge(); }

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

    //�ړ�����p�x
    float moveDeg_;

    std::map<ACT_CNTL, std::function<void(void)>>changeActCntl_;        //�A�N�V�������ƂɕԂ��{�^����ύX
    std::function<bool(void)>actCntlUpdate_;  

  

    void ProcessAct(void);

    //�X�L���ύX����
    void SkillChange(void);
   
};