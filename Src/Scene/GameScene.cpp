#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
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


	// カメラモード　：フリーカメラ
	Camera* camera = SceneManager::GetInstance().GetCamera();
	//カメラの対象設定
	//camera->SetFollow(&playerShip_->GetTransform());
	camera->ChangeMode(Camera::MODE::FREE);
}

void GameScene::Update(void)
{
	grid_->Update();
}

void GameScene::Draw(void)
{
	grid_->Draw();
}

void GameScene::Release(void)
{
	delete grid_;
}
