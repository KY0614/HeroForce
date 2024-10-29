#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/Collision.h"
#include "../Object/Grid.h"
#include "../Object/Character/PlayerBase.h"
#include"../Object/Character/Enemy.h"
#include "../Object/Common/Transform.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	

	// グリッド線機能の実態を生成
	grid_ = new Grid();
	grid_->Init();	

#ifdef _DEBUG_COL
	playerTest_ = new PlayerBase();
	playerTest_->Init();
	enemyTest_ = new Enemy();
	enemyTest_->Init();
#endif


	// カメラモード　：フリーカメラ
	Camera* camera = SceneManager::GetInstance().GetCamera();
	//カメラの対象設定
	//camera->SetFollow(&playerShip_->GetTransform());
	camera->ChangeMode(Camera::MODE::NONE);
}

void GameScene::Update(void)
{
	grid_->Update();

	auto& col = Collision::GetInstance();

#ifdef _DEBUG_COL
	playerTest_->Update();
	enemyTest_->Update();

	//プレイヤー攻撃判定
	if (playerTest_->GetAtk().IsAttack())
	{
		if (col.IsHitAtk(playerTest_, enemyTest_))
		{
			enemyTest_->Damage(2, 4);
		}
	}

	//敵の攻撃判定
	//アタック中であり攻撃判定が終了していないとき
	if (enemyTest_->GetAtk().IsAttack() && !enemyTest_->GetAtk().isHit_)
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

void GameScene::Draw(void)
{
	grid_->Draw();
#ifdef _DEBUG_COL
	playerTest_->Draw();
	enemyTest_->Draw();
#endif
}

void GameScene::Release(void)
{
	delete grid_;
#ifdef _DEBUG_COL
	playerTest_->Destroy();
	delete playerTest_;
	enemyTest_->Destroy();
	delete enemyTest_;
#endif
}
