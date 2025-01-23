#include <assert.h>
#include "../../../../Manager/Generic/ResourceManager.h"
#include "../PlayerBase.h"
#include "../../Chiken/ChickenBase.h"
#include "SelectPlayer.h"

SelectPlayer::SelectPlayer(void)
{
	role_ = 0;
	animKnightTime_ = 0.0f;
	animAxeTime_ = 0.0f;
	animMageTime_ = 0.0f;
	animArcherTime_ = 0.0f;
}

void SelectPlayer::Destroy(void)
{
}

void SelectPlayer::Init(void)
{
	Init3DModel();

	//キャラクター用
	animNumArray_[0].emplace(ANIM::IDLE, IDLE_ANIM);
	animNumArray_[1].emplace(ANIM::IDLE, IDLE_ANIM);
	animNumArray_[2].emplace(ANIM::IDLE, IDLE_ANIM);
	animNumArray_[3].emplace(ANIM::IDLE, IDLE_ANIM);

	animNumArray_[0].emplace(ANIM::SKILL_1, KNIGHT_ANIM);
	animNumArray_[1].emplace(ANIM::SKILL_1, AXE_ANIM);
	animNumArray_[2].emplace(ANIM::SKILL_1, MAGE_ANIM);
	animNumArray_[3].emplace(ANIM::SKILL_1, ARCHER_ANIM);

	ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED, 0);
	ResetAnimArray(ANIM::IDLE, ANIM_SPEED, 1);
	ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED, 2);
	ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED, 3);

	//チキン用
	animNum_.emplace(ANIM::UNIQUE_1, SWING_ANIM);
	ResetAnim(ANIM::UNIQUE_1, ANIM_SPEED);
}

void SelectPlayer::Update(void)
{
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++) { AnimArray(i); }
	Anim();


	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	animKnightTime_ += ANIM_SPEED * deltaTime;
	animAxeTime_ += ANIM_SPEED * deltaTime;
	
	//CheckAnim();

	for (auto& tran_ : transArray_) 
	{
		Quaternion yRot = Quaternion::AngleAxis(AsoUtility::Deg2RadF(ROT_SPEED), AsoUtility::AXIS_Y);
		tran_.quaRot = Quaternion::Mult(tran_.quaRot, yRot);

		//モデルの初期化
		tran_.Update();
	}
	trans_.Update();
}

void SelectPlayer::Draw(void)
{
	MV1DrawModel(transArray_[role_].modelId);
	MV1DrawModel(trans_.modelId);
}

void SelectPlayer::SetPos(VECTOR pos)
{
	for (auto& tran_ : transArray_)
	{
		tran_.pos = pos;
	}
}

void SelectPlayer::CheckAnim(void)
{
	if (animKnightTime_ > GetAnimArrayTime(0))
	{
		if (anim_ == ANIM::IDLE)
		{
			ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED, 0);
			animKnightTime_ = 0.0f;
		}
		else
		{
			ResetAnimArray(ANIM::IDLE, ANIM_SPEED, 0);
			animKnightTime_ = 0.0f;
		}
	}

	if (animAxeTime_ > GetAnimArrayTime(1))
	{
		if (anim_ == ANIM::IDLE)
		{
			ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED, 1);
			animAxeTime_ = 0.0f;
		}
		else
		{
			ResetAnimArray(ANIM::IDLE, ANIM_SPEED, 1);
			animAxeTime_ = 0.0f;
		}
	}
}

void SelectPlayer::SetAtkAnim(int i)
{
	if(i == 0)ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED, 0);
	if(i == 1)ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED, 1);
	if(i == 2)ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED, 2);
	if(i == 3)ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED, 3);
	return;
}

void SelectPlayer::SetIdleAnim(int i)
{
	ResetAnimArray(ANIM::IDLE, ANIM_SPEED, i);
	return;
}

void SelectPlayer::Init3DModel(void)
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

	for (auto& tran_ : transArray_) {
		//モデルの初期化
		tran_.Update();
	}
	trans_.Update();
}