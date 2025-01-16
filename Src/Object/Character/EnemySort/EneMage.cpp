#include"../../../Manager/Resource.h"
#include"../../../Manager/ResourceManager.h"
#include"../Enemy.h"
#include "EneMage.h"

void EneMage::SetParam(void)
{
	//�U���̑J��
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneMage::Skill_One, this));

	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MAGE));

	//���X�Őݒ肷��
	trans_.scl = { CHARACTER_SCALE,CHARACTER_SCALE,CHARACTER_SCALE };
	radius_ = MY_COL_RADIUS;
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	hp_ = HP_MAX;
	atkPow_ = ATK_POW;
	def_ = DEF;
	walkSpeed_ = WALK_SPEED;
	localCenterPos_ = LOCAL_CENTER_POS;
	stunDefMax_ = STUN_DEF_MAX;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
	
	skillOneShot_ = AsoUtility::VECTOR_ZERO;
	skillOneDelayCnt_ = 0.0f;
	skillAllCnt_ = 0.0f;
}

void EneMage::InitAnim(void)
{
	//���ʃA�j���[�V����������
	Enemy::InitAnim();

	//�ŗL�A�j���[�V����������
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_CHARGE);

	//�A�j���[�V�������x�ݒ�
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_1, SPEED_ANIM);

	//�A�j���[�V�������Z�b�g
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneMage::InitSkill(void)
{
	//�����ɃX�L���̐����i�[������
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);

	//�����ɃX�L���̐����A�j���[�V�������i�[������
	skillAnims_.emplace_back(ANIM::SKILL_1);

	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void EneMage::Attack(void)
{
	//���ʏ���
	Enemy::Attack();

	//�X�L���S�̂̃J�E���g
	CntUp(skillAllCnt_);
}

void EneMage::Skill_One(void)
{
	//�X�L���P�̐������
	if (skillOneShotCnt_ < SKILL_ONE_MAX_CNT)
	{
		//���W�̐ݒ�
		skillOneShot_ = VAdd(skillOneShot_, GetTargetVec(skillOneShot_, SKILL_ONE_SPEED));

		//�X�L���P�̔����Ԋu
		if (skillOneDelayCnt_ >= SKILL_ONE_SHOT_DELAY)
		{
			//�U������
			//----------------------------

			//�X�L���̎������ԏ�����
			skillOneDelayCnt_ = 0.0f;

			//�U���񐔑���
			skillOneShotCnt_++;

			//�U���쐬
			ATK& thisAtk = createSkill_();

			//���������U���̈ʒu�����킹��
			thisAtk.pos_ = skillOneShot_;
		}
	}

	//�J�E���^
	CntUp(skillOneDelayCnt_);
}

void EneMage::DrawDebug(void)
{
	Enemy::DrawDebug();
	DrawSphere3D(skillOneShot_, 25.0f, 20, 0xf0f0f0, 0xf0f0f0, true);
}

void EneMage::FinishAnim(void)
{
	//���ʃA�j���[�V�����̏I������
	Enemy::FinishAnim();

	switch (anim_)
	{
	case UnitBase::ANIM::UNIQUE_1:
		//���[�v�Đ�
		stepAnim_ = 0;
		break;
	}
}

void EneMage::ChangeStateAlert(void)
{
	//�X�V�����̒��g������
	Enemy::ChangeStateAlert();

	//���������߂Đݒ�
	trans_.quaRot = trans_.quaRot.LookRotation(GetTargetVec());

	//�ҋ@�A�j���[�V����
	ResetAnim(ANIM::UNIQUE_1, changeSpeedAnim_[ANIM::UNIQUE_1]);
}

void EneMage::ChangeStateAtk(void)
{
	//�X�V�����̒��g������
	stateUpdate_ = std::bind(&EneMage::UpdateAtk, this);

	//���������߂Đݒ�
	trans_.quaRot = trans_.quaRot.LookRotation(GetTargetVec());

	//���W�̏����ݒ�
	skillOneShot_ = MV1GetFramePosition(trans_.modelId, FRAME_ROD);
}

void EneMage::UpdateAtk(void)
{
	//**********************************************************
	//�I������
	//**********************************************************

	//�U�����I����Ă���Ȃ��ԑJ��
	if (skillAllCnt_ > SKILL_ONE_ALL_TIME)
	{
		//�X�L���̎������ԏ�����
		skillOneDelayCnt_ = 0.0f;

		//�U���񐔏�����
		skillOneShotCnt_ = 0;

		//�X�L���̃J�E���^������
		skillAllCnt_ = 0.0f;

		//�x�e��ԂɑJ��
		ChangeState(STATE::BREAK);
		return;
	}

	//**********************************************************
	//���쏈��
	//**********************************************************

	//�U���A�j���[�V����
	ResetAnim(nowSkillAnim_, changeSpeedAnim_[nowSkillAnim_]);

	//�U������
	Attack();
}
