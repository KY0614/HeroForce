#pragma once
#include"../Manager/SceneManager.h"
#include "../PlayerBase.h"
class PlKnight :
    public PlayerBase
{
public:
    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//�U���J�n����̑傫��

#ifdef DEBUG_ON
//�N�[���^�C��
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 3.0f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;
#else // DEBUG_ON
    //�N�[���^�C��
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 5.0f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;
#endif // DEBUG_ON

    //�U�������̎���
    static constexpr float ATK_START = 0.2f;
    static constexpr float SKILL_ONE_START = 1.0f;
    static constexpr float SKILL_TWO_START = 0.2f;

    //�e�U���̎�������
    static constexpr float FRAME_ATK_DURATION = 0.4f - ATK_START;
    static constexpr float FRAME_SKILL1_DURATION = 0.7f;
    static constexpr float FRAME_SKILL2_DURATION = 3.0f /*- SKILL_TWO_START*/;
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.7f;

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
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 400.0f;



    //�ŗL�A�j���[�V����
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_ONE_NUM = 2;
    static constexpr int SKILL_TWO_NUM = 18;
    static constexpr int BLOCKING_NUM = 21;
    PlKnight(SceneManager::PLAY_MODE _mode, InputManager::JOYPAD_NO _padNum);
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

private:
    float guardCnt_;        //�K�[�h����
};