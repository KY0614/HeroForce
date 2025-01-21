#include<cassert>

#include"EnemySort/Enemy.h"
#include"EnemySort/EneArcher.h"
#include"EnemySort/EneAxe.h"
#include"EnemySort/EneBrig.h"
#include"EnemySort/EneGolem.h"
#include"EnemySort/EneMage.h"

#include "EnemyManager.h"

void EnemyManager::Init(void)
{
	activeNum_ = 0;
	for (int i = 0; i < 4; i++)
	{
		CreateEnemy();
	}
}

void EnemyManager::Update(VECTOR _target)
{
	for (int i = 0; i < activeNum_; i++)
	{
		activeEnemys_[i]->SetTargetPos(_target);
		activeEnemys_[i]->Update();
	}
}

void EnemyManager::Draw(void)
{
	for (int i = 0; i < activeNum_; i++)activeEnemys_[i]->Draw();
}

void EnemyManager::Release(void)
{
	
}



void EnemyManager::CreateEnemy(void)
{
	//敵が最大数いたら生成処理を行わない
	if (activeNum_ >= ENEMY_MAX)return;

	Enemy* enm = nullptr;

	//乱数で種類決める
	TYPE type = static_cast<TYPE>(GetRand(static_cast<int>(TYPE::MAX)-1));

	//インスタンス生成
	switch (type)
	{
	case EnemyManager::TYPE::ARCHER:
		enm = new EneArcher();
		break;
	case EnemyManager::TYPE::AXE:
		enm = new EneAxe();
		break;
	case EnemyManager::TYPE::BRIG:
		enm = new EneBrig();
		break;
	case EnemyManager::TYPE::GOLEM:
		//ゴーレムはボスキャラなのでここでは生成しない
		return;
		break;
	case EnemyManager::TYPE::MAGE:
		enm = new EneMage();
		break;
	default:
		return;
		break;
	}

	//念のためのエラー回避用
	if (enm == nullptr)assert("敵の生成で問題がありました。");

	//敵の初期化
	enm->Init();

	//敵の更新等を掛けるやつをセット
	activeEnemys_[activeNum_] = enm;

	//カウンタ増加
	activeNum_++;
}

void EnemyManager::DethEnemy(int _num)
{
	//倒された敵の消去
	activeEnemys_[_num]->Destroy();
	delete activeEnemys_[_num];

	//総数の減少
	//この時点でactiveNum_は配列の末尾の番号を示すようになる。
	activeNum_--;

	//配列の空きを埋めるためのソート
	//配列の末尾の物を空きに移動させる
	//倒された敵が末尾の物なら処理はしない
	if (_num == activeNum_)return;

	//挿入処理
	//std::moveでもいいかも？分かり易いように下記のようにする
	activeEnemys_[_num] = activeEnemys_[activeNum_];

	//末尾の消去
	activeEnemys_[activeNum_]->Destroy();
	delete activeEnemys_[activeNum_];
}
