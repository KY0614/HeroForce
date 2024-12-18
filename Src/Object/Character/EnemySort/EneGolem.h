#pragma once

#include "../Enemy.h"

class EneGolem : public Enemy
{
public:
	//****************************************************************
	//�萔(�L�����ŗL)
	//****************************************************************

	//�A�j���[�V�����ԍ�(�L�����ŗL)
	static constexpr int ANIM_IDLE = 1;		//�ҋ@�A�j���[�V����
	static constexpr int ANIM_WALK = 2;		//�����A�j���[�V����
	
	static constexpr int ANIM_PUNCH = 5;		//����A�j���[�V����(�X�L���P)
	static constexpr int ANIM_MOWDOWN = 6;		//�ガ�����A�j���[�V����(�X�L���Q)
	static constexpr int ANIM_SHOUT = 7;		//���уA�j���[�V����(�X�L���R)
	static constexpr int ANIM_JUMP_ATK = 8;		//�W�����v�U���A�j���[�V����(�X�L���S)
	
	static constexpr int ANIM_JUMP = 3;			//�W�����v�A�j���[�V����(�ŗL�P)
	static constexpr int ANIM_LANDING = 4;		//���n�A�j���[�V����(�ŗL�Q)

	//TODO:�̂��ɍ��
	//static constexpr int ANIM_ENTRY = 74;		//�o���A�j���[�V����
	//static constexpr int ANIM_DAMAGE = 39;	//�_���[�W�A�j���[�V����
	//static constexpr int ANIM_DEATH = 24;		//����A�j���[�V����

	//�A�j���[�V�������x
	static constexpr float SPEED_ANIM_IDLE = 60.0f;		//�ҋ@�A�j���[�V�������x
	static constexpr float SPEED_ANIM_WALK = 60.0f;		//�����A�j���[�V�������x
	static constexpr float SPEED_ANIM_PUNCH = 60.0f;	//�p���`�A�j���[�V�������x
	static constexpr float SPEED_ANIM_MOWDOWN = 60.0f;	//�ガ�����A�j���[�V�������x
	static constexpr float SPEED_ANIM_SHOUT = 60.0f;	//���уA�j���[�V�������x
	static constexpr float SPEED_ANIM_JUMP = 60.0f;		//�W�����v�A�j���[�V�������x

	//���f���֌W
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,100.0f * CHARACTER_SCALE,0.0f };	//���f���̒��S���W�ւ̑��΍��W
	static constexpr float SCALE = 0.01f;		//���f���̑傫��

	//�t���[��(�{�[��)�ԍ�
	static constexpr int FRAME_L_HAND = 24;		//���̎�̂Ђ�
	static constexpr int FRAME_L_FOOT = 46;		//���̑�
	
	static constexpr int FRAME_R_HAND = 38;		//�E�̎�̂Ђ�
	static constexpr int FRAME_R_FOOT = 50;		//�E�̑�
	
	static constexpr int FRAME_HEAD = 15;		//��

	//�U���֌W
	static constexpr float ALERT_TIME = 1.0f;	//�U���̌x������
	static constexpr float BREAK_TIME = 1.0f;	//�U���̋x�e����

	//�G���g�̓����蔻�蔼�a
	static constexpr float MY_COL_RADIUS = 300.0f * CHARACTER_SCALE;

	//�G�X�e�[�^�X
	static constexpr int HP_MAX = 150;			//�G�̍ő�̗�
	static constexpr float ATK_POW = 120.0f;	//�G�̍U��
	static constexpr float DEF = 100.0f;		//�G�̖h��
	static constexpr int STUN_DEF_MAX = 100;	//�G�̍ő�X�^���h��l

	//���x�֌W
	static constexpr float WALK_SPEED = 2.0f;	//�����̑��x
	static constexpr float RUN_SPEED = 4.0f;	//����̑��x

	//�͈͊֌W
	static constexpr float SEARCH_RANGE = 2000.0f * CHARACTER_SCALE;		//���G����̑傫��
	static constexpr float ATK_START_RANGE = 500.0f * CHARACTER_SCALE;		//�U���J�n����̑傫��

	//�X�L���̓����蔻�蔼�a
	static constexpr float SKILL_ONE_COL_RADIUS = 50.0f;	//�X�L���P�̓����蔻�蔼�a
	//�X�L���̍U����
	static constexpr float SKILL_ONE_POW = 20.0f;			//�X�L���P�̍U����
	//�X�L���̎�������
	static constexpr float SKILL_ONE_DURATION = 1.0f;		//�X�L���P�̎�������
	//�X�L���̌㌄
	static constexpr float SKILL_ONE_BACKLASH = 2.0f;		//�X�L���P�̌㌄
	//�X�L���֌W
	static constexpr ATK SKILL_ONE = { AsoUtility::VECTOR_ZERO	//�X�L���P�̗v�f
		,SKILL_ONE_COL_RADIUS
		,SKILL_ONE_POW
		,SKILL_ONE_DURATION
		,SKILL_ONE_BACKLASH
		,0.0f };

	//�X�L���̓����蔻�蔼�a
	static constexpr float SKILL_TWO_COL_RADIUS = 50.0f;	//�X�L���Q�̓����蔻�蔼�a
	//�X�L���̍U����
	static constexpr float SKILL_TWO_POW = 20.0f;			//�X�L���Q�̍U����
	//�X�L���̎�������
	static constexpr float SKILL_TWO_DURATION = 1.0f;		//�X�L���Q�̎�������
	//�X�L���̌㌄
	static constexpr float SKILL_TWO_BACKLASH = 2.0f;		//�X�L���Q�̌㌄
	//�X�L���֌W
	static constexpr ATK SKILL_TWO = { AsoUtility::VECTOR_ZERO	//�X�L���Q�̗v�f
		,SKILL_TWO_COL_RADIUS
		,SKILL_TWO_POW
		,SKILL_TWO_DURATION
		,SKILL_TWO_BACKLASH
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

	//�G�̍U������
	void Attack(void)override;

	//�X�L��1
	void Skill_One(void)override;

	//�X�L��2
	void Skill_Two(void);

	//��ԑJ��(�U���x��)
	void ChangeStateAlert(void)override;
};

