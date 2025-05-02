#pragma once
#include"../../Manager/GameSystem/DataBank.h"
#include"../../Manager/Generic/SceneManager.h"

class PlayerBase;
class JobManagerBase;

class PlayerManager
{
public:

	//プレイヤーについての構造体
	struct PLAYER {
		JobManagerBase* ins;	//動かすキャラクターのインスタンス
		DataBank::PLAYER_INFO info;	//その他情報(詳細はDataBank参照)
	};

	//プレイヤーの数
	static constexpr int PLAYER_NUM = 4;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	/// <summary>
	/// 特定のキャラクタの情報を得る
	/// </summary>
	/// <param name="_num">何Pか</param>
	/// <returns>情報</returns>
	PlayerBase* GetPlayer(const int _num);

	//衝突判定
	void CollisionStage(const Transform& stageTrans);
	void ResetFaze(void);

private:
	//プレイヤーの管理
	PLAYER players_[PLAYER_NUM];

	//パッドの使用台数を管理
	int padNum_;
	
	JobManagerBase* CreateKeybordPlayer(const SceneManager::ROLE _role)const;
	JobManagerBase* CreatePadPlayer(const SceneManager::ROLE _role);
	JobManagerBase* CreateCpuPlayer(const SceneManager::ROLE _role);
};

