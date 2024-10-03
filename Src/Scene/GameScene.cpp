#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/SpaceDome.h"
#include "../Object/Ship/PlayerShip.h"
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
	
	// プレイヤー機能の実態を生成
	playerShip_ = new PlayerShip();
	playerShip_->Init();

	// スカイドーム
	spaceDome_ = new SpaceDome(playerShip_->GetTransform());
	spaceDome_->Init();

	// カメラモード　：フリーカメラ
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(&playerShip_->GetTransform());
	camera->ChangeMode(Camera::MODE::FOLLOW);
}

void GameScene::Update(void)
{
	spaceDome_->Update();
	grid_->Update();
	playerShip_->Update();
}

void GameScene::Draw(void)
{
	spaceDome_->Draw();

	grid_->Draw();

	playerShip_->Draw();
}

void GameScene::Release(void)
{
	playerShip_->Release();;
	delete playerShip_;

	delete grid_;

	spaceDome_->Release();
	delete spaceDome_;
}
