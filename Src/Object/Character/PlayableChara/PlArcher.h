#pragma once
#include "../PlayerBase.h"
class Arrow;
class PlArcher :
    public PlayerBase
{
public:
    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//�U���J�n����̑傫��
    static constexpr float ATKABLE_TIME = 1.0f;                         //�A�ˎ�t����

    //�N�[���^�C��
    static constexpr float ATK_COOLTIME = 0.1f;
    static constexpr float SKILL_ONE_COOLTIME = 3.0f;
    static constexpr float SKILL_TWO_COOLTIME = 5.0f;

    //�U�������̎���
    static constexpr float ATK_START = 0.2f;
    static constexpr float SKILL_ONE_START = 1.0f;
    static constexpr float SKILL_TWO_START = 0.2f;

    //�e�U���̎�������
    static constexpr float FRAME_ATK_DURATION = 2.0f;
    static constexpr float FRAME_SKILL1_DURATION = 0.7f;
    static constexpr float FRAME_SKILL2_DURATION = 3.0f - SKILL_TWO_START;
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.25f;

    //�㌄
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr float FRAME_SKILL1_BACKRASH = 0.2f;
    static constexpr float FRAME_SKILL2_BACKRASH = 0.2f;

    //�e�U���̍��W
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //�U���͈�
    static constexpr float COL_ATK = CHARACTER_SCALE * 100.0f;
    static constexpr float COL_SKILL1 = CHARACTER_SCALE * 150.0f;
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 200.0f;

    //�U���З�
    static constexpr float ATK_POW = 7.0f;
    static constexpr float SKILL_ONE_POW_MIN = 15.0f;
    static constexpr float SKILL_ONE_POW_MAX = 25.0f;
    static constexpr float SKILL_TWO_POW = 0.0f;

    //�ŗL�A�j���[�V����
    static constexpr int ATK_NUM = 16;
    static constexpr int SKILL_ONE_NUM = 57;
    static constexpr int SKILL_TWO_NUM = 57;

    //�ʏ�U���̍ő�l
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS,COL_ATK,ATK_POW,FRAME_ATK_DURATION,FRAME_ATK_BACKRASH,0.0f,false };

    //�X�L���P
    static constexpr ATK SKILL_ONE_MAX{ SKILL1_COL_LOCAL_POS ,COL_SKILL1 ,SKILL_ONE_POW_MIN,FRAME_SKILL1_DURATION ,FRAME_SKILL1_BACKRASH ,0.0f,false };

    //�X�L���Q
    static constexpr ATK SKILL_TWO_MAX{ SKILL2_COL_LOCAL_POS ,COL_SKILL2 ,SKILL_TWO_POW,FRAME_SKILL2_DURATION ,FRAME_SKILL2_BACKRASH ,0.0f,false };


    //�|��֌W
    static constexpr int ARROW_SIZE_MAX = 5;	//��̍ő�ێ���
    static constexpr float RELOAD_TIME = 5.0f;	//��̃����[�h����
    static constexpr float ARROW_SPEED = 10.0f;	//��̃����[�h����


    PlArcher(const SceneManager::CNTL _cntl);
    PlArcher(const InputManager::JOYPAD_NO _padNum);
    ~PlArcher(void) = default;
    void SetParam(void)override;

    //�e�A�N�V�����̏�����
    void InitAct(void)override;

    //�ŗL�A�j���[�V�����ԍ��̏�����
    void InitCharaAnim(void)override;

    //�U�����I�������̏�����
    void InitAtk(void)override;

    //�|��̍쐬
    void CreateArrow(void);

    //arrowAtk�̍쐬
    void CreateAtk(void);

    void Update(void)override;

    void Draw(void)override;

protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;

    void NmlAtkInit(void)override;
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;

    //�|�A�˗p
    void NmlActCommon (void)override;

    //�|����atk������
    void InitArrowAtk(ATK& arrowAtk);

    std::vector<ATK>arrowAtk_;
    int arrowMdlId_;							//��̃��f��
    std::vector<std::shared_ptr<Arrow>> arrow_;	//�|��
    bool isShotArrow_;							//�����������̔���(true:������)
    int arrowCnt_;								//��̎g�p���J�E���^
    float reloadCnt_;							//��̃����[�h����

    float atkAbleCnt_;                          //��̔��ˉ\�J�E���g
};
