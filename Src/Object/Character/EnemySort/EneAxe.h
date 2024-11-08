#pragma once
#include"../Enemy.h"

class EneAxe : public Enemy
{
public:

	//�A�j���[�V�����ԍ�(�L�����ŗL)
	static constexpr int ANIM_SKILL_1 = 9;	//�X�L��1�A�j���[�V����
	static constexpr int ANIM_SKILL_2 = 11;	//�X�L��2�A�j���[�V����

	//���f���֌W
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,100.0f * CHARACTER_SCALE,0.0f };	//���f���̒��S���W�ւ̑��΍��W

	//�U���֌W
	static constexpr float ALERT_TIME = 120.0f;	//�U���̌x������
	static constexpr float BREAK_TIME = 100.0f;	//�U���̋x�e����

	//�G���g�̓����蔻�蔼�a
	static constexpr float MY_COL_RADIUS = 100.0f * CHARACTER_SCALE;

	//�G�X�e�[�^�X
	static constexpr int HP_MAX = 5;			//�G�̍ő�̗�
	static constexpr int STUN_DEF_MAX = 100;	//�G�̍ő�X�^���h��l

	//���x�֌W
	static constexpr float WALK_SPEED = 2.0f;	//�����̑��x
	static constexpr float RUN_SPEED = 4.0f;	//����̑��x

	//�͈͊֌W
	static constexpr float SEARCH_RANGE = 1000.0f * CHARACTER_SCALE;		//���G����̑傫��
	static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;		//�U���J�n����̑傫��

	//�X�L���̓����蔻�蔼�a
	static constexpr float SKILL_1_COL_RADIUS = 10.0f;	//�X�L���P
	static constexpr float SKILL_2_COL_RADIUS = 24.0f;	//�X�L���Q

	//�X�L���֌W
	static constexpr ATK SKILL_1 = { AsoUtility::VECTOR_ZERO,SKILL_1_COL_RADIUS,1.0f,60.0f,120.0f,0.0f };	//�X�L���P
	static constexpr ATK SKILL_2 = { AsoUtility::VECTOR_ZERO,SKILL_2_COL_RADIUS,5.0f,180.0f,300.0f,0.0f };	//�X�L���Q
	
	//�L�����ŗL�ݒ�
	void SetParam(void)override;

	//�A�j���[�V�����ԍ��̏�����
	void InitAnimNum(void)override;

	//�X�L���̏�����
	void InitSkill(void)override;

	//�x�����Ԓ����ǂ�����Ԃ�
	const bool IsAlertTime(void)const override{ return alertCnt_ < ALERT_TIME; }
	//�x�e���Ԓ����ǂ�����Ԃ�
	const bool IsBreak(void)const override { return breakCnt_ < BREAK_TIME; }

	//�G�̍U������
	void Attack(void)override;

	//�X�L��1
	void Skill_1(void)override;

	//�X�L��2
	void Skill_2(void)override;

};

