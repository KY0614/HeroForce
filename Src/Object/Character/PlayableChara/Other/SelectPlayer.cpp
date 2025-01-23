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
	animNum_.emplace(ANIM::IDLE, IDLE_ANIM);
	animNum_.emplace(ANIM::UNIQUE_2, IDLE_ANIM);
	animNum_.emplace(ANIM::SKILL_1, KNIGHT_ANIM);
	animNum_.emplace(ANIM::SKILL_2, AXE_ANIM);
	animNum_.emplace(ANIM::SKILL_3, MAGE_ANIM);
	animNum_.emplace(ANIM::SKILL_4, ARCHER_ANIM);

	ResetAnimArray(ANIM::IDLE, ANIM_SPEED, 0);
	ResetAnimArray(ANIM::IDLE, ANIM_SPEED, 1);
	ResetAnimArray(ANIM::IDLE, ANIM_SPEED, 2);
	ResetAnimArray(ANIM::IDLE, ANIM_SPEED, 3);

	//チキン用
	animNum_.emplace(ANIM::UNIQUE_1, SWING_ANIM);

}

	ResetAnim(ANIM::UNIQUE_1, ANIM_SPEED);
}

void SelectPlayer::Update(void)
{
	AnimArray();
	Anim();

	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	animKnightTime_ += ANIM_SPEED * deltaTime;
	animAxeTime_ += ANIM_SPEED * deltaTime;
	
	CheckAnim();

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
			SetAtkAnim(0);
			animKnightTime_ = 0.0f;
		}
		else
		{
			SetIdleAnim(0);
			animKnightTime_ = 0.0f;
		}
	}

	if (animAxeTime_ > GetAnimArrayTime(1))
	{
		if (anim_ == ANIM::IDLE)
		{
			SetAtkAnim(1);
			animAxeTime_ = 0.0f;
		}
		else
		{
			SetIdleAnim(1);
			animAxeTime_ = 0.0f;
		}
	}
}

void SelectPlayer::SetAtkAnim(int i)
{
	if(i == 0)ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED, 0);
	if(i == 1)ResetAnimArray(ANIM::SKILL_2, ANIM_SPEED, 1);
	if(i == 2)ResetAnimArray(ANIM::SKILL_3, ANIM_SPEED, 2);
	if(i == 3)ResetAnimArray(ANIM::SKILL_4, ANIM_SPEED, 3);
	return;
}

void SelectPlayer::SetIdleAnim(int i)
{
	ResetAnimArray(ANIM::IDLE, ANIM_SPEED, i);
	return;
}

void SelectPlayer::SetAtkAnim(int i)
{
	switch (i)
	{
	case 0 :
		ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED, 0);
		break;

	case 1:
		ResetAnimArray(ANIM::SKILL_2, ANIM_SPEED, 1);
		break;

	case 2:
		ResetAnimArray(ANIM::SKILL_3, ANIM_SPEED, 2);
		break;

	case 3:
		ResetAnimArray(ANIM::SKILL_4, ANIM_SPEED, 3);
		break;

	default:
		break;
	}
}

void SelectPlayer::SetIdleAnim(int i)
{
	ResetAnimArray(ANIM::IDLE, ANIM_SPEED, i);
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
		tran_.pos = { 60.0f, 60.0f, -300.0f };
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