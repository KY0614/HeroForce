#include"Enemy.h"
#include "EneBrig.h"

EneBrig::EneBrig(const VECTOR& _spawnPos) : Enemy(_spawnPos)
{
	trans_.pos = _spawnPos;
}

void EneBrig::SetParam(void)
{
	//�U���x��
	alertSkills_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneBrig::AlertSkill_One, this));

	//�U���̑J��
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneBrig::Skill_One, this));

	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_BRIGANT));

	//���X�Őݒ肷��
	trans_.scl = { CHARACTER_SCALE,CHARACTER_SCALE,CHARACTER_SCALE };
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	localCenterPos_ = LOCAL_CENTER_POS;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;

	//�O������X�e�[�^�X���擾
	ParamLoad(CharacterParamData::UNIT_TYPE::E_KNIGHT);
}

void EneBrig::InitAnim(void)
{
	//���ʃA�j���[�V����������
	Enemy::InitAnim();

#pragma region �ŗL�A�j���[�V�����̏�����

	//�X�L��1
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
	//�X�C���O����
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_PRE_SWING);

#pragma endregion

#pragma region �A�j���[�V�������x�̐ݒ�

	//�X�L��1
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM);
	//�X�C���O����
	changeSpeedAnim_.emplace(ANIM::UNIQUE_1, SPEED_ANIM);

#pragma endregion
	//�A�j���[�V�������Z�b�g
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneBrig::InitSkill(void)
{
#pragma region �X�L���̊i�[

	//�X�L��1
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);

#pragma endregion

#pragma region �X�L���̗\������A�j���[�V�����i�[

	//�X�L��1
	skillPreAnims_.emplace_back(ANIM::UNIQUE_1);

#pragma endregion

#pragma region �X�L���̓���A�j���[�V����

	//�X�L��1
	skillAnims_.emplace_back(ANIM::SKILL_1);

#pragma endregion

	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void EneBrig::AlertSkill_One(void)
{
	//�G�̑O��
	VECTOR pos = trans_.GetForward();

	//�U���͈�
	pos = VScale(pos, SKILL_ONE_COL_RADIUS);

	//���W���킹
	pos = VAdd(trans_.pos, pos);

	//�͈͍쐬
	CreateAlert(pos, SKILL_ONE_COL_RADIUS * 2, SKILL_ONE_COL_RADIUS * 2);
}

void EneBrig::Skill_One(void)
{
	//�I������
	if (lastAtk_->IsFinishMotion())
	{
		//�U���I��
		isEndAllAtk_ = true;

		//�����I��
		return;
	}

	for (auto& nowSkill : nowSkill_)
	{
		//���W�̐ݒ�
		nowSkill.pos_ = MV1GetFramePosition(trans_.modelId, FRAME_KNIFE);
	}
}

void EneBrig::FinishAnim(void)
{
	//���ʃA�j���[�V�����̏I������
	Enemy::FinishAnim();

	switch (anim_)
	{
	case UnitBase::ANIM::UNIQUE_1:
		break;
	}
}