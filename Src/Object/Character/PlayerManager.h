#pragma once
#include"../../Manager/GameSystem/DataBank.h"
#include"../../Manager/Generic/SceneManager.h"

class PlayerBase;

class PlayerManager
{
public:

	//プレイヤーについての構造体
	struct PLAYER {
		PlayerBase* ins;	//動かすキャラクターのインスタンス
		DataBank::PLAYER_INFO info;	//その他情報(詳細はDataBank参照)
	};

	//プレイヤーの数
	static constexpr int PLAYER_NUM = 4;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	//プレイヤーの管理
	PLAYER players_[PLAYER_NUM];

	//パッドの使用台数を管理
	int padNum_;
	
	PlayerBase* CreateKeybordPlayer(const SceneManager::ROLE _role)const;
	PlayerBase* CreatePadPlayer(const SceneManager::ROLE _role);
	PlayerBase* CreateCpuPlayer(const SceneManager::ROLE _role);
};

