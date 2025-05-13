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
	//3Dモデルの初期化
	Init3DModel();

	//キャラクター用
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		//アニメーション番号を設定
		animNumArray_[i].emplace(ANIM::IDLE, IDLE_ANIM);
		animNumArray_[i].emplace(ANIM::UNIQUE_1, SPAWN_ANIM);
		ResetAnimArray(ANIM::UNIQUE_1, ANIM_SPEED, i);
	}

	//先頭の敵だけスポーン中にする
	isSpawn_[0] = true;
}

void WaitEnemy::Update(void)
{
	//アニメーションと同じ時間を加算していく
	float deltaTime = 1.0f / Application::DEFAULT_FPS;

	//アニメーション
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		//スポーン中はスポーン用のアニメーションを再生
		if (isSpawn_[i])
		{
			AnimArray(i);
			animChangeTime_[i] += ANIM_SPEED * deltaTime;
		}

		//スポーンアニメーションの再生が終わったらIdleアニメーションに切り替える
		if (isSpawn_[i] &&
			animChangeTime_[i] > animArrayTotalTime_[i])
		{
			if (animStateArray_[i] != ANIM::IDLE)
			{
				ResetAnimArray(ANIM::IDLE, ANIM_SPEED, i);
				animChangeTime_[i] = 0.0f;
				isSpawn_[i + 1] = true;		//次出現させる敵をスポーン中にする

			}
		}
	}

	//モデル情報の更新
	for (auto& tran_ : transArray_)
	{
		tran_.Update();
	}
}

void WaitEnemy::Draw(void)
{
	//順番に描画
	for (auto& tran : transArray_)
	{
		MV1DrawModel(tran.modelId);
	}
}

void WaitEnemy::Init3DModel(void)
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
