#include"../../../Manager/Resource.h"
#include"../../../Manager/ResourceManager.h"
#include"../Enemy.h"
#include "EneMage.h"

void EneMage::SetParam(void)
{
	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MAGE));

	//���X�Őݒ肷��
	radius_ = MY_COL_RADIUS;
	localCenterPos_ = LOCAL_CENTER_POS;
	colPos_ = VAdd(trans_.pos, localCenterPos_);
	hp_ = HP_MAX;
	walkSpeed_ = WALK_SPEED;
	stunDefMax_ = STUN_DEF_MAX;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
}

void EneMage::InitAnimNum(void)
{
	//���ʃA�j���[�V����������
	Enemy::InitAnimNum();

	//�ŗL�A�j���[�V����������
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
	animNum_.emplace(ANIM::SKILL_2, ANIM_SKILL_TWO);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_CHARGE);
}

void EneMage::InitSkill(void)
{
	//�����ɃX�L���̐����i�[������
	skills_.emplace_back(SKILL_ONE);
	skills_.emplace_back(SKILL_TWO);

	//�����ɃX�L���̐����A�j���[�V�������i�[������
	skillAnims_.emplace_back(ANIM::SKILL_1);
	skillAnims_.emplace_back(ANIM::SKILL_2);

	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void EneMage::Attack(void)
{
	//���݂̃X�L���̊m�F
	if (nowSkill_.front().radius_ == SKILL_ONE.radius_
		&& nowSkill_.front().backlash_ == SKILL_ONE.backlash_
		&& nowSkill_.front().duration_ == SKILL_ONE.duration_
		&& nowSkill_.front().pow_ == SKILL_ONE.pow_)
	{
		//�X�L��1����
		Skill_One();
	}
	else if (nowSkill_.front().radius_ == SKILL_TWO.radius_
		&& nowSkill_.front().backlash_ == SKILL_TWO.backlash_
		&& nowSkill_.front().duration_ == SKILL_TWO.duration_
		&& nowSkill_.front().pow_ == SKILL_TWO.pow_)
	{
		//�X�L��2����
		Skill_Two();
	}
}

void EneMage::Skill_One(void)
{
	//�O����
	VECTOR dir = trans_.quaRot.GetForward();

	for (auto& nowSkill : nowSkill_)
	{
		//���W�̐ݒ�
		nowSkill.pos_ = VAdd(colPos_, VScale(dir, nowSkill.radius_ + radius_));
	}
}

void EneMage::Skill_Two(void)
{
	//�O����
	VECTOR dir = trans_.quaRot.GetForward();

	for (auto& nowSkill : nowSkill_)
	{
		//���W�̐ݒ�
		nowSkill.pos_ = VAdd(colPos_, VScale(dir, nowSkill.radius_ + radius_));
	}
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

void EneMage::InitChangeState(void)
{
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		break;

	case Enemy::STATE::ALERT:
		//���������߂Đݒ�
		trans_.quaRot = trans_.quaRot.LookRotation(GetTargetVec());

		//���߃A�j���[�V����
		ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM);

		//�x���J�E���^������
		alertCnt_ = 0.0f;
		break;

	case Enemy::STATE::ATTACK:
		break;

	case Enemy::STATE::BREAK:
		//�U���x�e���Ԃ̏�����
		breakCnt_ = 0;
		break;
	}
}