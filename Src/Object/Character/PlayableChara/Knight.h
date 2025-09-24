#pragma once
#include "PlayerBase.h"
class Arrow;
class Knight :
    public PlayerBase
{
public:
    //�X�e�[�^�X
    static constexpr float POW_ATK = 120.0f;
    static constexpr float MAX_DEF = 150.0f;
    static constexpr float MAX_HP = 250;
    static constexpr float SPEED = PlayerBase::MOVE_SPEED_NORMAL;

    //�U���J�n����̑傫��
    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	
    //�X�L���g�p�J�n���ɃN�[���^�C���𑝂₷����

    static constexpr float SKILL_TWO_START_COOLTIME = 3.0f;

    //�K�[�h�\�ɂȂ�c��N�[���^�C��(�K�[�h���ԍŒ�1�b�m�ۂ���������)
    static constexpr float GUARD_STARTABLE_COOL = 4.0f;

    //�K�[�h�G�t�F�N�g�T�C�Y
    static constexpr float GUARD_EFFECT_SIZE = 20.0f;
#ifdef DEBUG_COOL
    //�N�[���^�C��
    static constexpr float NORMAL_ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 1.0f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;

   

#else // DEBUG_COOL
#ifdef DEBUG_ON
    void DrawDebug(void)override;
#endif // DEBUG_ON

    //�N�[���^�C��
#endif // DEBUG_COOL
    //�ʏ�U��
    //------------------------------------------------------------------
    //�N�[���^�C��
    static constexpr float NORMAL_ATK_COOLTIME = 0.75f;
    //�O��
    static constexpr float NORMAL_ATK_START = 0.2f; 
    //��������
    static constexpr float NORMAL_ATK_DURATION = 0.4f - NORMAL_ATK_START;  
    //�㌄
    static constexpr float NORMAL_ATK_BACKRASH = 0.1f;       
    //���[�J�����W
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f }; 
    //�����蔻��̑傫��
    static constexpr float NORMAL_ATK_COL = CHARACTER_SCALE * 100.0f;
    //�З� 
    static constexpr float NORMAL_ATK_POW = 9.0f;
    //------------------------------------------------------------------
    //�X�L��1
    //------------------------------------------------------------------
    //�N�[���^�C��
    static constexpr float SKILL_ONE_COOLTIME = 3.3f;
    //�O��
    static constexpr float SKILL_ONE_START = 0.2f;
    //��������
    static constexpr float SKILL_ONE_DURATION = 0.3f;
    //�㌄
    static constexpr float SKILL_ONE_BACKRASH = 0.2f;
    //�����蔻��̑傫��
    static constexpr float SKILL_ONE_COL = CHARACTER_SCALE * 150.0f;
    //���΍��W
    static constexpr VECTOR SKILL_ONE_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    //�З�
    static constexpr float SKILL_ONE_POW = 30.0f;   

    //------------------------------------------------------------------
    //�X�L��2
    //------------------------------------------------------------------
    //�N�[���^�C��
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;
    //�O��
    static constexpr float SKILL_TWO_START = 0.3f;
    //��������
    static constexpr float SKILL_TWO_DURATION = 3.0f;
    //�㌄
    static constexpr float SKILL_TWO_BACKRASH = 0.0f;
    //�����蔻��
    static constexpr float SKILL_TWO_COL = CHARACTER_SCALE * 0.0f;
    //���΍��W
    static constexpr VECTOR SKILL_TWO_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };
    //�З�(�K�[�h�X�L���Ȃ̂ňЗ͂͂Ȃ�)
    static constexpr float SKILL_TWO_POW = 0.0f;

    //�K�[�h���̃X�e�b�v
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.7f;
    //------------------------------------------------------------------
    //�ʏ�U���̍ő�l
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS
        ,NORMAL_ATK_COL
        ,NORMAL_ATK_POW
        ,NORMAL_ATK_DURATION
        ,NORMAL_ATK_BACKRASH
        ,0.0f,false };

    //�X�L���P
    static constexpr ATK SKILL_ONE_MAX{ SKILL_ONE_COL_LOCAL_POS
        ,SKILL_ONE_COL
        ,SKILL_ONE_POW
        ,SKILL_ONE_DURATION
        ,SKILL_ONE_BACKRASH
        ,0.0f,false };

    //�X�L���Q
    static constexpr ATK SKILL_TWO_MAX{ SKILL_TWO_COL_LOCAL_POS
        ,SKILL_TWO_COL
        ,SKILL_TWO_POW
        ,SKILL_TWO_DURATION
        ,SKILL_TWO_BACKRASH
        ,0.0f,false };

    //�ŗL�A�j���[�V����
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_ONE_NUM = 2;
    static constexpr int SKILL_TWO_NUM = 18;
    static constexpr int BLOCKING_NUM = 21;
    Knight(void);
    ~Knight(void) = default;
    void SetParam(void)override;
    void Update(void)override;
    void Draw(void)override;



protected:
    //�U���A�N�V�����̃p�����[�^�̏�����
    void InitAct(void)override;
    void AtkFunc(void)override;

    //void ResetGuardCnt(void)override;

    //�U������
    void NmlAtkInit(void)override;
    //�X�L������
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;

    //�e�X�L���̏���
    void InitSkill(ATK_ACT _act);
    void Skill1Func(void)override;
    void Skill2Func(void)override;

    //�ŗL�A�j���[�V�����ԍ��̏�����
    void InitCharaAnim(void)override;

private:
};

