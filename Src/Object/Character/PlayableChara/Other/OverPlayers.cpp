#include "OverPlayers.h"

using S_ROLE = SceneManager::ROLE;

OverPlayers::OverPlayers(void)
{
	for (int i = 0; i < PLAYERS; i++) { role_[i] = S_ROLE::NONE; }
}

void OverPlayers::Init(void)
{
	//��E�ݒ�
	for (int i = 0; i < PLAYERS; i++)
	{
		role_[i] = DataBank::GetInstance().Output(i + 1).role_;
	}

	//3D���f��������
	Init3DModel();

	//�A�j���[�V�����̐ݒ�
	animNum_.emplace(ANIM::IDLE, IDLE_ANIM);
	animNum_.emplace(ANIM::DEATH, DEATH_ANIM);
	ResetAnim(ANIM::DEATH, ANIM_SPEED);
}

void OverPlayers::Update(void)
{
	Animation();

	for (auto& tran_ : transArray_)
	{
		//���f���̏�����
		tran_.Update();
	}
}

void OverPlayers::Draw(void)
{
	for (int i = 0; i < PLAYERS; i++)
	{
		MV1DrawModel(transArray_[i].modelId);

		//���f���̏�����
		transArray_[i].Update();
	}
}

void OverPlayers::Init3DModel(void)
{
	//��E���f���̐ݒ�
	for (int i = 0; i < PLAYERS; i++) {

		switch (role_[i])
		{
		case S_ROLE::KNIGHT:
			transArray_[i].SetModel(
				ResourceManager::GetInstance()
				.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
			break;

		case S_ROLE::AXEMAN:
			transArray_[i].SetModel(
				ResourceManager::GetInstance()
				.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));
			break;

		case S_ROLE::MAGE:
			transArray_[i].SetModel(
				ResourceManager::GetInstance()
				.LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGE));
			break;

		case S_ROLE::ARCHER:
			transArray_[i].SetModel(
				ResourceManager::GetInstance()
				.LoadModelDuplicate(ResourceManager::SRC::PLAYER_ARCHER));
			break;

		default:
			break;
		}

		//�I�u�W�F�N�g���Ƃɍ��W�ʒu�̐ݒ�
		float deg = -1;
		if (i == 0) { transArray_[i].pos = PLAYER_POS_1; deg = PLAYER_ROT_1; }
		else if (i == 1) { transArray_[i].pos = PLAYER_POS_2; deg = PLAYER_ROT_2; }
		else if (i == 2) { transArray_[i].pos = PLAYER_POS_3; deg = PLAYER_ROT_3; }
		else if (i == 3) { transArray_[i].pos = PLAYER_POS_4; deg = PLAYER_ROT_4; }

		//���̑��ݒ�
		transArray_[i].scl = SCALE;
		transArray_[i].quaRot = Quaternion();
		transArray_[i].quaRotLocal = Quaternion::Euler(
			0.0f,
			AsoUtility::Deg2RadF(deg),
			0.0f);
		transArray_[i].Update();
	}
}

void OverPlayers::Animation()
{
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	// �A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (speedAnim_ * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		//�A�j���[�V�����I���������i�p����ōs�������߂Ă����j
		FinishAnim();
	}
	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	for (auto& t : transArray_) {
		MV1SetAttachAnimTime(t.modelId, atcAnim_, stepAnim_);
	}
}

void OverPlayers::ResetAnim(const ANIM _anim, const float _speed)
{
	if (anim_ == _anim)return;

	//�A�j���[�V�����X�s�[�h�̕ύX
	speedAnim_ = _speed;

	//�A�j���[�V�����X�e�[�g�ݒ�
	anim_ = _anim;
	stepAnim_ = 0.0f;

	for (auto& t : transArray_)
	{
		//�f�^�b�`
		//����atcAnim�̏�����
		MV1DetachAnim(t.modelId, atcAnim_);

		//�A�^�b�`
		//����atcAnim�̑��
		atcAnim_ = MV1AttachAnim(t.modelId, animNum_[anim_]);

		animTotalTime_ = MV1GetAttachAnimTotalTime(t.modelId, atcAnim_);


		// �Đ�����A�j���[�V�������Ԃ̐ݒ�
		MV1SetAttachAnimTime(t.modelId, atcAnim_, stepAnim_);
	}
}

void OverPlayers::FinishAnim()
{
	switch (anim_)
	{
	case UnitBase::ANIM::IDLE:
		//���[�v�Đ�
		stepAnim_ = 0.0f;
		break;

	case UnitBase::ANIM::DEATH:
		ResetAnim(ANIM::IDLE, ANIM_SPEED);
		break;
	}
}
