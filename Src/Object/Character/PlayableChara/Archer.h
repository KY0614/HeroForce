#pragma once

#include "PlayerBase.h"
class Arrow;
class Archer :
    public PlayerBase
{
public:
#ifdef DEBUG_ON
    void DrawDebug(void)override;
#endif // DEBUG_ON

    //�X�e�[�^�X
    static constexpr int ATK_POW = 90;
    static constexpr int MAX_HP = 235;
    static constexpr int MAX_DEF = 90;
    static constexpr float SPEED = PlayerBase::MOVE_SPEED_FAST;

    //�o�t�����l
    static constexpr float BUFF = 20.0f;

    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//�U���J�n����̑傫��
    static constexpr float ATKABLE_TIME = 0.0f;                         //�A�ˎ�t����

    //�G�t�F�N�g�T�C�Y
    static constexpr float ARROW_EFFECT_SIZE = 20.0f;
    //�ʏ�U��
    //-----------------------------------------------------------
    //�N�[���^�C��
    static constexpr float NORMAL_ATK_COOLTIME = 0.3f;
    //�O��
    static constexpr float NORMAL_ATK_START = 0.3f;
    //��̐�������
    static constexpr float NORMAL_ATK_DURATION = 2.0f;
    //�㌄
    static constexpr float NORMAL_ATK_BACKRASH = 0.1f;
    //�U�����W�̑��΍��W
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    //�U����
    static constexpr float NORMAL_ATK_POW = 7.0f;
    //�����蔻��̑傫��
    static constexpr float NORMAL_ATK_COL = CHARACTER_SCALE * 100.0f;

    //�v���C���[���̂̌㌄
    static constexpr float NMLATK_BACKRASH_MAX = 0.2f;
    //-----------------------------------------------------------
    //�X�L��1
    //-----------------------------------------------------------
    //�N�[���^�C��
    static constexpr float SKILL_ONE_COOLTIME = 3.0f;
    //�O��
    static constexpr float SKILL_ONE_START = 2.0f;
    //�З͂������Ȃ闭�ߎ���
    static constexpr float SKILL_ONE_CHARGE_START_CNT = 1.0f;
    //�U������
    static constexpr float SKILL_ONE_DURATION = 0.7f;
    //�㌄
    static constexpr float SKILL_ONE_BACKRASH = 0.2f;
    //�U�����΍��W
    static constexpr VECTOR SKILL_ONE_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    //�����蔻��̑傫��
    static constexpr float SKILL_ONE_COL = CHARACTER_SCALE * 150.0f;
    //�v���C���[���̂̌㌄
    static constexpr float SKILL_ONE_BACKRASH_MAX = 0.5f;

    //�X�L���Q�ŗL�̂���
    //���߂����̈З�
    static constexpr float SKILL_ONE_POW_MIN = 15.0f;
    //���߂Ȃ��������̈З�
    static constexpr float SKILL_ONE_POW_MAX = 25.0f;

    //���߃A�j���[�V�����̃X�e�b�v
    static constexpr float SKILL_CHARGE_STEPANIM = 9.5f;
    //-----------------------------------------------------------
    //�X�L��2
    //-----------------------------------------------------------
    //�N�[���^�C��
    static constexpr float SKILL_TWO_COOLTIME = 5.0f;
    //�O��
    static constexpr float SKILL_TWO_START = 0.2f;
    //�s������
    static constexpr float SKILL_TWO_DURATION = 3.0f - SKILL_TWO_START;
    //�㌄
    static constexpr float SKILL_TWO_BACKRASH = 0.2f;
    //�U���̑��΍��W
    static constexpr VECTOR SKILL_TWO_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };
    //�U���̓����蔻��
    static constexpr float SKILL_TWO_COL = CHARACTER_SCALE * 200.0f;
    //�o�t�X�L���Ȃ̂ōU���͂�0
    static constexpr float SKILL_TWO_POW = 0.0f;
    //�o�t��������
    static constexpr float SKILL_TWO_BUFF_TIME = 20.0f;
    //�㌄
    static constexpr float SKILL_TWO_BACKRASH_MAX = 0.2f;
    //-----------------------------------------------------------
    // 
    //�ŗL�A�j���[�V����
    static constexpr int ATK_NUM = 16;
    static constexpr int SKILL_ONE_NUM = 16;
    static constexpr int SKILL_TWO_NUM = 16;

    //�ʏ�U���̍ő�l
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS,NORMAL_ATK_COL,NORMAL_ATK_POW,NORMAL_ATK_DURATION,NORMAL_ATK_BACKRASH,0.0f,false };

    //�X�L���P
    static constexpr ATK SKILL_ONE_MAX{ SKILL_ONE_COL_LOCAL_POS ,SKILL_ONE_COL ,SKILL_ONE_POW_MIN,SKILL_ONE_DURATION ,SKILL_ONE_BACKRASH ,0.0f,false };

    //�X�L���Q
    static constexpr ATK SKILL_TWO_MAX{ SKILL_TWO_COL_LOCAL_POS ,SKILL_TWO_COL ,SKILL_TWO_POW,SKILL_TWO_DURATION ,SKILL_TWO_BACKRASH ,0.0f,false };

    //�|��֌W
    //��̍ő�ێ���
    static constexpr int ARROW_SIZE_MAX = 5;		
    //��̂ƂԃX�s�[�h
    static constexpr float ARROW_SPEED = 10.0f;	

    //�X�L���`���[�W�G�t�F�N�g�̑傫��
    static constexpr float CHARGE_SKILL_EFF_SIZE = 50.0f;
 
    //�֐�
    //********************************************
    Archer(void);
    ~Archer(void) = default;
    void SetParam(void)override;

    //�e�A�N�V�����̏�����
    void InitAct(void)override;

    //�ŗL�A�j���[�V�����ԍ��̏�����
    void InitCharaAnim(void)override;

    //�U�����I�������̏�����
    void InitAtk(void)override;

    //�|��̍쐬
    void CreateArrow(ATK_TYPE _type);

    //arrowAtk�̍쐬
    void CreateAtk(ATK_TYPE _type);

    //��֘A�̃Q�b�^
    //-------------------------------------------------
    //ATK
    const PlayerBase::ATK GetArrowAtk(const ATK_TYPE _type, const int i)override;
    //��̌�
    const int GetArrowCnt(const int _type)override { return arrow_[static_cast<ATK_TYPE>(_type)].size(); }
    //��̃q�b�g
    void SetIsArrowHit(ATK_TYPE _type, const bool _flg, int _num)override;
    //��̓����蔻��ŕK�v
    std::vector<ATK> GetAtks(ATK_TYPE _type)override { return arrowAtk_[_type]; }
    //-------------------------------------------------
    //�o�t
    void Buff(PlayerBase& _target)override;

    //���
    void Destroy(void)override;
    //�X�V
    void Update(void)override;
    //�`��
    void Draw(void)override;
    //********************************************

protected:
    //�����o�֐�
    //*************************************************
    //�ʏ�U��
    void AtkFunc(void)override;
    //�X�L���P
    void Skill1Func(void)override;
    //�X�L���Q
    void Skill2Func(void)override;

    //�e�A�N�V�����̏���������
    void NmlAtkInit(void)override;
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;

    //��Ɩ�ɑΉ������U���ɑΉ�����X�V����(�X�L���ɖ��O��t���Ă���Ă݂�)
    void ArrowUpdate(ATK_TYPE _type);

    //�|����atk������
    void InitArrowAtk(ATK& arrowAtk);
    //*************************************************
    //�����o�ϐ�
    //*************************************************
    std::map<ATK_TYPE, std::vector<ATK>>arrowAtk_;  //���ATK
    int arrowMdlId_;							//��̃��f��
    std::map<ATK_TYPE, std::vector<std::shared_ptr<Arrow>>> arrow_;	//�|��
    bool isShotArrow_;							//�����������̔���(true:������)

    //�e�U���̖�̌��J�E���g
    int arrowCnt_[static_cast<int>(ATK_TYPE::MAX)];	//��̎g�p���J�E���^
    float backrashCnt_;                             //�A�[�`���[���̂̌㌄
    //*************************************************
};

