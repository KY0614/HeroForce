#include<cassert>
#include<random>

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
	TYPE type;
	
	do
	{
		type = static_cast<TYPE>(GetRand(static_cast<int>(TYPE::MAX) - 1));
	} while (type == TYPE::GOLEM);

	//生成相対座標
	VECTOR createLocalPos = GetNotOverlappingPos(type);

	//インスタンス生成
	switch (type)
	{
	case EnemyManager::TYPE::ARCHER:
		enm = new EneArcher(createLocalPos);
		break;
	case EnemyManager::TYPE::AXE:
		enm = new EneAxe(createLocalPos);
		break;
	case EnemyManager::TYPE::BRIG:
		enm = new EneBrig(createLocalPos);
		break;
	case EnemyManager::TYPE::GOLEM:
		//ゴーレムはボスキャラなのでここでは生成しない
		//enm = new EneGolem(AsoUtility::VECTOR_ZERO);
		return;
		break;
	case EnemyManager::TYPE::MAGE:
		enm = new EneMage(createLocalPos);
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

VECTOR EnemyManager::GetNotOverlappingPos(TYPE _type)
{
	//敵の大きさ
	float eneSize;

	switch (_type)
	{
	case EnemyManager::TYPE::ARCHER:
		eneSize = EneArcher::MY_COL_RADIUS;
		break;
	case EnemyManager::TYPE::AXE:
		eneSize = EneAxe::MY_COL_RADIUS;
		break;
	case EnemyManager::TYPE::BRIG:
		eneSize = EneBrig::MY_COL_RADIUS;
		break;
	case EnemyManager::TYPE::MAGE:
		eneSize = EneMage::MY_COL_RADIUS;
		break;
	default:
		eneSize = ENEMY_DISTANCE;
		break;
	}

	//生成判定
	bool isGenelateEnemy = false;

	//生成座標
	VECTOR createPos = createPos_[GetRand(createPos_.size() - 1)];

	//生成相対座標
	VECTOR ret = AsoUtility::VECTOR_ZERO;

	//生成できるまで繰り返す
	while (!isGenelateEnemy)
	{
		//円範囲の中の一点を取る
		GetRandomPointInCircle(createPos, GENELATE_RADIUS, ret);

		//生成場所が被っていないか
		if (!IsOverlap(ret, eneSize * 2))
		{
			//被っていなかった

			//生成完了
			isGenelateEnemy = true;
		}
	}

	return ret;
}

void EnemyManager::GetRandomPointInCircle(VECTOR _myPos, const int _r, VECTOR& _tPos)
{
	// ランダムエンジンを生成
	std::random_device rd;
	std::mt19937 gen(rd());

	//乱数の範囲(0 〜 2π)
	std::uniform_real_distribution<> dis(0, 2 * DX_PI_F);

	//乱数の範囲(0 〜 1)
	std::uniform_int_distribution<> create(0, createPos_.size() - 1);

	// ランダムな角度theta
	float theta = dis(gen);

	// ランダムな半径r' (0 〜 r) で、均等に分布するように sqrt を使う
	float radius = sqrt(static_cast<float>(rand()) / RAND_MAX) * _r;

	// 円内の点を計算
	_tPos.x = static_cast<int>(_myPos.x + radius * cos(theta));
	_tPos.z = static_cast<int>(_myPos.z + radius * sin(theta));

	//出現位置の指定しなおし
	_myPos = createPos_[create(gen)];
}

bool EnemyManager::IsOverlap(VECTOR& _tPos, float _minDist)
{
	for (const auto& enemy : activeEnemys_) {
		if (enemy == nullptr)
			continue;

		int dx = _tPos.x - enemy->GetPos().x;
		int dz = _tPos.z - enemy->GetPos().z;
		double distance = std::sqrt(dx * dx + dz * dz);
		if (distance < _minDist) {
			return true; // 重なっている場合
		}
	}
	return false; // 重なっていない場合
}

void EnemyManager::CollisionStage(const Transform& stageTrans)
{
	auto& col = Collision::GetInstance();

	for (int i = 0; i < activeNum_; i++)
	{
		if (col.IsHitUnitStageObject(stageTrans.modelId, activeEnemys_[i]->GetTransform().pos, activeEnemys_[i]->GetRadius()))
		{
			activeEnemys_[i]->SetPrePos();
		}
	}
}

void EnemyManager::DeathEnemy(int _num)
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
