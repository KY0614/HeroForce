#include <assert.h>
#include "../../../../Manager/Generic/ResourceManager.h"
#include "../PlayerBase.h"
#include "SelectPlayer.h"

SelectPlayer::SelectPlayer(void)
{
	role_ = 0;
	animChangeTime_ = 0.0f;
}

void SelectPlayer::Destroy(void)
{
}

void SelectPlayer::Init(void)
{
	Init3DModel();

	animNum_.emplace(ANIM::IDLE, IDLE_ANIM);

	ResetAnimArray(ANIM::IDLE, ANIM_SPEED);
}

void SelectPlayer::Update(void)
{
	AnimArray();

	int randNum = 0;
	//if (animChangeTime_ > 50.0f)
	//{
	//	randNum = GetRand(2);
	//	
	//}
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	animChangeTime_ += ANIM_SPEED * deltaTime;
	
	if (animChangeTime_ > GetAnimArrayTime())
	{
		if (anim_ == ANIM::IDLE)
		{
			ResetAnimArray(ANIM::SKILL_1, ANIM_SPEED);
			animChangeTime_ = 0.0f;
		}
		else
		{
			ResetAnimArray(ANIM::IDLE, ANIM_SPEED);
			animChangeTime_ = 0.0f;
		}
	}

	for (auto& tran_ : transArray_) 
	{
		Quaternion yRot = Quaternion::AngleAxis(AsoUtility::Deg2RadF(ROT_SPEED), AsoUtility::AXIS_Y);
		tran_.quaRot = Quaternion::Mult(tran_.quaRot, yRot);

		//モデルの初期化
		tran_.Update();
	}
}

void SelectPlayer::Draw(void)
{
	MV1DrawModel(transArray_[role_].modelId);
	//DrawFormatString(0, 0, 0xFF0000, "time : %2.f", animChangeTime_);

	for (auto& tran_ : transArray_) {

		//モデルの初期化
		tran_.Update();
	}
}

void SelectPlayer::SetPos(VECTOR pos)
{
	for (auto& tran_ : transArray_)
	{
		tran_.pos = pos;
	}
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

	for (auto& tran_ : transArray_) {
		//モデルの初期化
		tran_.Update();
	}

}