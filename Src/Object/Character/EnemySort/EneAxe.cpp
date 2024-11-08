#include"../../../Manager/Resource.h"
#include"../../../Manager/ResourceManager.h"
#include"../Enemy.h"
#include "EneAxe.h"

void EneAxe::SetParam(void)
{
	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_AXEMAN));

	//���X�Őݒ肷��
	radius_ = MY_COL_RADIUS;
	localCenterPos_ = LOCAL_CENTER_POS;
	colPos_ = VAdd(trans_.pos, localCenterPos_);
	hp_ = 5;
	walkSpeed_ = WALK_SPEED;
	stunDefMax_ = 100;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
}

void EneAxe::InitAnimNum(void)
{
	//���ʃA�j���[�V����������
	Enemy::InitAnimNum();

	//�ŗL�A�j���[�V����������
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_1);
	animNum_.emplace(ANIM::SKILL_2, ANIM_SKILL_2);
}

void EneAxe::InitSkill(void)
{
	//�����ɃX�L���̐����i�[������
	skills_.emplace_back(SKILL_1);
	skills_.emplace_back(SKILL_2);

	//�����ɃX�L���̐����A�j���[�V�������i�[������
	skillAnims_.emplace_back(ANIM::SKILL_1);
	skillAnims_.emplace_back(ANIM::SKILL_2);

	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void EneAxe::Attack(void)
{
	//���݂̃X�L���̊m�F
	if (nowSkill_.front().radius_ == SKILL_1.radius_
		&& nowSkill_.front().backlash_ == SKILL_1.backlash_
		&& nowSkill_.front().duration_ == SKILL_1.duration_
		&& nowSkill_.front().pow_ == SKILL_1.pow_)
	{
		//�X�L��1����
		Skill_1();
	}
	else if (nowSkill_.front().radius_ == SKILL_2.radius_
		&& nowSkill_.front().backlash_ == SKILL_2.backlash_
		&& nowSkill_.front().duration_ == SKILL_2.duration_
		&& nowSkill_.front().pow_ == SKILL_2.pow_)
	{
		//�X�L��2����
		Skill_2();
	}
}

void EneAxe::Skill_1(void)
{
	//�O����
	VECTOR dir = trans_.quaRot.GetForward();

	for (auto& nowSkill : nowSkill_)
	{
		//���W�̐ݒ�
		nowSkill.pos_ = VAdd(colPos_, VScale(dir, nowSkill.radius_));
	}
}

void EneAxe::Skill_2(void)
{
	//�O����
	VECTOR dir = trans_.quaRot.GetForward();

	for (auto& nowSkill : nowSkill_)
	{
		//���W�̐ݒ�
		nowSkill.pos_ = VAdd(colPos_, VScale(dir, nowSkill.radius_));
	}
}
