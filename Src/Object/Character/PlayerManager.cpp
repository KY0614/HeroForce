#include<cassert>
#include"../../Manager/Generic/InputManager.h"

#include "PlayableChara/PlayerBase.h"
#include"PlayableChara/PlayerCpu.h"
#include "PlayableChara/USER/PlAxeMan.h"
#include "PlayableChara/USER/PlKnight.h"
#include "PlayableChara/USER/PlArcher.h"
#include "PlayableChara/USER/PlMage.h"
#include "PlayableChara/CPU/CpuAxe.h"
#include "PlayableChara/CPU/CpuArcher.h"
#include "PlayableChara/CPU/CpuKnight.h"
#include "PlayableChara/CPU/CpuMage.h"

#include "PlayerManager.h"

void PlayerManager::Init(void)
{
	DataBank& data = DataBank::GetInstance();

	padNum_ = 1;

	PLAYER info[4];

	//プレイヤー番号ごとにプレイヤー情報を入れる
	info[0].info= DataBank::GetInstance().Output(1);
	info[1].info= DataBank::GetInstance().Output(2);
	info[2].info= DataBank::GetInstance().Output(3);
	info[3].info= DataBank::GetInstance().Output(4);

	for (int i = 0; i < PLAYER_NUM; i++) {
		//情報を取得
		players_[i].info = DataBank::GetInstance().Output(i+1);

		if (players_[i].info.role_ == SceneManager::ROLE::NONE)players_[i].info.role_ = SceneManager::ROLE::KNIGHT;
		//生成
		if (players_[i].info.mode_ == SceneManager::PLAY_MODE::USER){
			//ユーザー
			if (players_[i].info.cntrol_ == SceneManager::CNTL::KEYBOARD)players_[i].ins = CreateKeybordPlayer(players_[i].info.role_);
			if (players_[i].info.cntrol_ == SceneManager::CNTL::PAD)players_[i].ins = CreatePadPlayer(players_[i].info.role_);

			players_[i].ins->SetPlayerNum(i);
		}
		else {
			players_[i].ins = CreateCpuPlayer(players_[i].info.role_);
			players_[i].ins->SetPlayerNum(i);
		}
		//デバッグ用
		//if (players_[i].ins == nullptr)assert("プレイヤーの生成が正常に動作していません");

		players_[i].ins->Init();
	}
}

void PlayerManager::Update(void)
{
	for (int i = 0; i < PLAYER_NUM; i++) {
		players_[i].ins->Update();
	}
}

void PlayerManager::Draw(void)
{
	for (int i = 0; i < PLAYER_NUM; i++) {
		players_[i].ins->Draw();
	}
}

void PlayerManager::Release(void)
{
	for (int i = 0; i < PLAYER_NUM; i++) {
		players_[i].ins->Release();
	}
}

PlayerBase* PlayerManager::GetPlayer(const int _num)
{
	return players_[_num].ins->GetPlayer();
}

void PlayerManager::CollisionStage(const Transform& stageTrans)
{
	auto& col = Collision::GetInstance();

	for (int i = 0; i < PLAYER_NUM; i++)
	{
		if (col.IsHitUnitStageObject(stageTrans.modelId, players_[i].ins->GetPos(), players_[i].ins->GetRadius()))
		{
			players_[i].ins->SetPrePos();
		}
	}
}

void PlayerManager::ResetFaze(void)
{
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		players_[i].ins->GetPlayer()->Reset();
	}
}

std::shared_ptr<JobManagerBase> PlayerManager::CreateKeybordPlayer(const SceneManager::ROLE _role) const
{
	std::shared_ptr<JobManagerBase> ret;

	switch (_role) {
	case SceneManager::ROLE::AXEMAN:
		ret = std::make_shared<PlAxe>(SceneManager::CNTL::KEYBOARD);  //キャラクタ一人に必要な情報
		break;
	case SceneManager::ROLE::ARCHER:
		ret = std::make_shared<PlArcher>(SceneManager::CNTL::KEYBOARD);
		break;
	case SceneManager::ROLE::KNIGHT:
		ret = std::make_shared<PlKnight>(SceneManager::CNTL::KEYBOARD);
		break;
	case SceneManager::ROLE::MAGE:
		ret = std::make_shared<PlMage>(SceneManager::CNTL::KEYBOARD);
		break;
	default:
		assert("プレイヤーの役職情報が間違っています");
		break;
	}

	return ret;
}

std::shared_ptr<JobManagerBase> PlayerManager::CreatePadPlayer(const SceneManager::ROLE _role)
{
	std::shared_ptr<JobManagerBase> ret;

	switch (_role) {
	case SceneManager::ROLE::AXEMAN:
		ret = std::make_shared<PlAxe>(static_cast<InputManager::JOYPAD_NO>(padNum_));
		break;
	case SceneManager::ROLE::ARCHER:
		ret = std::make_shared<PlArcher>(static_cast<InputManager::JOYPAD_NO>(padNum_));
		break;
	case SceneManager::ROLE::KNIGHT:
		ret = std::make_shared<PlKnight>(static_cast<InputManager::JOYPAD_NO>(padNum_));
		break;
	case SceneManager::ROLE::MAGE:
		ret = std::make_shared<PlMage>(static_cast<InputManager::JOYPAD_NO>(padNum_));
		break;
	default:
		assert("プレイヤーの役職情報が間違っています");
		break;
	}

	padNum_++;

	return ret;
}

std::shared_ptr<JobManagerBase> PlayerManager::CreateCpuPlayer(const SceneManager::ROLE _role)
{
	std::shared_ptr<JobManagerBase> ret;

	switch (_role) {
	case SceneManager::ROLE::AXEMAN:
		ret = std::make_shared<CpuAxe>();
		break;
	case SceneManager::ROLE::ARCHER:
		ret = std::make_shared<CpuArcher>();
		break;
	case SceneManager::ROLE::KNIGHT:
		ret = std::make_shared<CpuKnight>();
		break;
	case SceneManager::ROLE::MAGE:
		ret = std::make_shared<CpuMage>();
		break;
	default:
		assert("プレイヤーの役職情報が間違っています");
		break;
	}
	return ret;
}
