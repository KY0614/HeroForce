#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/StageBase.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/System/LevelBase.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	stage_ = new StageBase();
	stage_->Init();

	sky_ = new SkyDome();
	sky_->Init();

	level_ = new LevelBase();
	level_->Init();

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
	sky_->Update();
	level_->Update();
}

void GameScene::Draw(void)
{
	sky_->Draw();
	grid_->Draw();
	stage_->Draw();
	level_->Draw();
}

void GameScene::Release(void)
{
	delete level_;
	delete sky_;
	stage_->Release();
	delete stage_;
	delete grid_;
}
