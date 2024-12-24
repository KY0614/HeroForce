#pragma once
#include"../Manager/SceneManager.h"
#include "../PlayerBase.h"
class PlKnight :
    public PlayerBase
{
public:
    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//�U���J�n����̑傫��
    //�X�L���g�p�J�n���ɃN�[���^�C���𑝂₷����
    static constexpr float SKILL_TWO_START_COOLTIME = 3.0f;

    //�K�[�h�\�ɂȂ�c��N�[���^�C��(�K�[�h����1�b�m�ۂ���������)
    static constexpr float GUARD_STARTABLE_COOL = 4.0f;
#ifdef DEBUG_COOL
//�N�[���^�C��
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 3.0f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;

#else // DEBUG_COOL
    //�N�[���^�C��
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 5.0f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;
#endif // DEBUG_COOL

    //�U�������̎���
    static constexpr float ATK_START = 0.2f;
    static constexpr float SKILL_ONE_START = 1.0f;
    static constexpr float SKILL_TWO_START = 0.3f;

    //�e�U���̎�������
    static constexpr float FRAME_ATK_DURATION = 0.4f - ATK_START;
    static constexpr float FRAME_SKILL1_DURATION = 0.7f;
    static constexpr float FRAME_SKILL2_DURATION = 3.0f /*- SKILL_TWO_START*/;
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.7f;

    //�㌄
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr float FRAME_SKILL1_BACKRASH = 0.2f;
    static constexpr float FRAME_SKILL2_BACKRASH = 0.0f;

    //�e�U���̍��W
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //�U���͈�
    static constexpr float COL_ATK = CHARACTER_SCALE * 100.0f;
    static constexpr float COL_SKILL1 = CHARACTER_SCALE * 150.0f;
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 400.0f;

    //�U���З�
    static constexpr float ATK_POW = 7.0f;
    static constexpr float SKILL_ONE_POW = 30.0f;
    static constexpr float SKILL_TWO_POW = 0.0f;

    //VECTOR pos_;		//�ʒu
    //float radius_;		//�����蔻��̔��a
    //float pow_;			//�Z�З�
    //float duration_;	//�������ԁi�U�����ǂꂭ�炢���������L�q)
    //float backlash_;	//�㌄�i�㌄���ǂꂭ�炢���������L�q)
    //float cnt_;			//�J�E���^�[
    //bool isHit_;

    //�ʏ�U���̍ő�l
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS
        ,COL_ATK
        ,ATK_POW
        ,FRAME_ATK_DURATION
        ,FRAME_ATK_BACKRASH
        ,0.0f,false };

    //�X�L���P
    static constexpr ATK SKILL_ONE_MAX{ SKILL1_COL_LOCAL_POS
        ,COL_SKILL1 
        ,SKILL_ONE_POW
        ,FRAME_SKILL1_DURATION 
        ,FRAME_SKILL1_BACKRASH 
        ,0.0f,false };

    //�X�L���Q
    static constexpr ATK SKILL_TWO_MAX{ SKILL2_COL_LOCAL_POS 
        ,COL_SKILL2 
        ,SKILL_TWO_POW
        ,FRAME_SKILL2_DURATION 
        ,FRAME_SKILL2_BACKRASH 
        ,0.0f,false };

    //�ŗL�A�j���[�V����
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_ONE_NUM = 2;
    static constexpr int SKILL_TWO_NUM = 18;
    static constexpr int BLOCKING_NUM = 21;
    PlKnight(const SceneManager::PLAY_MODE _mode, const SceneManager::CNTL _cntl);
    PlKnight(const SceneManager::PLAY_MODE _mode, const InputManager::JOYPAD_NO _padNum);
    ~PlKnight(void) = default;
    void SetParam(void)override;

protected:
    //�U���A�N�V�����̃p�����[�^�̏�����
    void InitAct(void)override;
    void AtkFunc(void)override;

    void ResetGuardCnt(void)override;

    //�e�X�L���̏���
    void Skill1Func(void)override;
    void Skill2Func(void)override;

    //�ŗL�A�j���[�V�����ԍ��̏�����
    void InitCharaAnim(void)override;

    //�������X�L��
    void ChargeAct(void)override;

private:
    float guardCnt_;        //�K�[�h����
};