#include"../../../Manager/Resource.h"
#include"../../../Manager/ResourceManager.h"
#include"../Enemy.h"
#include "EneGolem.h"

void EneGolem::SetParam(void)
{
	//�U���̑J��
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneGolem::Skill_One, this));
	changeSkill_.emplace(ATK_ACT::SKILL_TWO, std::bind(&EneGolem::Skill_Two, this));

	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_GOLEM));

	//���X�Őݒ肷��
	trans_.scl = { SCALE,SCALE,SCALE };
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

void EneGolem::InitAnim(void)
{
	//���S�[�����͑S���ŗL�A�j���[�V����

	//�ŗL�A�j���[�V����������
	animNum_.emplace(ANIM::IDLE, EneGolem::ANIM_IDLE);
	animNum_.emplace(ANIM::WALK, EneGolem::ANIM_WALK);
	animNum_.emplace(ANIM::SKILL_1, ANIM_PUNCH);
	animNum_.emplace(ANIM::SKILL_2, ANIM_MOWDOWN);
	animNum_.emplace(ANIM::SKILL_3, ANIM_SHOUT);
	animNum_.emplace(ANIM::SKILL_4, ANIM_JUMP_ATK);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_JUMP);
	animNum_.emplace(ANIM::UNIQUE_2, ANIM_LANDING);

	//�A�j���[�V�������x�ݒ�
	changeSpeedAnim_.emplace(ANIM::IDLE, SPEED_ANIM_IDLE);
	changeSpeedAnim_.emplace(ANIM::WALK, SPEED_ANIM_WALK);
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM_PUNCH);
	changeSpeedAnim_.emplace(ANIM::SKILL_2, SPEED_ANIM_MOWDOWN);
	changeSpeedAnim_.emplace(ANIM::SKILL_3, SPEED_ANIM_SHOUT);
	changeSpeedAnim_.emplace(ANIM::SKILL_4, SPEED_ANIM_JUMP);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_1, SPEED_ANIM_JUMP);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_2, SPEED_ANIM_JUMP);

	//�A�j���[�V�������Z�b�g
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneGolem::InitSkill(void)
{
	//�����ɃX�L���̐����i�[������
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);
	skills_.emplace(ATK_ACT::SKILL_TWO, SKILL_TWO);

	//�����ɃX�L���̐����A�j���[�V�������i�[������
	skillAnims_.emplace_back(ANIM::SKILL_1);
	skillAnims_.emplace_back(ANIM::SKILL_2);

	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void EneGolem::Skill_One(void)
{
	for (auto& nowSkill : nowSkill_)
	{
		//���W�̐ݒ�
		nowSkill.pos_ = MV1GetFramePosition(trans_.modelId, FRAME_R_HAND);
	}
}

void EneGolem::Skill_Two(void)
{
	for (auto& nowSkill : nowSkill_)
	{
		//���W�̐ݒ�
		nowSkill.pos_ = MV1GetFramePosition(trans_.modelId, FRAME_R_HAND);
	}
}

void EneGolem::ChangeStateAlert(void)
{
	//�X�V�����̒��g������
	Enemy::ChangeStateAlert();

	//�ҋ@�A�j���[�V����
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}
