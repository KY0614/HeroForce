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

}

void SelectPlayer::Update(void)
{

	for (auto& tran_ : trans_) {
		//ƒ‚ƒfƒ‹‚Ì‰Šú‰»
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

void SelectPlayer::SetPos(VECTOR pos)
{
	for (int i = 0;i < SceneManager::PLAYER_NUM;i++)
	{
		trans_[i].pos = pos;
	}
}

void SelectPlayer::Init3DModel(void)
{	
	//‹RŽm
	trans_[0].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));

	//•€Žg‚¢
	trans_[1].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));

	//–‚–@Žg‚¢
	trans_[2].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGE));

	//‹|Žg‚¢
	trans_[3].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_ARCHER));

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
		//ƒ‚ƒfƒ‹‚Ì‰Šú‰»
		tran_.Update();
	}
}