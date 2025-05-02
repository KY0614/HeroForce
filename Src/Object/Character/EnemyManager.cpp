#include<cassert>
#include<random>
#include"../Manager/Generic/SceneManager.h"

#include"EnemySort/Enemy.h"
#include"EnemySort/EneArcher.h"
#include"EnemySort/EneAxe.h"
#include"EnemySort/EneBrig.h"
#include"EnemySort/EneGolem.h"
#include"EnemySort/EneMage.h"

#include "EnemyManager.h"

EnemyManager::EnemyManager(std::vector<VECTOR> _pos) : createPos_(_pos)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		activeEnemys_[i] = nullptr;
		activeEnemysType_[i] = TYPE::MAX;
	}

	for (int a = 0; a < static_cast<int>(TYPE::MAX); a++)
	{
		activeTypeNum_[a] = 0;
	}

	allExp_ = 0.0f;

	activeNum_ = 0;

	createIntCnt_ = 0.0f;
}

void EnemyManager::Init(void)
{
	for (int a = 0; a < static_cast<int>(TYPE::MAX); a++)
	{
		activeTypeNum_[a] = 0;
	}

	dunkCnt_ = 0;

	allExp_ = 0.0f;

	activeNum_ = 0;

	createIntCnt_ = 0.0f;

	ProcessChangePhase(1);
}

void EnemyManager::Update(void)
{
	//カウンタ
	createIntCnt_ = createIntCnt_ + SceneManager::GetInstance().GetDeltaTime();

	//敵の生成処理
	if (createIntCnt_ >= CREATE_INTERVAL)
	{
		//間隔カウンタ初期化
		createIntCnt_ = 0.0f;

		//敵の生成
		CreateEnemy();
	}

	//生存している敵の処理
	for (int i = 0; i < activeNum_; i++)
	{
		//死亡判定
		if (!activeEnemys_[i]->IsAlive() 
			&& activeEnemys_[i]->IsFinishAnim(Enemy::ANIM::DEATH))
		{

			//敵削除
			DeathEnemy(i);
			break;
		}

		//activeEnemys_[i]->SetTargetPos(_target);
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

void EnemyManager::CollisionStage(const Transform& stageTrans)
{
	auto& col = Collision::GetInstance();

	for (int i = 0; i < activeNum_; i++)
	{
		if (col.IsHitUnitStageObject(stageTrans.modelId, activeEnemys_[i]->GetTransform().pos, activeEnemys_[i]->GetRadius()))
		{
			activeEnemys_[i]->SetPrePos();

			//移動補間
			activeEnemys_[i]->KeepCollStageDistance();
		}
	}
}



void EnemyManager::CreateEnemy(void)
{
	//敵が最大数いたら生成処理を行わない
	if (activeNum_ >= ENEMY_MAX)return;

	Enemy* enm = nullptr;

	//乱数で種類決める
	TYPE type;
	
	//ゴーレム以外で
	do
	{
		type = static_cast<TYPE>(GetRand(static_cast<int>(TYPE::MAX) - 1));
	} while (type == TYPE::GOLEM || activeTypeNum_[static_cast<int>(type)] >= ONETYPE_MAX);

	//生成相対座標
	VECTOR createLocalPos = createPos_[GetRand(createPos_.size() - 1)];
	createLocalPos.x += activeNum_ * Enemy::COL_RADIUS;
	createLocalPos.y = 0.0f;

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

	//敵の種類を保存
	activeEnemysType_[activeNum_] = type;

	//カウンタ増加
	activeTypeNum_[static_cast<int>(activeEnemysType_[activeNum_])]++;
	activeNum_++;
}

void EnemyManager::CreateBoss(void)
{
	//敵が最大数いたら生成処理を行わない
	if (activeNum_ >= ENEMY_MAX)return;

	//敵の生成
	Enemy* enm = nullptr;

	//生成相対座標
	VECTOR createLocalPos = createPos_[1];

	//ゴーレムの生成
	enm = new EneGolem(createLocalPos);

	//念のためのエラー回避用
	if (enm == nullptr)assert("敵の生成で問題がありました。");

	//敵の初期化
	enm->Init();

	//敵の更新等を掛けるやつをセット
	activeEnemys_[activeNum_] = enm;

	//敵の種類を保存
	activeEnemysType_[activeNum_] = TYPE::GOLEM;

	//カウンタ増加
	activeTypeNum_[static_cast<int>(activeEnemysType_[activeNum_])]++;
	activeNum_++;
}

VECTOR EnemyManager::GetNotOverlappingPos(void)
{
	//敵の大きさ
	float eneSize;
	eneSize = Enemy::COL_RADIUS;

	//生成判定
	bool isGenelateEnemy = false;

	//生成挑戦回数
	int challengeCnt = 0;

	//生成座標
	VECTOR createPos = AsoUtility::VECTOR_ZERO;//createPos_[GetRand(createPos_.size() - 1)];

	//生成相対座標
	VECTOR ret = AsoUtility::VECTOR_ZERO;

	//生成できるまで繰り返す
	while (!isGenelateEnemy)
	{
 		if (challengeCnt == ENEMY_CREATE_CHALLENGE_LIMIT)
			assert("敵の生成挑戦回数上限に達したためフリーズと判断しました");

		//カウンタ
		challengeCnt++;

		//生成座標
		VECTOR createPos = createPos_[GetRand(createPos_.size() - 1)];

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

	//乱数の範囲(0 ～ 2π)
	std::uniform_real_distribution<> dis(0, 2 * DX_PI_F);

	//乱数の範囲(0 ～ 1)
	std::uniform_int_distribution<> create(0, createPos_.size() - 1);

	// ランダムな角度theta
	float theta = dis(gen);

	// ランダムな半径r' (0 ～ r) で、均等に分布するように sqrt を使う
	float radius = sqrt(static_cast<float>(rand()) / RAND_MAX) * _r;

	// 円内の点を計算
	_tPos.x = static_cast<int>(_myPos.x + radius * cos(theta));
	_tPos.z = static_cast<int>(_myPos.z + radius * sin(theta));

	//出現位置の指定しなおし
	_myPos = createPos_[create(gen)];
}

bool EnemyManager::IsOverlap(VECTOR& _tPos, float _minDist)
{
	//1体も作られていないなら判定しない
	if (activeNum_ <= 0)
		return false;

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

void EnemyManager::DeleteAllEnemy(void)
{
	for (int i = activeNum_ - 1; i >= 0;i--) {
		if (activeEnemys_[i] == nullptr)
			continue;

		//敵をすべて削除
		activeEnemys_[i]->Destroy();
		delete activeEnemys_[i];
		activeEnemys_[i] = nullptr;
		activeTypeNum_[static_cast<int>(activeEnemysType_[i])]--;
		activeEnemysType_[i] = TYPE::MAX;
		activeNum_--;
	}
}

void EnemyManager::DeathEnemy(int _num)
{
	//倒された敵の経験値を保存
	allExp_ += activeEnemys_[_num]->GetExp();
	dunkCnt_++;

	//倒された敵の消去
	activeEnemys_[_num]->Destroy();
	delete activeEnemys_[_num];

	//総数の減少
	//この時点でactiveNum_は配列の末尾の番号を示すようになる。
	activeNum_--;
	activeTypeNum_[static_cast<int>(activeEnemysType_[_num])]--;

	//有効な敵の種類の初期化
	activeEnemysType_[_num] = TYPE::MAX;

	//配列の空きを埋めるためのソート
	//配列の末尾の物を空きに移動させる
	//倒された敵が末尾の物なら処理はしない
	if (_num == activeNum_)return;

	//挿入処理
	//deleteをすると移行された側の情報も消えるのでnullptr設定のみ　移動方法はstd::moveでもあり
	activeEnemys_[_num] = activeEnemys_[activeNum_];
	activeEnemys_[activeNum_] = nullptr;
	activeEnemysType_[_num] = activeEnemysType_[activeNum_];
}

void EnemyManager::ProcessChangePhase(const int _phase)
{
	//敵の削除
	DeleteAllEnemy();
	dunkCnt_ = 0;

	//敵作成数
	int createNum = 0;

	//フェーズによって作成する敵の数を変える
	if (_phase == 1)
		createNum = PHASE_ONE_INIT_CREATE_ENEMY;
	else if (_phase == 2)
		createNum = PHASE_TWO_INIT_CREATE_ENEMY;
	else if (_phase == 3)
	{
		//ボスの作成
		CreateBoss();
		return;
	}
	else 
		return;

	for (int i = 0; i < createNum; i++)
	{
		//敵の初期生成
		CreateEnemy();
	}
 }