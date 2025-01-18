#pragma once

#include "../Enemy.h"

class EneGolem : public Enemy
{
public:
	//****************************************************************
	//�萔(�L�����ŗL)
	//****************************************************************

	//TODO:�S�[�����̃e�N�X�`���p�X���m�F���邱��

	//�A�j���[�V�����ԍ�(�L�����ŗL)
	static constexpr int ANIM_IDLE = 1;		//�ҋ@�A�j���[�V����
	static constexpr int ANIM_WALK = 2;		//�����A�j���[�V����
	
	static constexpr int ANIM_PUNCH = 6;		//����A�j���[�V����(�X�L���P)
	static constexpr int ANIM_MOWDOWN = 8;		//�ガ�����A�j���[�V����(�X�L���Q)
	static constexpr int ANIM_SHOUT = 10;		//���уA�j���[�V����(�X�L���R)
	static constexpr int ANIM_JUMP_ATK = 11;	//�W�����v�U���A�j���[�V����(�X�L���S)
	
	static constexpr int ANIM_JUMP = 3;			//�W�����v�A�j���[�V����(�ŗL�P)
	static constexpr int ANIM_LANDING = 4;		//���n�A�j���[�V����(�ŗL�Q)
	static constexpr int ANIM_PRE_PUNCH = 5;	//���菀���A�j���[�V����(�ŗL�R)
	static constexpr int ANIM_PRE_MOWDOWN = 7;	//�ガ���������A�j���[�V����(�ŗL�S)
	static constexpr int ANIM_PRE_SHOUT = 9;	//���я����A�j���[�V����(�ŗL�T)

	//TODO:�̂��ɍ��
	//static constexpr int ANIM_ENTRY = 74;		//�o���A�j���[�V����
	//static constexpr int ANIM_DAMAGE = 39;	//�_���[�W�A�j���[�V����
	//static constexpr int ANIM_DEATH = 24;		//����A�j���[�V����

	//�A�j���[�V�������x
	static constexpr float SPEED_ANIM_IDLE = 60.0f;			//�ҋ@�A�j���[�V�������x
	static constexpr float SPEED_ANIM_WALK = 60.0f;			//�����A�j���[�V�������x
	static constexpr float SPEED_ANIM_PUNCH = 60.0f;		//�p���`�A�j���[�V�������x
	static constexpr float SPEED_ANIM_PRE_PUNCH = 60.0f;	//�p���`�O�A�j���[�V�������x
	static constexpr float SPEED_ANIM_MOWDOWN = 60.0f;		//�ガ�����A�j���[�V�������x
	static constexpr float SPEED_ANIM_PRE_MOWDOWN = 30.0f;	//�ガ�����܂��A�j���[�V�������x
	static constexpr float SPEED_ANIM_SHOUT = 60.0f;		//���уA�j���[�V�������x
	static constexpr float SPEED_ANIM_JUMP = 60.0f;			//�W�����v�A�j���[�V�������x

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
	static constexpr float ALERT_TIME = 0.8f;	//�U���̌x������
	static constexpr float BREAK_TIME = 0.5f;	//�U���̋x�e����

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
	static constexpr float ATK_START_RANGE = 700.0f * CHARACTER_SCALE;		//�U���J�n����̑傫��

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

	//�X�L���̓����蔻�蔼�a
	static constexpr float SKILL_THREE_COL_RADIUS = 50.0f;	//�X�L���R�̓����蔻�蔼�a
	//�X�L���̍U����
	static constexpr float SKILL_THREE_POW = 20.0f;			//�X�L���R�̍U����
	//�X�L���̎�������
	static constexpr float SKILL_THREE_DURATION = 1.0f;		//�X�L���R�̎�������
	//�X�L���̌㌄
	static constexpr float SKILL_THREE_BACKLASH = 3.0f;		//�X�L���R�̌㌄
	//�X�L���֌W
	static constexpr ATK SKILL_THREE = { AsoUtility::VECTOR_ZERO	//�X�L���R�̗v�f
		,SKILL_THREE_COL_RADIUS
		,SKILL_THREE_POW
		,SKILL_THREE_DURATION
		,SKILL_THREE_BACKLASH
		,0.0f };

	//�X�L���R�̌��ʔ͈�
	static constexpr float SKILL_THREE_FALL_RADIUS = 300.0f;	//�X�L���R��覐΂̗�����͈͂̔��a

	//�U�������Ԋu
	static constexpr float SKILL_THREE_DELAY = 2.0f;			//�X�L���R�̍U�������Ԋu

	//�U����
	static constexpr int SKILL_THREE_MAX_CNT = 5;				//�X�L���R�̍U����

private:	
	//****************************************************************
	//�ϐ�
	//****************************************************************

	int skillThreeCnt_;			//�X�L���R�̐����J�E���^
	float skillThreeDelayCnt_;	//�X�L���R�̔����Ԋu�p�J�E���^

	////�X�L�����Ƃ̏�ԑJ��
	//std::map<ATK_ACT, std::function<void(void)>> changeStateAlert_;		//�X�L�����Ƃ̏�ԑJ��(�x��)

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

	//�X�L��1(����)
	void Skill_One(void)override;

	//�X�L��2(�ガ����)
	void Skill_Two(void);

	//�X�L��3(���эU��)
	void Skill_Three(void);

	/// <summary>
	/// �~�͈͂̃����_����_�����
	/// </summary>
	/// <param name="_myPos">�U���҂̍��W</param>
	/// <param name="_r">�U�������͈�</param>
	/// <param name="_tPos">�U�����̂̍��W</param>
	void GetRandomPointInCircle(const VECTOR _myPos, const int _r, VECTOR& _tPos);

	/// <summary>
	/// �U���B���d�Ȃ��Ă��Ȃ���
	/// </summary>
	/// <param name="_tPos">�U�����̂̍��W</param>
	/// <param name="_minDist">�U���͈͂̒��a</param>
	/// <returns>�U�����d�Ȃ�����(true:�d�Ȃ���)</returns>
	bool IsOverlap(VECTOR _tPos, float _minDist);
};
