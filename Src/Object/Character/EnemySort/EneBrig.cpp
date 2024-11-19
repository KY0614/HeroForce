#include"../../../Manager/Resource.h"
#include"../../../Manager/ResourceManager.h"
#include"../Enemy.h"
#include "EneBrig.h"

void EneBrig::SetParam(void)
{
	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_BRIGANT));

	//���X�Őݒ肷��
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
}

void EneBrig::InitAnimNum(void)
{
	//���ʃA�j���[�V����������
	Enemy::InitAnimNum();

	//�ŗL�A�j���[�V����������
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
}

void EneBrig::InitSkill(void)
{
	//�����ɃX�L���̐����i�[������
	skills_.emplace_back(SKILL_ONE);

	//�����ɃX�L���̐����A�j���[�V�������i�[������
	skillAnims_.emplace_back(ANIM::SKILL_1);

	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void EneBrig::Attack(void)
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
}

void EneBrig::Skill_One(void)
{
	//�O����
	VECTOR dir = trans_.quaRot.GetForward();

	for (auto& nowSkill : nowSkill_)
	{
		//���W�̐ݒ�
		nowSkill.pos_ = VAdd(colPos_, VScale(dir, nowSkill.radius_ + radius_));
	}
}

void EneBrig::InitChangeState(void)
{
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		break;

	case Enemy::STATE::ALERT:
		//���������߂Đݒ�
		trans_.quaRot = trans_.quaRot.LookRotation(GetMovePow2Target());

		//�ҋ@�A�j���[�V����
		ResetAnim(ANIM::IDLE, SPEED_ANIM);

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