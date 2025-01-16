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
	for (auto& tran_ : trans_) 
	{
		Quaternion yRot = Quaternion::AngleAxis(AsoUtility::Deg2RadF(ROT_SPEED), AsoUtility::AXIS_Y);
		tran_.quaRot = Quaternion::Mult(tran_.quaRot, yRot);

		//モデルの初期化
		tran_.Update();
	}
}

void SelectPlayer::Draw(void)
{
	MV1DrawModel(trans_[role_].modelId);

	for (auto& tran_ : trans_) {
		//モデルの初期化
		tran_.Update();
	}
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
	//騎士
	trans_[0].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));

	//斧使い
	trans_[1].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));

	//魔法使い
	trans_[2].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGE));

	//弓使い
	trans_[3].SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_ARCHER));

	float scale = 0.4f;
	for (auto& tran_ : trans_) 
	{
		tran_.scl = { scale, scale, scale };
		tran_.pos = { 60.0f, 60.0f, -300.0f };
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