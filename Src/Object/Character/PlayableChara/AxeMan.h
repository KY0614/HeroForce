#pragma once
#include "PlayerBase.h"
class AxeMan :
    public PlayerBase
{
public:
#ifdef DEBUG_ON
    void DrawDebug(void)override;
#endif // DEBUG_ON

    //�ʏ�U��
    //-------------------------------------------------
    //�N�[���^�C��
    static constexpr float NORMAL_ATK_COOLTIME = 1.5f;
    //�O��
    static constexpr float NORMAL_ATK_START = 0.2f;
    //�U������
    static constexpr float NORMAL_ATK_DURATION = 0.5f - NORMAL_ATK_START;
    //�㌄
    static constexpr float NORMAL_ATK_BACKRASH = 0.1f;
    //���΍��W
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    //�З�
    static constexpr float NORMAL_ATK_POW = 12.0f;
    //�����蔻��
    static constexpr float NORMAL_ATK_COL = CHARACTER_SCALE * 100.0f;
    //-------------------------------------------------
    //�X�L���P
    //-------------------------------------------------
    //�N�[���^�C��
    static constexpr float SKILL_ONE_COOLTIME = 6.0f;
    //�O��
    static constexpr float SKILL_ONE_START = 10.0f;
    //��������
    static constexpr float SKILL_ONE_DURATION = 0.7f;
    //�㌄
    static constexpr float SKILL_ONE_BACKRASH = 0.2f;
    //���΍��W
    static constexpr VECTOR SKILL_ONE_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    //�����蔻��
    static constexpr float SKILL_ONE_COL = CHARACTER_SCALE * 150.0f;

    //���߂ĂȂ����̈З�
    static constexpr float SKILL_ONE_POW = 25.0f;
    //���߂��Ƃ��̈З�
    static constexpr float SKILL_ONE_CHARGE_POW = 50.0f;
    //�`���[�W�A�j���[�V����
    static constexpr float SKILL_ONE_CHARGE_STEPANIM = 16.9f;
    //-------------------------------------------------
    //�X�L���Q
    //-------------------------------------------------
    //�N�[���^�C��
    static constexpr float SKILL_TWO_COOLTIME = 3.5f;
    //�O��
    static constexpr float SKILL_TWO_START = 0.2f;
    //�U������
    static constexpr float SKILL_TWO_DURATION = 3.0f - SKILL_TWO_START;
    //�㌄
    static constexpr float SKILL_TWO_BACKRASH = 0.2f;
    //���΍��W
    static constexpr VECTOR SKILL_TWO_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };
    //�����蔻��̑傫��
    static constexpr float SKILL_TWO_COL = CHARACTER_SCALE * 200.0f;
    //�З�
    static constexpr float SKILL_TWO_POW = 8.0f;

    //�G�t�F�N�g�T�C�Y
    static constexpr float CHARGE_AXE_EFF_SIZE = 20.0f;

    //�A�j���[�V�����ύX�t���[��
    static constexpr float SKILL_TWO_CHANGE_ANIM_STEP = 14.0f;
    //�A�j���[�V�����`�F���W����܂ł̃J�E���g
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.25f;
    //�X�L���`���[�W
    static constexpr float CHARGE_SKILL_EFF_SIZE = 50.0f;
 
    



    //�ʏ�U���̍ő�l
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS,NORMAL_ATK_COL,NORMAL_ATK_POW,NORMAL_ATK_DURATION,NORMAL_ATK_BACKRASH,0.0f,false };

    //�X�L���P
    static constexpr ATK SKILL_ONE_MAX{ SKILL_ONE_COL_LOCAL_POS ,SKILL_ONE_COL ,SKILL_ONE_POW,SKILL_ONE_DURATION ,SKILL_ONE_BACKRASH ,0.0f,false };

    //�X�L���Q
    static constexpr ATK SKILL_TWO_MAX{ SKILL_TWO_COL_LOCAL_POS ,SKILL_TWO_COL ,SKILL_TWO_POW,SKILL_TWO_DURATION ,SKILL_TWO_BACKRASH ,0.0f,false };




    //�ŗL�A�j���[�V����
    static constexpr int ATK_NUM = 9;
    static constexpr int SKILL_ONE_NUM = 8;
    static constexpr int SKILL_TWO_NUM = 9;
    static constexpr int SPIN_NUM = 11;

    //�X�L���Q�̉�]�U���̑��i�q�b�g�C���^�[�o��
    static constexpr float MULTIHIT_INTERVAL = 0.5f;
    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//�U���J�n����̑傫��

    AxeMan(void);
    ~AxeMan(void) = default;
    void SetParam(void)override;
protected:

    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
    void InitAct(void)override;
    void InitSkill(ATK_ACT _act);

    void Draw(void)override;

    //�ŗL�A�j���[�V�����ԍ��̏�����
    void InitCharaAnim(void)override;

    //�U������
    void NmlAtkInit(void)override;
    //�X�L������
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;
};

