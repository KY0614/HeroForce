#include"../Enemy.h"
#include "EneBrig.h"

void EneBrig::SetParam(void)
{
	//�U���̑J��
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneBrig::Skill_One, this));

	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_BRIGANT));

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
}

void EneBrig::InitAnim(void)
{
	//���ʃA�j���[�V����������
	Enemy::InitAnim();

	//�ŗL�A�j���[�V����������
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);

	//�A�j���[�V�������x�ݒ�
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM);

	//�A�j���[�V�������Z�b�g
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneBrig::InitSkill(void)
{
	//�����ɃX�L���̐����i�[������
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);

	//�����ɃX�L���̐����A�j���[�V�������i�[������
	skillAnims_.emplace_back(ANIM::SKILL_1);

	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void EneBrig::Attack(void)
{
	//�Ή��X�L������
	processSkill_();
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

void EneBrig::ChangeStateAlert(void)
{
	//�X�V�����̒��g������
	Enemy::ChangeStateAlert();

	//�ҋ@�A�j���[�V����
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}