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
	//3Dモデル初期化
	Init3DModel();

	//キャラクター用
	for (int i = 0; i < SceneManager::PLAYER_NUM;i++)
	{
		animNumArray_[i].emplace(ANIM::IDLE, IDLE_ANIM);
		ResetAnimArray(ANIM::IDLE, ANIM_SPEED, i);
	}

	//アニメーション番号を設定
	animNumArray_[0].emplace(ANIM::SKILL_1, KNIGHT_ANIM);
	animNumArray_[1].emplace(ANIM::SKILL_1, AXE_ANIM);
	animNumArray_[2].emplace(ANIM::SKILL_1, MAGE_ANIM);
	animNumArray_[3].emplace(ANIM::SKILL_1, ARCHER_ANIM);

	//チキン用
	animNum_.emplace(ANIM::UNIQUE_1, CHICKEN_SWINGHAND_ANIM);
	ResetAnim(ANIM::UNIQUE_1, CHICKEN_SPEED);

	//アニメーション時間を初期化
	for (auto& animTime : animChangeTime_)
	{
		animTime = 0.0f;
	}

	//初期の役職を騎士にしとく
	role_ = 0;
}

void SelectCharacter::Update(void)
{
	//アニメーション
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++) { AnimArray(i); }
	Anim();

	//アニメーションと同じ時間を加算していく
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	for (auto& animTime : animChangeTime_)
	{
		animTime += ANIM_SPEED * deltaTime;
	}

	//一定時間ごとにアニメーションを変更
	UpdateAnimCycle();

	for (auto& tran_ : transArray_) 
	{
		Quaternion yRot = Quaternion::AngleAxis(AsoUtility::Deg2RadF(ROT_SPEED), AsoUtility::AXIS_Y);
		tran_.quaRot = Quaternion::Mult(tran_.quaRot, yRot);

		//モデルの初期化
		tran_.Update();
	}
	trans_.Update();
}

void SelectCharacter::Draw(void)
{
	//キャラ
	MV1DrawModel(transArray_[role_].modelId);
	//チキン
	MV1DrawModel(trans_.modelId);
}

void SelectCharacter::SetPos(VECTOR pos)
{
	//配列の座標を順々に設定
	for (auto& tran_ : transArray_)
	{
		tran_.pos = pos;
	}
}

void SelectCharacter::Init3DModel(void)
{	
	//騎士
	transArray_[0].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));

	//斧使い
	transArray_[1].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));

	//魔法使い
	transArray_[2].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGE));

	//弓使い
	transArray_[3].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_ARCHER));

	//チキン
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::CHICKEN));

	//座標、拡大率、回転を設定
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

	//モデルの初期化
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
		//現在のアニメーション再生時間を超えていたら違うアニメーションにする
		if (animChangeTime_[i] > GetAnimArrayTime(i))
		{
			//アイドルアニメーションと攻撃アニメーションを交互に再生させる
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