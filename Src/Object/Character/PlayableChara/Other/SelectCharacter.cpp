#include "../../../../Manager/Generic/ResourceManager.h"
#include "SelectCharacter.h"

SelectCharacter::SelectCharacter(void)
{
	role_ = -1;
	for (int i = 0; i < 4; i++) 
	{
		animChangeTime_[i] = -1.0f;
	}
}

void SelectCharacter::Destroy(void)
{
}

void SelectCharacter::Init(void)
{
	//3D���f��������
	Init3DModel();

	//�L�����N�^�[�p
	for (int i = 0; i < SceneManager::PLAYER_NUM;i++)
	{
		animNumArray_[i].emplace(ANIM::IDLE, IDLE_ANIM);
		ResetAnimArray(ANIM::IDLE, ANIM_SPEED, i);
	}

	//�A�j���[�V�����ԍ���ݒ�
	animNumArray_[0].emplace(ANIM::SKILL_1, KNIGHT_ANIM);
	animNumArray_[1].emplace(ANIM::SKILL_1, AXE_ANIM);
	animNumArray_[2].emplace(ANIM::SKILL_1, MAGE_ANIM);
	animNumArray_[3].emplace(ANIM::SKILL_1, ARCHER_ANIM);

	//�`�L���p
	animNum_.emplace(ANIM::UNIQUE_1, CHICKEN_SWINGHAND_ANIM);
	ResetAnim(ANIM::UNIQUE_1, CHICKEN_SPEED);

	//�A�j���[�V�������Ԃ�������
	for (auto& animTime : animChangeTime_)
	{
		animTime = 0.0f;
	}

	//�����̖�E���R�m�ɂ��Ƃ�
	role_ = 0;
}

void SelectCharacter::Update(void)
{
	//�A�j���[�V����
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++) { AnimArray(i); }
	Anim();

	//�A�j���[�V�����Ɠ������Ԃ����Z���Ă���
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	for (auto& animTime : animChangeTime_)
	{
		animTime += ANIM_SPEED * deltaTime;
	}

	//��莞�Ԃ��ƂɃA�j���[�V������ύX
	UpdateAnimCycle();

	for (auto& tran_ : transArray_) 
	{
		Quaternion yRot = Quaternion::AngleAxis(AsoUtility::Deg2RadF(ROT_SPEED), AsoUtility::AXIS_Y);
		tran_.quaRot = Quaternion::Mult(tran_.quaRot, yRot);

		//���f���̏�����
		tran_.Update();
	}
	trans_.Update();
}

void SelectCharacter::Draw(void)
{
	//�L����
	MV1DrawModel(transArray_[role_].modelId);
	//�`�L��
	MV1DrawModel(trans_.modelId);
}

void SelectCharacter::SetPos(VECTOR pos)
{
	//�z��̍��W�����X�ɐݒ�
	for (auto& tran_ : transArray_)
	{
		tran_.pos = pos;
	}
}

void SelectCharacter::Init3DModel(void)
{	
	//�R�m
	transArray_[0].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));

	//���g��
	transArray_[1].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));

	//���@�g��
	transArray_[2].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGE));

	//�|�g��
	transArray_[3].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_ARCHER));

	//�`�L��
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::CHICKEN));

	//���W�A�g�嗦�A��]��ݒ�
	float scale = 0.4f;
	for (auto& tran_ : transArray_) 
	{
		tran_.scl = { scale, scale, scale };
		tran_.pos = { 70.0f, 60.0f, -300.0f };
		tran_.quaRot = Quaternion();
		tran_.quaRotLocal = Quaternion::Euler(
			0.0f, AsoUtility::Deg2RadF(0.0f),
			0.0f
		);
	}

	scale = 0.9f;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { -40.0f, 50.0f, -300.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(0.0f),
		0.0f
	);

	//���f���̏�����
	for (auto& tran_ : transArray_) 
	{	
		tran_.Update();
	}
	trans_.Update();
}

void SelectCharacter::UpdateAnimCycle(void)
{
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		//���݂̃A�j���[�V�����Đ����Ԃ𒴂��Ă�����Ⴄ�A�j���[�V�����ɂ���
		if (animChangeTime_[i] > GetAnimArrayTime(i))
		{
			//�A�C�h���A�j���[�V�����ƍU���A�j���[�V���������݂ɍĐ�������
			if (animStateArray_[i] != ANIM::IDLE)
			{
				ResetAnimArray(ANIM::IDLE, ANIM_SPEED, i);
				animChangeTime_[i] = 0.0f;
			}
			else
			{
				ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED, i);
				animChangeTime_[i] = 0.0f;
			}
		}
	}
}