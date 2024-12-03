#pragma once
#include "../Enemy.h"

class EneMage : public Enemy
{
public:
	//****************************************************************
	//�萔(�L�����ŗL)
	//****************************************************************

	//�A�j���[�V�����ԍ�(�L�����ŗL)
	static constexpr int ANIM_SKILL_ONE = 75;	//�X�L��1�A�j���[�V����
	static constexpr int ANIM_CHARGE = 79;		//���߃A�j���[�V����(�ŗL�A�j���[�V����)

	//���f���֌W
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,100.0f * CHARACTER_SCALE,0.0f };	//���f���̒��S���W�ւ̑��΍��W

	//�U���֌W
	static constexpr float ALERT_TIME = 2.5f;	//�U���̌x������
	static constexpr float BREAK_TIME = 4.0f;	//�U���̋x�e����

	//�G���g�̓����蔻�蔼�a
	static constexpr float MY_COL_RADIUS = 100.0f * CHARACTER_SCALE;

	//�G�X�e�[�^�X
	static constexpr int HP_MAX = 140;			//�G�̍ő�̗�
	static constexpr float ATK_POW = 0.0f;		//�G�̍U��
	static constexpr float DEF = 150.0f;		//�G�̖h��
	static constexpr int STUN_DEF_MAX = 100;	//�G�̍ő�X�^���h��l

	//���x�֌W
	static constexpr float WALK_SPEED = 2.0f;	//�����̑��x
	static constexpr float RUN_SPEED = 4.0f;	//����̑��x

	//�͈͊֌W
	static constexpr float SEARCH_RANGE = 1500.0f * CHARACTER_SCALE;		//���G����̑傫��
	static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;		//�U���J�n����̑傫��

	//�X�L���̎˒�
	static constexpr float SKILL_ONE_RANGE = 40.0f;			//�X�L���P�̎˒�

	//�X�L���̓����蔻�蔼�a
	static constexpr float SKILL_ONE_COL_RADIUS = 10.0f;	//�X�L���P�̓����蔻�蔼�a

	//�X�L���̍U����
	static constexpr float SKILL_ONE_POW = 0.0f;			//�X�L���P�̍U����

	//�X�L���̎�������
	static constexpr float SKILL_ONE_DURATION = 3.0f;		//�X�L���P�̎�������

	//�X�L���̌㌄
	static constexpr float SKILL_ONE_BACKLASH = 4.5f;		//�X�L���P�̌㌄

	//�X�L���֌W
	static constexpr ATK SKILL_ONE = { AsoUtility::VECTOR_ZERO	//�X�L���P�̗v�f
		,SKILL_ONE_COL_RADIUS
		,SKILL_ONE_POW
		,SKILL_ONE_DURATION
		,SKILL_ONE_BACKLASH
		,0.0f };

private:
	//****************************************************************
	//�֐�
	//****************************************************************

	//�L�����ŗL�ݒ�
	void SetParam(void)override;

	//�A�j���[�V�����ԍ��̏�����
	void InitAnimNum(void)override;

	//�X�L���̏�����
	void InitSkill(void)override;

	//�x�����Ԓ����ǂ�����Ԃ�
	const bool IsAlertTime(void)const override { return alertCnt_ < ALERT_TIME; }
	//�x�e���Ԓ����ǂ�����Ԃ�
	const bool IsBreak(void)const override { return breakCnt_ < BREAK_TIME; }

	//�G�̍U������
	void Attack(void)override;

	//�X�L��1
	void Skill_One(void)override;

	//�A�j���[�V�����I�����̓���
	void FinishAnim(void)override;

	//��ԑJ��(�U���x��)
	void ChangeStateAlert(void)override;
};

