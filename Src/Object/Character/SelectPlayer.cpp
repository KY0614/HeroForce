#include <assert.h>
#include "../Manager/ResourceManager.h"
#include "SelectPlayer.h"

SelectPlayer::SelectPlayer(void)
{
	role_ = 0;
}

void SelectPlayer::Destroy(void)
{
}

void SelectPlayer::Init(void)
{
	Init3DModel();
	for (auto& tran_ : trans_) {
		//モデルの初期化
		tran_.Update();
	}
}

void SelectPlayer::Update(void)
{

	for (auto& tran_ : trans_) {
		//モデルの初期化
		tran_.Update();
	}
}

void SelectPlayer::Draw(void)
{
	MV1DrawModel(trans_[role_].modelId);
}

void SelectPlayer::SetRole(int role)
{
	this->role_ = role;
}

void SelectPlayer::Init3DModel(void)
{	
	//騎士
	trans_[0].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	//trans_[0].scl = { scale, scale, scale };
	//trans_[0].pos = { 110.0f, 110.0f, -300.0f };
	//trans_[0].quaRot = Quaternion();
	//trans_[0].quaRotLocal = Quaternion::Euler(
	//	0.0f, AsoUtility::Deg2RadF(0.0f),
	//	0.0f
	//);
	//斧使い
	trans_[1].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));
	//trans_[1].scl = { scale, scale, scale };
	//trans_[1].pos = { 110.0f, 110.0f, -300.0f };
	//trans_[1].quaRot = Quaternion();
	//trans_[1].quaRotLocal = Quaternion::Euler(
	//	0.0f, AsoUtility::Deg2RadF(0.0f),
	//	0.0f
	//);
	//魔法使い
	trans_[2].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGE));
	//trans_[2].scl = { scale, scale, scale };
	//trans_[2].pos = { 110.0f, 110.0f, -300.0f };
	//trans_[2].quaRot = Quaternion();
	//trans_[2].quaRotLocal = Quaternion::Euler(
	//	0.0f, AsoUtility::Deg2RadF(0.0f),
	//	0.0f
	//);
	//弓使い
	trans_[3].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_ARCHER));
	//trans_[3].scl = { scale, scale, scale };
	//trans_[3].pos = { 110.0f, 110.0f, -300.0f };
	//trans_[3].quaRot = Quaternion();
	//trans_[3].quaRotLocal = Quaternion::Euler(
	//	0.0f, AsoUtility::Deg2RadF(0.0f),
	//	0.0f
	//);
	float scale = 0.4f;
	for (auto& tran_ : trans_) 
	{
		tran_.scl = { scale, scale, scale };
		tran_.pos = { 40.0f, 60.0f, -300.0f };
		tran_.quaRot = Quaternion();
		tran_.quaRotLocal = Quaternion::Euler(
			0.0f, AsoUtility::Deg2RadF(0.0f),
			0.0f
		);
	}

	for (auto& tran_ : trans_) {
		//モデルの初期化
		tran_.Update();
	}
}