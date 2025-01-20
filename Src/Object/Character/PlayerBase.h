#pragma once
#include<map>
#include<functional>
#include<variant>
#include"../Utility/AsoUtility.h"
#include"../Manager/SceneManager.h"
#include"../Manager/InputManager.h"
#include"PlayerInput.h"
#include "../UnitBase.h"

//�f�o�b�O
#define DEBUG_ON
//#define INPUT_DEBUG_ON
#define DEBUG_COOL
class PlayerBase :
    public UnitBase
{
public:
#ifdef DEBUG_ON
    void InitDebug(void);
    //�f�o�b�O�p�֐�
    virtual void DrawDebug(void);
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
   



    static constexpr float FRAME_DODGE_MAX = 1.0f * CHARACTER_SCALE;
    static constexpr float DODGE_CDT_MAX = 0.5f;

    static constexpr int MAX_HP = 100;

    //�v���C���[���g�̓����蔻��
    static constexpr VECTOR PLAYER_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //���g�̓����蔻�蔼�a
    static constexpr float MY_COL_RADIUS = 66.0f * CHARACTER_SCALE;


    PlayerBase(const InputManager::JOYPAD_NO _padNum) :padNum_(_padNum) {}
    PlayerBase(const SceneManager::CNTL _cntl) :cntl_(_cntl) {}
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void) = 0;
    void Init(void)override;
    virtual void Update(void)override;
    virtual void Draw(void)override;





    //�R���g���[���[�ύX�p�֐�
    void ChangeControll(SceneManager::CNTL _cntl);

    //����֘A
   //---------------------------------------
    const bool IsDodge(void)const { return 0.0f < dodgeCnt_ && dodgeCnt_ < FRAME_DODGE_MAX; }

    //-------------------------------------------------------------
    //�_���[�W�֐�
    void Damage(void);

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
    struct PlayerAtk
    {
        ATK_ACT act_;
        float atkStartCnt_;
        ATK atk_;
        float coolCnt_;
        float CoolTime_[static_cast<int>(ATK_ACT::MAX)];
        std::map<ATK_ACT, ATK>atkMax_;
        float coolTimeMax_[static_cast<int>(ATK_ACT::MAX)];             //�N�[���^�C���ő�
        float atkStartTime_[static_cast<int>(ATK_ACT::MAX)];            //�U����������
        bool IsAtkStart(void){ return 0.0f < atkStartCnt_ && atkStartCnt_ <= atkStartTime_[static_cast<int>(act_)]; }

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
    ATK_TYPE atkType_;                                          //�^�C�v�ϐ�
    ATK_TYPE atkTypes_[static_cast<int>(ATK_ACT::MAX)];         //�U���̃^�C�v(�`���[�W���邩���Ȃ���)
    std::map<ATK_TYPE, std::function<void(void)>>changeAtkType_;//�U���^�C�v�ύX
    std::function<void(void)>atkTypeUpdate_;                    //�U���^�C�v���Ƃ̃A�b�v�f�[�g
    bool isPush_;                                               //�������X�L���p�̃{�^���������Ă��邩�ǂ���  true:�����Ă���
    bool moveAble_;             //�ړ��\����Ԃ�  true:�ړ��\
    bool isAtk_;                                                 //�ʏ�U���J�n�������ǂ���
    bool isSkill_;                                                 //�X�L���J�n�������ǂ���


 

    //���ꂼ��̍ő�l�Z�b�g�p(�U���̍��W�̓��[�J�����W�Őݒ肵�Ă܂�)
    std::map<ATK_ACT, ATK>atkMax_;
    float coolTimeMax_[static_cast<int>(ATK_ACT::MAX)];             //�N�[���^�C���ő�
    float atkStartTime_[static_cast<int>(ATK_ACT::MAX)];            //�U����������
    //�R���g���[���[�n
    InputManager::JOYPAD_NO padNum_;                //�Q�[���p�b�h�̔ԍ�
    int leftStickX_;            //�p�b�h�̃X�e�B�b�N��X�p�x  
    int leftStickY_;            //�p�b�h�̃X�e�B�b�N��Y�p�x
    float stickDeg_;            //�p�b�h�̃X�e�B�b�N�̊p�x

    float moveDeg_;             //�ړ�����

   //����n
    float dodgeCnt_;            //�h�b�W�J�E���g
    float dodgeCdt_;            //�h�b�W�̌㌄

 
    //*************************************************
    //�����o�֐�
    //*************************************************
    VECTOR GetTargetVec(VECTOR _targetPos);


    ////�v���C���[��CPU��USER������
    //SceneManager::PLAY_MODE mode_;
    ////���[�h�ύX���Ȃ����ǃf�o�b�O���₷���悤�ɂ���
    //std::map < SceneManager::PLAY_MODE, std::function<void(void)>>changeMode_;
    //std::function<void(void)>modeUpdate_;       //���[�h���Ƃ̏���

    std::map<SceneManager::CNTL, std::function<void(void)>>changeNmlActControll_;       //�ʏ�X�L��
    std::function<void(void)>nmlActUpdate_;                                             //�ʏ�X�L���X�V
     //�`���[�W�U��
    std::map<SceneManager::CNTL, std::function<void(void)>>changeChargeActCntl_;       //�R���g���[���[���Ƃ̃X�L���ύX
    std::function<void(void)>chargeActUpdate_;




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

    ////�f�o�b�O���₷���悤�Ƀ`�F���W���
    //void ChangeMode(SceneManager::PLAY_MODE _mode);
    //void ChangeUser(void);
    //void ChangeCpu(void);

    //����n�i�L�[�{�[�h)
    void KeyBoardControl(void);

    //�Q�[���p�b�h
    void GamePad(void);


    SceneManager::CNTL cntl_;
    std::map < SceneManager::CNTL, std::function<void(void)>> changeCntl_;//�R���g���[���[�ύX�p
    std::function<void(void)> cntlUpdate_;                                 //���ꂼ��̍X�V

    ATK_ACT skillNo_;     //�X�L���ύX�p

 
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

    //�`���[�W�Ȃ��̍U��
    void NmlAct(void);
    //�`���[�W�Ȃ��U��(�L�[�{�[�h)
    void NmlActKeyBoard(void);
    //�`���[�W�Ȃ��U��(�p�b�h)
    void NmlActPad(void);

    //�ύX(�L�[�{�[�h�ƃQ�[���p�b�h)
    void ChangeNmlActControll(void);
    void ChangeNmlActKeyBoard(void);
    void ChangeNmlActPad(void);

    //�Z�����U�����ʏ���(�U���J�E���g�Ƃ��㌄�Ƃ�)
    virtual void NmlActCommon(void);

    //�Z�����patk�X�V
    void NmlAtkUpdate(void);


    //�`���[�W�U��
    virtual void ChargeAct(void);
    //�`���[�W�U��(�L�[�{�[�h)
    void ChargeActKeyBoard(void);
    //�`���[�W�U��(�p�b�h)
    void ChargeActPad(void);

    ////Change�֐�
    ////�`���[�W�U��(�L�[�{�[�h)
    //void ChangeChargeActKeyBoard(void);
    ////�`���[�W�U��(�p�b�h)
    //void ChangeChargeActPad(void);

    ////�ύX�_
    //void ChangeChargeActControll(void);

    //void ChangeAtkType(ATK_ACT _act);


    ////�U���^�C�v�ύX
    //void ChangeChargeAct(void);
    //void ChangeNmlAct(void);

    //�U���������t���O
    const bool IsAtkStart(void)const { return 0.0f < atkStartCnt_ && atkStartCnt_ <= atkStartTime_[static_cast<int>(act_)]; }

    //�U�����������̂��m�F����
    const bool IsFinishAtkStart(void)const { return atkStartCnt_ > atkStartTime_[static_cast<int>(act_)]; }

    //�R���g���[���[�ύX
    void ChangeGamePad(void);
    //�L�[�{�[�h�ύX
    void ChangeKeyBoard(void);


    //�U�����W�̓���
    void SyncActPos(ATK& _atk);

    //�X�L�����Ƃ̑���X�V
    void ChangeSkillControll(ATK_ACT _skill);

    //�U���I�������̏�����
    virtual void InitAtk(void);

    //�ړ��֘A
    //-------------------------------------
    //�ړ�����
    void Move(float _deg, VECTOR _axis);

    //��������
    void Turn(float _deg, VECTOR _axis);

    //�����Ă邩�ǂ���
    bool IsMove(void) { return moveSpeed_ > 0.0f; }

    //�U��
    //-------------------------------------
    //�U�������ǂ���(UnitBase�ŏC���\��)
    const bool IsAtkAction(void)const { return IsAtkStart() || atk_.IsAttack() || atk_.IsBacklash(); }

    //�U���\���ǂ���(true:�\)
    const bool IsAtkable(void)const { return!IsAtkAction() && !IsDodge(); }


    //����֘A
    //---------------------------------------
    //����\��
    //��𒆂��ǂ���

    const bool IsDodgeable(void)const { return !IsDodge() && !IsAtkAction() && !IsCoolDodge(); }
    //�N�[���^�C�������ǂ���
    const bool IsCoolDodge(void)const { return dodgeCdt_ < DODGE_CDT_MAX; }
    void Dodge(void);

    //�h�b�W�J�E���g������
    void ResetDodgeFrame(void) { dodgeCnt_ = 0.0f; }


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

    //std::map<ACT_CNTL, std::function<void(void)>>changeActCntl_;        //�A�N�V�������ƂɕԂ��{�^����ύX
    //std::function<bool(void)>actCntlUpdate_;

#ifdef DEBUG_INPUT
//************************************************************************
//�L�[�{�[�h�ƃp�b�h�̋��ʊe�A�N�V�����{�^���ݒ�(�e�X�g�p�Ŏg�����̂ňꉞ�c���Ă����܂�)
//************************************************************************
//�R���g���[���[�̃{�^���ԍ�(�����܂Ńe�X�g�p�Ȃ̂�private�ɒu���Ă����܂�)
    static constexpr int RIGHT_BTN = PAD_INPUT_B;
    static constexpr int TOP_BTN = PAD_INPUT_X;
    static constexpr int R_BUTTON = PAD_INPUT_R;
    static constexpr int LEFT_BTN = PAD_INPUT_Y;
    //�{�^�����͂��邽�߂̏��
    struct InputMapInfo
    {
        SceneManager::CNTL type;     //�R���g���[���[�^�C�v

        //�{�^���ԍ�
        //std::variant<int, InputManager::JOYPAD_IN_STATE> buttonId_;
        int buttonId_;
    };

    //�p�b�h�ƃL�[�{�[�h�̋��ʃ{�^���ɖ��O��t����
    using InputActionMap_t = std::map<std::string, std::vector<InputMapInfo>>;
    using InputType = SceneManager::CNTL;
    std::map<std::string, bool>currentInput_;                           //��������Ă�{�^����Ԃ�
    std::map<std::string, bool>lastInput_;                              //�O�ɉ����ꂽ�{�^��
    InputActionMap_t inputActionMap_;                                   //�A�N�V�����{�^�����܂Ƃ߂Ĉ������߂̕ϐ�
    /// <summary>
    /// ���̓`�F�b�N
    /// </summary>
    /// <param name="action">���ׂ����A�N�V�����̖��O</param>
    /// <returns>true�@������Ă�@/�@false�@������ĂȂ�</returns>
    bool IsPressed(const std::string& action)const;

    //���͏��̍X�V
    void InputUpdate(void);
    //******************************************************************************************
#endif // DEBUG_ON
    //����ꌳ��
    void ProcessInput(void);
    
    //�A�N�V�����̔�������
    void ProcessAct(void);

    //�X�L���ύX����
    void SkillChange(void);

    //�U������
    virtual void NmlAtkInit(void);
    //�X�L������
    virtual void SkillOneInit(void);
    virtual void SkillTwoInit(void);

};