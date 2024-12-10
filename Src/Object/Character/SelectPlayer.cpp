#include <assert.h>
#include "../Manager/ResourceManager.h"
#include "SelectPlayer.h"

SelectPlayer::SelectPlayer()
{
}

void SelectPlayer::Destroy(void)
{
}

void SelectPlayer::Init(void)
{
	Init3DModel();
	UpdateTrans();
}

void SelectPlayer::Update(void)
{
	UpdateTrans();
}

void SelectPlayer::Draw(void)
{
	MV1DrawModel(trans_[role].modelId);
}

void SelectPlayer::SetRole(int role)
{
	this->role = role;
}

void SelectPlayer::Init3DModel(void)
{	
	//ãRém
	trans_[0].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	float scale = CHARACTER_SCALE;
	trans_[0].scl = { scale, scale, scale };
	trans_[0].pos = { 110.0f, 110.0f, -50.0f };
	trans_[0].quaRot = Quaternion();
	trans_[0].quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	//ïÄégÇ¢
	trans_[1].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));
	trans_[1].scl = { scale, scale, scale };
	trans_[1].pos = { 110.0f, 110.0f, -50.0f };
	trans_[1].quaRot = Quaternion();
	trans_[1].quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	//ñÇñ@égÇ¢
	trans_[2].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGE));
	trans_[2].scl = { scale, scale, scale };
	trans_[2].pos = { 110.0f, 110.0f, -50.0f };
	trans_[2].quaRot = Quaternion();
	trans_[2].quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	//ã|égÇ¢
	trans_[3].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_ARCHER));
	trans_[3].scl = { 30.0f, 30.0f, 30.0f };
	trans_[3].pos = { 110.0f, 110.0f, -50.0f };
	trans_[3].quaRot = Quaternion();
	trans_[3].quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(0.0f),
		0.0f
	);

	UpdateTrans();
}

void SelectPlayer::UpdateTrans(void)
{
	for (auto& tran_ : trans_) {
		//ÉÇÉfÉãÇÃèâä˙âª
		tran_.Update();
	}
}
