#include<cassert>
#include"../../Manager/Generic/InputManager.h"

#include "PlayerBase.h"
#include"PlayerCpu.h"
#include "PlayableChara/USER/PlAxeMan.h"
#include "PlayableChara/USER/PlKnight.h"
#include "PlayableChara/USER/PlArcher.h"
#include "PlayableChara/USER/PlMage.h"

#include "PlayerManager.h"

void PlayerManager::Init(void)
{
	DataBank& data = DataBank::GetInstance();

	padNum_ = 1;

	for (int i = 0; i < PLAYER_NUM; i++) {
		//情報を取得
		players_[i].info = data.Output(i);
		//生成
		if (players_[i].info.mode_ == SceneManager::PLAY_MODE::USER){
			//ユーザー
			if (players_[i].info.cntrol_ == SceneManager::CNTL::KEYBOARD)players_[i].ins = CreateKeybordPlayer(players_[i].info.role_);
			if (players_[i].info.cntrol_ == SceneManager::CNTL::PAD)players_[i].ins = CreatePadPlayer(players_[i].info.role_);
		}
		else {
			players_[i].ins = CreateCpuPlayer(players_[i].info.role_);
		}
		

		//デバッグ用
		if (players_[i].ins == nullptr)assert("プレイヤーの生成が正常に動作していません");

		//初期化
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
		players_[i].ins->Destroy();
	}
}

PlayerBase* PlayerManager::CreateKeybordPlayer(const SceneManager::ROLE _role) const
{
	PlayerBase* ret = nullptr;

	switch (_role) {
	case SceneManager::ROLE::AXEMAN:
		ret = new PlAxe(SceneManager::CNTL::KEYBOARD);
		break;
	case SceneManager::ROLE::ARCHER:
		ret = new PlArcher(SceneManager::CNTL::KEYBOARD);
		break;
	case SceneManager::ROLE::KNIGHT:
		ret = new PlKnight(SceneManager::CNTL::KEYBOARD);
		break;
	case SceneManager::ROLE::MAGE:
		ret = new PlMage(SceneManager::CNTL::KEYBOARD);
		break;
	default:
		assert("プレイヤーの役職情報が間違っています");
		break;
	}

	return ret;
}

PlayerBase* PlayerManager::CreatePadPlayer(const SceneManager::ROLE _role)
{
	PlayerBase* ret = nullptr;

	switch (_role) {
	case SceneManager::ROLE::AXEMAN:
		ret = new PlAxe(static_cast<InputManager::JOYPAD_NO>(padNum_));
		break;
	case SceneManager::ROLE::ARCHER:
		ret = new PlArcher(static_cast<InputManager::JOYPAD_NO>(padNum_));
		break;
	case SceneManager::ROLE::KNIGHT:
		ret = new PlKnight(static_cast<InputManager::JOYPAD_NO>(padNum_));
		break;
	case SceneManager::ROLE::MAGE:
		ret = new PlMage(static_cast<InputManager::JOYPAD_NO>(padNum_));
		break;
	default:
		assert("プレイヤーの役職情報が間違っています");
		break;
	}

	padNum_++;

	return ret;
}

PlayerBase* PlayerManager::CreateCpuPlayer(const SceneManager::ROLE _role)
{
	PlayerBase* ret = nullptr;

	switch (_role) {
	case SceneManager::ROLE::AXEMAN:
		ret = new PlAxe(SceneManager::CNTL::NONE);
		break;
	case SceneManager::ROLE::ARCHER:
		ret = new PlArcher(SceneManager::CNTL::NONE);
		break;
	case SceneManager::ROLE::KNIGHT:
		ret = new PlKnight(SceneManager::CNTL::NONE);
		break;
	case SceneManager::ROLE::MAGE:
		ret = new PlMage(SceneManager::CNTL::NONE);
		break;
	default:
		assert("プレイヤーの役職情報が間違っています");
		break;
	}

	return ret;
}
