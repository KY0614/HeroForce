#pragma once
#include"../Enemy.h"

class EneBrig : public Enemy
{
public:
	//****************************************************************
	//�萔(�L�����ŗL)
	//****************************************************************

	//�A�j���[�V�����ԍ�(�L�����ŗL)
	static constexpr int ANIM_SKILL_ONE = 96;	//�X�L��1�A�j���[�V����(�U�肩����)
	static constexpr int ANIM_PRE_SWING = 95;	//�X�L��1�A�j���[�V����(�U�肩�Ԃ�)

	//���f���֌W
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,100.0f * CHARACTER_SCALE,0.0f };	//���f���̒��S���W�ւ̑��΍��W

	//�t���[��(�{�[��)�ԍ�
	static constexpr int FRAME_KNIFE = 63;		//�i�C�t

	//�U���֌W
	static constexpr float ALERT_TIME = 0.4f;	//�U���̌x������
	static constexpr float BREAK_TIME = 0.8f;	//�U���̋x�e����

	//�G���g�̓����蔻�蔼�a
	static constexpr float MY_COL_RADIUS = 100.0f * CHARACTER_SCALE;

	//�G�X�e�[�^�X
	static constexpr int HP_MAX = 150;			//�G�̍ő�̗�
	static constexpr float ATK_POW = 120.0f;	//�G�̍U��
	static constexpr float DEF = 100.0f;		//�G�̖h��
	static constexpr int STUN_DEF_MAX = 100;	//�G�̍ő�X�^���h��l
	static constexpr float EXP = 300.0f;		//�G�̌o���l

	//���x�֌W
	static constexpr float WALK_SPEED = 3.0f;	//�����̑��x
	static constexpr float RUN_SPEED = 4.0f;	//����̑��x

	//�͈͊֌W
	static constexpr float SEARCH_RANGE = 1000.0f * CHARACTER_SCALE;		//���G����̑傫��
	static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;		//�U���J�n����̑傫��

	//�X�L���̓����蔻�蔼�a
	static constexpr float SKILL_ONE_COL_RADIUS = 15.0f;	//�X�L���P�̓����蔻�蔼�a

	//�X�L���̍U����
	static constexpr float SKILL_ONE_POW = 20.0f;			//�X�L���P�̍U����

	//�X�L���̎�������
	static constexpr float SKILL_ONE_DURATION = 1.0f;		//�X�L���P�̎�������

	//�X�L���̌㌄
	static constexpr float SKILL_ONE_BACKLASH = 1.0f;		//�X�L���P�̌㌄

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
	void InitAnim(void)override;

	//�X�L���̏�����
	void InitSkill(void)override;

	//�x�����Ԓ����ǂ�����Ԃ�
	const bool IsAlertTime(void)const override { return alertCnt_ < ALERT_TIME; }
	//�x�e���Ԓ����ǂ�����Ԃ�
	const bool IsBreak(void)const override { return breakCnt_ < BREAK_TIME; }

	//�X�L��1�̌x��
	void AlertSkill_One(void)override;

	//�X�L��1
	void Skill_One(void)override;

	//�A�j���[�V�����I�����̓���
	void FinishAnim(void)override;
};

