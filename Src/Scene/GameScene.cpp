#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/Collision.h"
#include "../Object/Grid.h"
#include "../Object/Character/PlayerBase.h"
#include"../Object/Character/Enemy.h"
#include"../Object/Character/Chiken/ChickenManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/System/LevelScreenManager.h"
#include "../Object/System/UnitPositionLoad.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	unitLoad_ = new UnitPositionLoad();
	unitLoad_->Init();

	stage_ = new StageManager();
	stage_->Init();

	sky_ = new SkyDome();
	sky_->Init();

	level_ = new LevelScreenManager();
	level_->Init();


	// グリッド線機能の実態を生成
	grid_ = new Grid();
	grid_->Init();	

#ifdef _DEBUG_COL
	playerTest_ = new PlayerBase();
	playerTest_->Init();
	enemyTest_ = new Enemy();
	enemyTest_->Init();	
	chicken_ = new ChickenManager(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::CPU));
	chicken_->Init();
#endif


	// カメラモード　：フリーカメラ
	Camera* camera = SceneManager::GetInstance().GetCamera();
	//カメラの対象設定
	camera->SetFollow(&playerTest_->GetTransform());
	camera->ChangeMode(Camera::MODE::FOLLOW_SPRING);
}

void GameScene::Update(void)
{
	//grid_->Update();
	level_->Update();

#ifdef _DEBUG_COL
	playerTest_->Update();
	enemyTest_->SetTargetPos(playerTest_->GetPos());
	enemyTest_->Update();
#endif

	chicken_->SetTargetPos(playerTest_->GetPos());
	chicken_->Update();

	//あたり判定
	Collision();

	//強化要素の反映
	LevelUpReflection();
}

void GameScene::Draw(void)
{
	sky_->Draw();
	//grid_->Draw();
#ifdef _DEBUG_COL
	playerTest_->Draw();
	enemyTest_->Draw();
#endif
	stage_->Draw();
	chicken_->Draw();
	level_->Draw();
}

void GameScene::Release(void)
{
	level_->Release();
	delete level_;
	delete sky_;
	stage_->Release();
	delete stage_;
	delete grid_;
	delete unitLoad_;
	delete chicken_;
#ifdef _DEBUG_COL
	playerTest_->Destroy();
	delete playerTest_;
	enemyTest_->Destroy();
	delete enemyTest_;
#endif
}


//当たり判定（他項目に干渉するもののみ）
void GameScene::Collision(void)
{
	auto& col = Collision::GetInstance();


#ifdef _DEBUG_COL

	auto ePos = enemyTest_->GetPos();
	auto eAtk = enemyTest_->GetAtk();
	auto pPos = playerTest_->GetPos();
	auto pAtk = playerTest_->GetAtk();

	//敵側索敵
	if (col.Search(ePos, pPos, enemyTest_->GetSearchRange()))
	{
		//移動を開始
		enemyTest_->SetIsMove(true);
	}
	else
	{
		//移動を停止
		enemyTest_->SetIsMove(false);
	}


	if (col.Search(ePos, pPos, enemyTest_->GetAtkStartRange()))
	{
		//状態を変更
		enemyTest_->ChangeState(Enemy::STATE::ALERT);
	}

	//プレイヤー攻撃判定
	//攻撃中でありその攻撃が一度も当たっていないか
	if (pAtk.IsAttack()&&!pAtk.isHit_)
	{
		//当たり判定
		if (col.IsHitAtk(playerTest_, enemyTest_))	
		{
			//被弾
			enemyTest_->Damage(5, 4);				
			//攻撃判定の終了
			playerTest_->SetIsHit(true);			
		}
	}

	//敵の攻撃判定
	//アタック中であり攻撃判定が終了していないとき
	if (eAtk.IsAttack() && !eAtk.isHit_)
	{
		//攻撃が当たる範囲であり、プレイヤーが回避していないとき
		if (col.IsHitAtk(enemyTest_, playerTest_) && !playerTest_->IsDodge())
		{
			//ダメージ
			playerTest_->Damage();
			//使用した攻撃を判定終了に
			enemyTest_->SetIsHit(true);
		}
	}

#endif


}

void GameScene::LevelUpReflection()
{
	//ステート確認
	if (level_->GetState() == LevelScreenManager::STATE::FIN)
	{
		return;
	}

	//ここでプレイヤーの強化を反映
	switch (level_->GetType())
	{
	case LevelScreenManager::TYPE::ATTACK:
		break;

	case LevelScreenManager::TYPE::DEFENSE:
		break;

	case LevelScreenManager::TYPE::LIFE:
		break;

	case LevelScreenManager::TYPE::SPEED:
		break;

	default:
		break;
	}
}
