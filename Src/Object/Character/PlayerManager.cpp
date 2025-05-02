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
		if (players_[i].ins == nullptr)assert("プレイヤーの生成が正常に動作していません");

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

JobManagerBase* PlayerManager::CreateKeybordPlayer(const SceneManager::ROLE _role) const
{
	JobManagerBase* ret = nullptr;

	switch (_role) {
	case SceneManager::ROLE::AXEMAN:
		ret = new PlAxe(SceneManager::CNTL::KEYBOARD);  //キャラクタ一人に必要な情報
		//ret = new PlAxe();
		break;
	case SceneManager::ROLE::ARCHER:
		ret = new PlArcher(SceneManager::CNTL::KEYBOARD);
		//ret = new PlArcher();
		break;
	case SceneManager::ROLE::KNIGHT:
		ret = new PlKnight(SceneManager::CNTL::KEYBOARD);
		//ret = new PlKnight();
		break;
	case SceneManager::ROLE::MAGE:
		ret = new PlMage(SceneManager::CNTL::KEYBOARD);
		//ret = new PlMage();
		break;
	default:
		assert("プレイヤーの役職情報が間違っています");
		break;
	}

	return ret;
}

JobManagerBase* PlayerManager::CreatePadPlayer(const SceneManager::ROLE _role)
{
	JobManagerBase* ret = nullptr;

	switch (_role) {
	case SceneManager::ROLE::AXEMAN:
		ret = new PlAxe(static_cast<InputManager::JOYPAD_NO>(padNum_));
		//ret = new PlAxe();
		break;
	case SceneManager::ROLE::ARCHER:
		ret = new PlArcher(static_cast<InputManager::JOYPAD_NO>(padNum_));
		//ret = new PlArcher();
		break;
	case SceneManager::ROLE::KNIGHT:
		ret = new PlKnight(static_cast<InputManager::JOYPAD_NO>(padNum_));
		//ret = new PlKnight();
		break;
	case SceneManager::ROLE::MAGE:
		ret = new PlMage(static_cast<InputManager::JOYPAD_NO>(padNum_));
		//ret = new PlMage();
		break;
	default:
		assert("プレイヤーの役職情報が間違っています");
		break;
	}

	padNum_++;

	return ret;
}

JobManagerBase* PlayerManager::CreateCpuPlayer(const SceneManager::ROLE _role)
{
	JobManagerBase* ret = nullptr;

	switch (_role) {
	case SceneManager::ROLE::AXEMAN:
		//ret = new PlAxe(SceneManager::CNTL::NONE);
		ret = new CpuAxe();
		//ret = new PlAxe();
		break;
	case SceneManager::ROLE::ARCHER:
		//ret = new PlArcher(SceneManager::CNTL::NONE);
		ret = new CpuArcher();
		//ret = new PlArcher();
		break;
	case SceneManager::ROLE::KNIGHT:
		//ret = new PlKnight(SceneManager::CNTL::NONE);
		ret = new CpuKnight();
		//ret = new PlKnight();
		break;
	case SceneManager::ROLE::MAGE:
		//ret = new PlMage(SceneManager::CNTL::NONE);
		ret = new CpuMage();
		//ret = new PlMage();
		break;
	default:
		assert("プレイヤーの役職情報が間違っています");
		break;
	}

	return ret;
}
