#include<random>
#include"Enemy.h"
#include "EneGolem.h"

EneGolem::EneGolem(const VECTOR& _spawnPos) : Enemy(_spawnPos)
{
	trans_.pos = _spawnPos;
}

void EneGolem::Destroy(void)
{
	auto& eff = EffectManager::GetInstance();

	//����
	Enemy::Destroy();

	eff.Stop(EffectManager::EFFECT::BOSS_PUNCH);
	eff.Stop(EffectManager::EFFECT::BOSS_SHOUT);
	eff.Stop(EffectManager::EFFECT::BOSS_SHOUT_ATK);
}

void EneGolem::SetParam(void)
{
	//�U���x��
	alertSkills_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneGolem::AlertSkill_One, this));
	alertSkills_.emplace(ATK_ACT::SKILL_TWO, std::bind(&EneGolem::AlertSkill_Two, this));
	alertSkills_.emplace(ATK_ACT::SKILL_THREE, std::bind(&EneGolem::AlertSkill_Three, this));

	//�U���̑J��
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneGolem::Skill_One, this));
	changeSkill_.emplace(ATK_ACT::SKILL_TWO, std::bind(&EneGolem::Skill_Two, this));
	changeSkill_.emplace(ATK_ACT::SKILL_THREE, std::bind(&EneGolem::Skill_Three, this));

	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_GOLEM));

	//���X�Őݒ肷��
	trans_.scl = { SCALE,SCALE,SCALE };
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	localCenterPos_ = LOCAL_CENTER_POS;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
	skillThreeCnt_ = 0;
	skillThreeDelayCnt_ = 0.0f;
	isPreSkillThreeAtk_ = false;

	//�O������X�e�[�^�X���擾
	ParamLoad(CharacterParamData::UNIT_TYPE::BOSS);
}

void EneGolem::InitAnim(void)
{
	//���S�[�����͑S���ŗL�A�j���[�V����

#pragma region �A�j���[�V����������

	//����
	animNum_.emplace(ANIM::IDLE, EneGolem::ANIM_IDLE);
	//����
	animNum_.emplace(ANIM::WALK, EneGolem::ANIM_WALK);
	//����
	animNum_.emplace(ANIM::RUN, EneGolem::ANIM_WALK);
	//���S
	animNum_.emplace(ANIM::DEATH, EneGolem::ANIM_KNOCK);
	//�p���`
	animNum_.emplace(ANIM::SKILL_1, ANIM_PUNCH);
	//�ガ����
	animNum_.emplace(ANIM::SKILL_2, ANIM_MOWDOWN);
	//����
	animNum_.emplace(ANIM::SKILL_3, ANIM_SHOUT);
	//�W�����v�U��
	animNum_.emplace(ANIM::SKILL_4, ANIM_JUMP_ATK);
	//�W�����v
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_JUMP);
	//���n
	animNum_.emplace(ANIM::UNIQUE_2, ANIM_LANDING);
	//�p���`����
	animNum_.emplace(ANIM::UNIQUE_3, ANIM_PRE_PUNCH);
	//�ガ��������
	animNum_.emplace(ANIM::UNIQUE_4, ANIM_PRE_MOWDOWN);
	//���я���
	animNum_.emplace(ANIM::UNIQUE_5, ANIM_PRE_SHOUT);

#pragma endregion

#pragma region �A�j���[�V�������x�ݒ�

	//����
	changeSpeedAnim_.emplace(ANIM::IDLE, SPEED_ANIM_IDLE);
	//����
	changeSpeedAnim_.emplace(ANIM::WALK, SPEED_ANIM_WALK);
	//����
	changeSpeedAnim_.emplace(ANIM::RUN, SPEED_ANIM_WALK);
	//���S
	changeSpeedAnim_.emplace(ANIM::DEATH, SPEED_ANIM_KNOCK);
	//�p���`
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM_PUNCH);
	//�ガ����
	changeSpeedAnim_.emplace(ANIM::SKILL_2, SPEED_ANIM_MOWDOWN);
	//����
	changeSpeedAnim_.emplace(ANIM::SKILL_3, SPEED_ANIM_SHOUT);
	//�W�����v�U��
	changeSpeedAnim_.emplace(ANIM::SKILL_4, SPEED_ANIM_JUMP);
	//�W�����v
	changeSpeedAnim_.emplace(ANIM::UNIQUE_1, SPEED_ANIM_JUMP);
	//���n
	changeSpeedAnim_.emplace(ANIM::UNIQUE_2, SPEED_ANIM_JUMP);
	//�p���`����
	changeSpeedAnim_.emplace(ANIM::UNIQUE_3, SPEED_ANIM_PRE_PUNCH);
	//�ガ��������
	changeSpeedAnim_.emplace(ANIM::UNIQUE_4, SPEED_ANIM_PRE_MOWDOWN);
	//���я���
	changeSpeedAnim_.emplace(ANIM::UNIQUE_5, SPEED_ANIM_PRE_SHOUT);

#pragma endregion

	//�A�j���[�V�������Z�b�g
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneGolem::InitEffect(void)
{
	//�G�t�F�N�g
	auto& eff = EffectManager::GetInstance();
	//���\�[�X
	auto& res = ResourceManager::GetInstance();

	//�p���`�G�t�F�N�g
	eff.Add(EffectManager::EFFECT::BOSS_PUNCH,
		res.Load(ResourceManager::SRC::BOSS_PUNCH_EFE).handleId_);

	//���уG�t�F�N�g
	eff.Add(EffectManager::EFFECT::BOSS_SHOUT,
		res.Load(ResourceManager::SRC::BOSS_SHOUT_EFE).handleId_);

	//����(�U��)�G�t�F�N�g
	eff.Add(EffectManager::EFFECT::BOSS_SHOUT_ATK,
		res.Load(ResourceManager::SRC::BOSS_SHOUT_ATK_EFE).handleId_);
}

void EneGolem::InitSkill(void)
{

#pragma region �X�L���̊i�[

	//�X�L��1
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);
	//�X�L��2
	skills_.emplace(ATK_ACT::SKILL_TWO, SKILL_TWO);
	//�X�L��3
	skills_.emplace(ATK_ACT::SKILL_THREE, SKILL_THREE);

#pragma endregion

#pragma region �X�L���̗\������A�j���[�V�����i�[

	//�X�L��1
	skillPreAnims_.emplace_back(ANIM::UNIQUE_3);
	//�X�L��2
	skillPreAnims_.emplace_back(ANIM::UNIQUE_4);
	//�X�L��3
	skillPreAnims_.emplace_back(ANIM::UNIQUE_5);

#pragma endregion

#pragma region �X�L���̓���A�j���[�V����

	//�X�L��1
	skillAnims_.emplace_back(ANIM::SKILL_1);
	//�X�L��2
	skillAnims_.emplace_back(ANIM::SKILL_2);
	//�X�L��3
	skillAnims_.emplace_back(ANIM::SKILL_3);

#pragma endregion

	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void EneGolem::Alert(void)
{
	//�x��
	alertNowSkill_();

	//�N�[���_�E���J�E���^
	CntUp(alertCnt_);

	//�X�L���R�ȊO�͒ʏ�ʂ�
	if (!IsAlertTime() && atkAct_ != ATK_ACT::SKILL_THREE)isEndAlert_ = true;
}

void EneGolem::AlertSkill_One(void)
{
	//�G�t�F�N�g�}�l�[�W���[
	auto& eff = EffectManager::GetInstance();

	//���W
	VECTOR pos = trans_.pos;

	//���΍��W���킹
	pos = VAdd(pos, Quaternion::PosAxis(trans_.quaRot, SKILL_ONE_ALERT));

	//�͈͍쐬
	CreateAlert(pos, SKILL_ONE_COL_RADIUS * 2, SKILL_ONE_COL_RADIUS * 2);
}

void EneGolem::AlertSkill_Two(void)
{
	//���W
	VECTOR pos = trans_.pos;

	//���΍��W���킹
	pos = VAdd(pos, Quaternion::PosAxis(trans_.quaRot, SKILL_TWO_ALERT));

	//�͈͍쐬
	CreateAlert(pos, SKILL_TWO_COL_RADIUS * 2 + SKILL_TWO_ALERT_RANGE_X, SKILL_TWO_COL_RADIUS * 2);
}

void EneGolem::AlertSkill_Three(void)
{
	//�U���̐����\������
	if (skillThreeCnt_ >= SKILL_THREE_MAX_CNT
		&& skillThreeDelayCnt_ >= SKILL_THREE_DELAY)
	{
		//�����I��
		isEndAlert_ = true;

		//�����I��
		return;
	}

	if (skillThreeDelayCnt_ < SKILL_THREE_DELAY)
	{
		//�J�E���g
		CntUp(skillThreeDelayCnt_);

		return;
	}
	
	//�U�������ł������̔���
	bool isGenelateAttack = false;

	//�����ł���܂ŌJ��Ԃ�
	while (!isGenelateAttack)
	{
		//�~�͈͂̒��̈�_�����
		GetRandomPointInCircle(trans_.pos, SKILL_THREE_FALL_RADIUS, skillThreePreAtk_[skillThreeCnt_].pos_);

		if (!IsOverlap(skillThreePreAtk_[skillThreeCnt_], SKILL_THREE_COL_RADIUS * 2))
		{
			//��������
			isGenelateAttack = true;
		}
	}

	//�������ꂽ����p
	CntUp(skillThreePreAtk_[skillThreeCnt_].cnt_);

	//�͈͍쐬
	CreateAlert(skillThreePreAtk_[skillThreeCnt_].pos_, SKILL_THREE_COL_RADIUS * 2, SKILL_THREE_COL_RADIUS * 2);

	//������
	skillThreeDelayCnt_ = 0.0f;

	//�J�E���g
	skillThreeCnt_++;
}

void EneGolem::Skill_One(void)
{
	//�G�t�F�N�g�}�l�[�W���[
	auto& eff = EffectManager::GetInstance();

	if (stepAnim_ == SKILL_ONE_EFF_TIME)
		//�G�t�F�N�g�Đ�
		eff.Play(EffectManager::EFFECT::BOSS_PUNCH,
			MV1GetFramePosition(trans_.modelId, FRAME_R_HAND),
			trans_.quaRot,
			SKILL_ONE_EFF_SIZE,
			SoundManager::SOUND::NONE);


	//�U���̍Đ���
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
		nowSkill.pos_ = MV1GetFramePosition(trans_.modelId, FRAME_R_HAND);
	}
}

void EneGolem::Skill_Two(void)
{
	//�U���̍Đ���
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
		nowSkill.pos_ = MV1GetFramePosition(trans_.modelId, FRAME_R_HAND);
	}
}

void EneGolem::Skill_Three(void)
{
	//�G�t�F�N�g�}�l�[�W���[
	auto& eff = EffectManager::GetInstance();

	//�U���I������
	if (lastAtk_ == nullptr ? false : lastAtk_->IsFinishMotion()
		&& skillThreeCnt_ >= SKILL_THREE_MAX_CNT)
	{
		//�U���I��
		isEndAllAtk_ = true;

		//�����I��
		return;
	}

	//�U���̊Ԋu�Ɛ������
	if (skillThreeDelayCnt_ > SKILL_THREE_DELAY
		&& skillThreeCnt_ < SKILL_THREE_MAX_CNT)
	{
		if(skillThreeCnt_ == 0)
			//�G�t�F�N�g�Đ�
			eff.Play(EffectManager::EFFECT::BOSS_SHOUT,
				MV1GetFramePosition(trans_.modelId, FRAME_HEAD),
				trans_.quaRot,
				SKILL_ONE_EFF_SIZE,
				SoundManager::SOUND::NONE);


		//�Ԋu�J�E���^�̏�����
		skillThreeDelayCnt_ = 0.0f;

		//�Ō�ɐ������ꂽ�U�����i�[
		lastAtk_ = &createSkill_();

		//�ێ����Ă������W�������ƂɃZ�b�g
		lastAtk_->pos_ = skillThreePreAtk_[skillThreeCnt_].pos_;

		//�G�t�F�N�g�̍Đ�
		eff.Play(EffectManager::EFFECT::BOSS_SHOUT_ATK,
			lastAtk_->pos_,
			trans_.quaRot,
			SKILL_THREE_ATK_EFF_SIZE,
			SoundManager::SOUND::NONE);

		//�J�E���^����
		skillThreeCnt_++;
	}

	//�Ԋu�J�E���^
	CntUp(skillThreeDelayCnt_);
}

void EneGolem::ResetAlertJudge(void)
{
	//����
	Enemy::ResetAlertJudge();

	//�X�L���R�̐����J�E���^������
	skillThreeCnt_ = 0;

	//�X�L���R�̐����Ԋu�J�E���^������
	skillThreeDelayCnt_ = 0.0f;

	//�X�L���R�̔����\���������菉����
	isPreSkillThreeAtk_ = false;
}

void EneGolem::ResetAtkJudge(void)
{
	//����
	Enemy::ResetAtkJudge();

	//�X�L���R�̐����J�E���^������
	skillThreeCnt_ = 0;

	//�X�L���R�̐����Ԋu�J�E���^������
	skillThreeDelayCnt_ = 0.0f;
}

void EneGolem::ChangeStateAtk(void)
{
	//�G�t�F�N�g�}�l�[�W���[
	auto& eff = EffectManager::GetInstance();

	//�X�V�����̒��g������
	stateUpdate_ = std::bind(&EneGolem::UpdateAtk, this);

	//�ŏ��ɍU���𐶐����邩
	if (atkAct_ != ATK_ACT::SKILL_THREE)
	{
		//�U������
		lastAtk_ = &createSkill_();
	}
}

void EneGolem::GetRandomPointInCircle(const VECTOR _myPos, const int _r, VECTOR& _tPos)
{
	// �����_���G���W���𐶐�
	std::random_device rd;
	std::mt19937 gen(rd());

	//�����_���p�x�͈̔�(0 �` 2��)
	std::uniform_real_distribution<> dis(0, 2 * DX_PI_F);

	//�����_�����a�͈̔�(0 �` r)
	std::uniform_real_distribution<> randRadius(0, _r);

	// �����_���Ȋp�xtheta
	float theta = dis(gen);

	// �����_���Ȕ��ar' (0 �` r) �ŁA�ϓ��ɕ��z����悤�� sqrt ���g��
	float radius = randRadius(gen);

	// �~���̓_���v�Z
	_tPos.x = static_cast<int>(_myPos.x + radius * cos(theta));
	_tPos.z = static_cast<int>(_myPos.z + radius * sin(theta));
}

bool EneGolem::IsOverlap(ATK& _thisAtk, float _minDist)
{
	for (const auto& atk : skillThreePreAtk_) {
		//�������g�Ƃ͔��肵�Ȃ�
		if (atk.cnt_ == _thisAtk.cnt_)
			continue;

		int dx = _thisAtk.pos_.x - atk.pos_.x;
		int dz = _thisAtk.pos_.z - atk.pos_.z;
		double distance = std::sqrt(dx * dx + dz * dz);
		if (distance < _minDist) {
			return true; // �d�Ȃ��Ă���ꍇ
		}
	}
	return false; // �d�Ȃ��Ă��Ȃ��ꍇ
}