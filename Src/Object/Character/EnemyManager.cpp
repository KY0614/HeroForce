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
	dunkCnt_ = 0;
}

EnemyManager::~EnemyManager()
{
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

	ProcessChangePhase(PHASE_ONE);
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

	//敵の生成用
	std::unique_ptr<Enemy> enm;

	//ループ制限
	int retry = 0;

	//乱数で種類決める
	int typeRand = GetRand(static_cast<int>(TYPE::MAX) - 1);
	
	//ゴーレム　又は　同じ種類の敵数制限に引っかかったらもう一度
	while (typeRand == static_cast<int>(TYPE::GOLEM)|| activeTypeNum_[typeRand] >= ONETYPE_MAX && retry < RETRY_LIMIT)
	{
		//ランダムで敵を決める
		typeRand = GetRand(static_cast<int>(TYPE::MAX) - 1);
		
		//リトライ回数
		retry++;
	} 

	//TYPEに変換
	TYPE type = static_cast<TYPE>(typeRand);

	//ループ制限を超えていたら処理をやめる
	if (retry >= RETRY_LIMIT)return;

	//生成地点をランダムで決める
	int createPosPoint = GetRand(createPos_.size() - 1);
	//生成相対座標
	VECTOR createLocalPos = createPos_[createPosPoint];
	//敵の数分座標をずらす
	createLocalPos.x += activeNum_ * Enemy::COL_RADIUS;
	createLocalPos.y = 0.0f;
	createLocalPos.z += activeNum_ * Enemy::COL_RADIUS;

	//インスタンス生成
	switch (type)
	{
	case EnemyManager::TYPE::ARCHER:
		enm = std::make_unique<EneArcher>(createLocalPos);
		break;
	case EnemyManager::TYPE::AXE:
		enm = std::make_unique <EneAxe>(createLocalPos);
		break;
	case EnemyManager::TYPE::BRIG:
		enm = std::make_unique <EneBrig>(createLocalPos);
		break;
	case EnemyManager::TYPE::GOLEM:
		//ゴーレムはボスキャラなのでここでは生成されない
		return;
	case EnemyManager::TYPE::MAGE:
		enm = std::make_unique <EneMage>(createLocalPos);
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
	activeEnemys_[activeNum_] = std::move(enm);

	//敵の種類を保存
	activeEnemysType_[activeNum_] = type;

	//カウンタ増加
	int activeEnmType = static_cast<int>(activeEnemysType_[activeNum_]);
	activeTypeNum_[activeEnmType]++;
	activeNum_++;
}

void EnemyManager::CreateBoss(void)
{
	//敵が最大数いたら生成処理を行わない
	if (activeNum_ >= ENEMY_MAX)return;

	//敵の生成
	std::unique_ptr<Enemy> enm;

	//生成相対座標
	VECTOR createLocalPos = createPos_[GOLEM_SPAWN];

	//ゴーレムの生成
	enm = std::make_unique<EneGolem>(createLocalPos);

	//念のためのエラー回避用
	if (enm == nullptr)assert("敵の生成で問題がありました。");

	//敵の初期化
	enm->Init();

	//敵の更新等を掛けるやつをセット
	activeEnemys_[activeNum_] = std::move(enm);

	//敵の種類を保存
	activeEnemysType_[activeNum_] = TYPE::GOLEM;

	//カウンタ増加
	int activeEnmType = static_cast<int>(activeEnemysType_[activeNum_]);
	activeTypeNum_[activeEnmType]++;
	activeNum_++;
}

void EnemyManager::DeleteAllEnemy(void)
{
	for (int i = activeNum_ - 1; i >= 0;i--) {
		if (activeEnemys_[i] == nullptr || activeEnemysType_[i] == TYPE::MAX)
			continue;

		//敵をすべて削除
		activeEnemys_[i]->Destroy();
		activeEnemys_[i].reset();
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
	activeEnemys_[_num].reset();

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
	//末尾配列を消去した配列に移す
	activeEnemys_[_num] = std::move(activeEnemys_[activeNum_]);
	activeEnemysType_[_num] = activeEnemysType_[activeNum_];
}

void EnemyManager::ProcessChangePhase(const int _phase)
{
	//敵の削除
	DeleteAllEnemy();

	//倒した敵の数初期化
	dunkCnt_ = 0;

	//敵の初期作成数
	int createNum = 0;

	//フェーズによって作成する敵の数を変える
	switch (_phase)
	{
	case PHASE_ONE:
		//フェーズ1の時
		createNum = PHASE_ONE_INIT_CREATE_ENEMY;
		break;

	case PHASE_TWO:
		//フェーズ2の時
		createNum = PHASE_TWO_INIT_CREATE_ENEMY;
		break;

	case PHASE_LAST:
		//ボスの生成
		CreateBoss();
		return;

	default:
		return;
	}

	for (int i = 0; i < createNum; i++)
	{
		//敵の初期生成
		CreateEnemy();
	}
 }