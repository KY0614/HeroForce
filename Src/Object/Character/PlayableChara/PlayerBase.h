#pragma once
#include<map>
#include<functional>
#include"../Utility/AsoUtility.h"
#include"../../../Manager/Generic/SceneManager.h"
#include "../../UnitBase.h"
#include"../../../Manager/Generic/InputManager.h"
#include"../PlayerInput.h"
#include "../../UnitBase.h"

//�f�o�b�O
#define DEBUG_ON
//#define INPUT_DEBUG_ON
//#define DEBUG_COOL
class PlayerDodge;
class PlayerBase :
    public UnitBase
{
public:
#ifdef DEBUG_ON
    void InitDebug(void);
    unsigned int color_Col_;
    unsigned int color_Atk_;
    unsigned int color_skl1_;
    unsigned int color_skl2_;

    //�f�o�b�O�p�֐�
    virtual void DrawDebug(void);

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
    static constexpr float SPEED_DODGE = 15.0f;
    static constexpr float MOVE_SPEED_SLOW = 3.0f;
    static constexpr float MOVE_SPEED_NORMAL = 5.0f;
    static constexpr float MOVE_SPEED_FAST = 8.0f;
   



    static constexpr float FRAME_DODGE_MAX = 1.0f * CHARACTER_SCALE;
    static constexpr float DODGE_CDT_MAX = 0.5f;



    //�v���C���[���g�̓����蔻��
    static constexpr VECTOR PLAYER_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //���g�̓����蔻�蔼�a
    static constexpr float MY_COL_RADIUS = 66.0f * CHARACTER_SCALE;

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

    //PlayerBase(const InputManager::JOYPAD_NO _padNum) :padNum_(_padNum) {}
    //PlayerBase(const SceneManager::CNTL _cntl) :cntl_(_cntl) {}
    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void) = 0;
    void Init(void)override;
    virtual void Update(void)override;
    virtual void Draw(void)override;


   // //����֘A
   ////---------------------------------------
   // const bool IsDodge(void)const { return 0.0f < dodgeCnt_ && dodgeCnt_ < FRAME_DODGE_MAX; }

    //-------------------------------------------------------------
    //�_���[�W�֐�
    void Damage(void);

    //���Z�b�g
    void Reset(void);

    //�ړ��֘A
     //-------------------------------------
    //�ړ�����
    void Move(float _deg, VECTOR _axis);

    //��������
    void Turn(float _deg, VECTOR _axis);

    //�����Ă邩�ǂ���
    bool IsMove(void) { return moveSpeed_ > 0.0f; }

    //�X�L���g�p�\���ǂ���
    bool IsSkillable(void);
    //�X�L���ύX����
    void SkillChange(void);

    //*****************************************************
    //�Q�b�^
    //*****************************************************
    //�N�[����
    const bool GetIsCool(ATK_ACT _act) { return isCool_[static_cast<int>(_act)]; }

    //�c�[���^�C��
    const float GetCoolTime(ATK_ACT _act) { return coolTime_[static_cast<int>(_act)]; }

    //���݂̎g�������X�L��
    const ATK_ACT GetSkillNo(void) { return skillNo_; }

    //�U��������
    const bool GetIsAtk(void){ return isAtk_; }

    //�X�L��������
    const bool GetIsSkill(void){ return isSkill_; }

    //�O��
    const float GetAtkStartCnt(void) { return atkStartCnt_; }

    //�A�j���[�V�����X�e�b�v
    const float GetStepAnim(void) { return stepAnim_; }

    //���
    PlayerDodge* GetDodge(void) { return dodge_; }

    //�N�[���^�C�������̃Q�b�^
    float* GetCoolTimePer(void) { return coolTimePer_; }


    //**************************************************************
    //�Z�b�^�[
    //**************************************************************
    //�U���n
    //-------------------------------------------------------------------------------------------------------------------
    //�U���̂��ꂼ��̒l
    void SetAtk(const ATK _atk) { atk_ = _atk; }
    //�O���̃J�E���^�[
    void SetAtkStartCnt(const float _atkStartCnt) { atkStartCnt_ = _atkStartCnt; }

    //�O���̍ő厞�ԃZ�b�^
    void SetAtkStartTime(const float _atkStartTime, const ATK_ACT _act) { atkStartTime_[static_cast<int>(_act)] = _atkStartTime; }

    //�U�����邩�ǂ���
    void SetIsAtk(const bool _isAtk) { isAtk_ = _isAtk; }

    //�U���������t���O
    const bool IsAtkStart(void)const { return 0.0f < atkStartCnt_ && atkStartCnt_ <= atkStartTime_[static_cast<int>(act_)]; }

    //�U�����������̂��m�F����
    const bool IsFinishAtkStart(void)const { return atkStartCnt_ > atkStartTime_[static_cast<int>(act_)]; }

    //�U���ύX�p(��ɓ��͂��ꂽ��ς���悤�ɂ���)
    void ChangeAct(const ATK_ACT _act);

    //�U���̍ő�l�̏�����(�|��Ƃ��̈Ⴄatk�̔z��Ƃ��g���p)
    void ResetParam(ATK& _atk);

    //�ߐڍU����atk�������p
    void ResetParam(void);

    //�U���I�������̏�����
    virtual void InitAtk(void);

    //�������ԃZ�b�^
    void SetDulation(const float _dulation) { atk_.duration_ = _dulation; }

    //�X�L�����邩
    void SetIsSkill(const bool _isSkill) { isSkill_ = _isSkill; }

    //�ړ��\���ǂ���
    void SetMoveAble(const bool _moveAble) { moveAble_ = _moveAble; }
    //�N�[���ɂ��邩�ǂ���
    void SetIsCool(const bool _isCool, const ATK_ACT _act) { isCool_[static_cast<int>(_act)] = _isCool; }

    //�N�[���^�C���Z�b�^
    void SetCoolTime(const float coolTime, ATK_ACT _act) { coolTime_[static_cast<int>(_act)] = coolTime; }

    //���̑�
    //------------------------------------------------------------------------------------
    //�A�j���[�V�����X�e�b�v
    void SetStepAnim(const float _stepAnim) { stepAnim_ = _stepAnim; }

    //�X�s�[�h
    void SetMoveSpeed(const float _speed) { moveSpeed_ = _speed; }

protected:
    //�|�C���^
      //����@�\
    PlayerDodge* dodge_;

    //*************************************************
    // �񋓌^
    //*************************************************



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
        float CoolTime_[static_cast<int>(ATK_ACT::MAX)];
        float CoolTimeMax_[static_cast<int>(ATK_ACT::MAX)];                //�N�[���^�C���ő�
        std::map<ATK_ACT, ATK>atkMax_;         
        float atkStartTime_[static_cast<int>(ATK_ACT::MAX)];            //�U����������
        bool IsAtkStart(void){ return 0.0f < atkStartCnt_ && atkStartCnt_ <= atkStartTime_[static_cast<int>(act_)]; }
    };


    //*************************************************
    // �����o�ϐ�
    //*************************************************
    //�V�X�e���n
    //�X�e�[�^�X�n
    ATK_ACT act_;                                               //�U�����
    std::map < ATK_ACT, std::function<void(void)>>changeAct_;   //�U���̕ύX
    std::function<void(void)>actUpdate_;                        //�U�����Ƃ̍X�V����
    float atkStartCnt_;                                         //�U������������܂ł̃J�E���g
    ATK_TYPE atkTypes_[static_cast<int>(ATK_ACT::MAX)];         //�U���̃^�C�v(�`���[�W���邩���Ȃ���)
    std::map<ATK_TYPE, std::function<void(void)>>changeAtkType_;//�U���^�C�v�ύX
    std::function<void(void)>atkTypeUpdate_;                    //�U���^�C�v���Ƃ̃A�b�v�f�[�g
    ATK_TYPE atkType_;                                          //�^�C�v�ϐ�
    float coolTime_[static_cast<int>(ATK_ACT::MAX)];            //���ꂼ��̃N�[���^�C���J�E���g
    bool isCool_[static_cast<int>(ATK_ACT::MAX)];               //���ꂼ��̍U���g���I�����i�[����
    float multiHitInterval_;                                    //���i�q�b�g�̃_���[�W�Ԋu
    VECTOR userOnePos_;                                         //���[�U�[1�Ǐ]�p�̍��W   
    VECTOR colPos_;                                             //�v���C���[�̓����蔻����W
    float speed_;                                               //�L�����N�^�[�̃X�e�[�^�X�Ƃ��ẴX�s�[�h

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

 
    //*************************************************
    //�����o�֐�
    //*************************************************
    VECTOR GetTargetVec(VECTOR _targetPos);



    //�A�j��No������
    void InitAnimNum(void);

    //�������ƂɈႤ�A�j���[�V�����̏�����
    virtual void InitCharaAnim(void);

    //���ꂼ��̃A�N�V�����̏�����
    virtual void InitAct(void);

    void ChangeNmlAtk(void);
    void ChangeSkillOne(void);
    void ChangeSkillTwo(void);

    //USER�֌W
    //------------------------------------------------
    //���[�U�[������Ƃ��̍X�V
    void UserUpdate(void);

    SceneManager::CNTL cntl_;

    ATK_ACT skillNo_;     //�X�L���ύX�p

 
    //�v���C���[(CPU�ƃ��[�U�[)���ʏ���
    //--------------------------------------------------
    //�U������
    virtual void AtkFunc(void) = 0;
    //�X�L��1
    virtual void Skill1Func(void) = 0;
    //�X�L��2
    virtual void Skill2Func(void) = 0;

    //�U�����W�̓���
    void SyncActPos(ATK& _atk);

    //�X�L�����Ƃ̑���X�V
    void ChangeSkillControll(ATK_ACT _skill);

    //�U��
    //-------------------------------------
    //�U�������ǂ���(UnitBase�ŏC���\��)
    const bool IsAtkAction(void)const { return IsAtkStart() || atk_.IsAttack() || atk_.IsBacklash(); }

    //�U���\���ǂ���(true:�\)
    const bool IsAtkable(void)const;


    //����֘A
    //---------------------------------------
    //����\��
    //��𒆂��ǂ���

    const bool IsDodgeable(void)const;

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

    //�N�[���^�C���̃J�E���g
    void CoolTimeCnt(void);

private:
    //�����o�ϐ�
    float coolTimePer_[static_cast<int>(ATK_ACT::MAX)];
  
#ifdef DEBUG_INPUT

    //******************************************************************************************
#endif // DEBUG_ON
    
    //�A�N�V�����̔�������
    void ProcessAct(void);
    void CoolTimePerCalc(void);
 

    //�U������
    virtual void NmlAtkInit(void)=0;
    //�X�L������
    virtual void SkillOneInit(void)=0;
    virtual void SkillTwoInit(void)=0;

};