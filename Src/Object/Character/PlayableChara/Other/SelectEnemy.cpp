#include "SelectEnemy.h"

SelectEnemy::SelectEnemy(void)
{
	animChangeTime_[0] = 0.0f;
	animChangeTime_[1] = 0.0f;
	animChangeTime_[2] = 0.0f;
	animChangeTime_[3] = 0.0f;

	isSpawn_[0] = false;
	isSpawn_[1] = false;
	isSpawn_[2] = false;
	isSpawn_[3] = false;
}

void SelectEnemy::Destroy(void)
{
}

void SelectEnemy::Init(void)
{
	Init3DModel();

	//キャラクター用
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		//アニメーション番号を設定
		animNumArray_[i].emplace(ANIM::IDLE, IDLE_ANIM);
		animNumArray_[i].emplace(ANIM::UNIQUE_1, SPAWN_ANIM);
		ResetAnimArray(ANIM::UNIQUE_1, ANIM_SPEED, i);
	}

	isSpawn_[0] = true;
}

void SelectEnemy::Update(void)
{
	//アニメーションと同じ時間を加算していく
	float deltaTime = 1.0f / Application::DEFAULT_FPS;

	//if (isSpawn_[3] &&
	//	animChangeTime_[3] > animArrayTotalTime_[3])
	//{
	//	//リセット
	//	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	//	{
	//		ResetAnimArray(ANIM::UNIQUE_1, SPAWN_ANIM, i);
	//		animChangeTime_[i] = 0.0f;
	//		isSpawn_[i] = false;
	//		FinishAnimArray(i);
	//	}
	//	isSpawn_[0] = true;
	//}

	//アニメーション
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		if (isSpawn_[i])
		{
			AnimArray(i);
			animChangeTime_[i] += ANIM_SPEED * deltaTime;
		}

		if (isSpawn_[i] &&
			animChangeTime_[i] > animArrayTotalTime_[i])
		{
			if (animStateArray_[i] != ANIM::IDLE)
			{
				ResetAnimArray(ANIM::IDLE, ANIM_SPEED, i);
				animChangeTime_[i] = 0.0f;
				isSpawn_[i + 1] = true;

			}
		}
	}

	//モデルの初期化
	for (auto& tran_ : transArray_)
	{
		tran_.Update();
	}
}

void SelectEnemy::Draw(void)
{
	for (auto& tran : transArray_)
	{
		MV1DrawModel(tran.modelId);
	}
}

void SelectEnemy::Init3DModel(void)
{
	//弓使い
	transArray_[0].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::ENEMY_ARCHER));

	//斧使い
	transArray_[1].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::ENEMY_AXEMAN));

	//魔法使い
	transArray_[2].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::ENEMY_MAGE));

	//山賊
	transArray_[3].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::ENEMY_BRIGANT));

	//座標、拡大率、回転を設定
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

	//モデルの初期化
	for (auto& tran_ : transArray_)
	{
		tran_.Update();
	}
}
