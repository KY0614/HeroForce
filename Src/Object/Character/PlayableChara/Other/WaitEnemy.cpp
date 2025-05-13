#include"../Manager/Generic/SceneManager.h"
#include"../../../../Utility/AsoUtility.h"
#include "WaitEnemy.h"

WaitEnemy::WaitEnemy(void)
{
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		animChangeTime_[i] = 0.0f;
		isSpawn_[i] = false;
	}
}

void WaitEnemy::Destroy(void)
{
}

void WaitEnemy::Init(void)
{
	//3D���f���̏�����
	Init3DModel();

	//�L�����N�^�[�p
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		//�A�j���[�V�����ԍ���ݒ�
		animNumArray_[i].emplace(ANIM::IDLE, IDLE_ANIM);
		animNumArray_[i].emplace(ANIM::UNIQUE_1, SPAWN_ANIM);
		ResetAnimArray(ANIM::UNIQUE_1, ANIM_SPEED, i);
	}

	//�擪�̓G�����X�|�[�����ɂ���
	isSpawn_[0] = true;
}

void WaitEnemy::Update(void)
{
	//�A�j���[�V�����Ɠ������Ԃ����Z���Ă���
	float deltaTime = 1.0f / Application::DEFAULT_FPS;

	//�A�j���[�V����
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		//�X�|�[�����̓X�|�[���p�̃A�j���[�V�������Đ�
		if (isSpawn_[i])
		{
			AnimArray(i);
			animChangeTime_[i] += ANIM_SPEED * deltaTime;
		}

		//�X�|�[���A�j���[�V�����̍Đ����I�������Idle�A�j���[�V�����ɐ؂�ւ���
		if (isSpawn_[i] &&
			animChangeTime_[i] > animArrayTotalTime_[i])
		{
			if (animStateArray_[i] != ANIM::IDLE)
			{
				ResetAnimArray(ANIM::IDLE, ANIM_SPEED, i);
				animChangeTime_[i] = 0.0f;
				isSpawn_[i + 1] = true;		//���o��������G���X�|�[�����ɂ���

			}
		}
	}

	//���f�����̍X�V
	for (auto& tran_ : transArray_)
	{
		tran_.Update();
	}
}

void WaitEnemy::Draw(void)
{
	//���Ԃɕ`��
	for (auto& tran : transArray_)
	{
		MV1DrawModel(tran.modelId);
	}
}

void WaitEnemy::Init3DModel(void)
{
	//�|�g��
	transArray_[0].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::ENEMY_ARCHER));

	//���g��
	transArray_[1].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::ENEMY_AXEMAN));

	//���@�g��
	transArray_[2].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::ENEMY_MAGE));

	//�R��
	transArray_[3].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::ENEMY_BRIGANT));

	//���W�A�g�嗦�A��]��ݒ�
	float scale = 0.6f;
	for (int i = 0; i < SceneManager::PLAYER_NUM;i++)
	{
		transArray_[i].scl = { scale, scale, scale };
		transArray_[i].pos = { -210.0f + (150.0f * i), -50.0f, 70.0f};
		transArray_[i].quaRot = Quaternion();
		transArray_[i].quaRotLocal = Quaternion::Euler(
			0.0f, AsoUtility::Deg2RadF(0.0f),
			0.0f
		);
	}

	//���f���̏�����
	for (auto& tran_ : transArray_)
	{
		tran_.Update();
	}
}
