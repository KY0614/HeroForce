#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/PlayerBase.h"
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
	

	// �O���b�h���@�\�̎��Ԃ𐶐�
	grid_ = new Grid();
	grid_->Init();	

	//�v���C���[�x�[�X
	playerB_ = new PlayerBase();
	playerB_->Init();


	// �J�������[�h�@�F�t���[�J����
	Camera* camera = SceneManager::GetInstance().GetCamera();
	//�J�����̑Ώېݒ�
	//camera->SetFollow(&playerShip_->GetTransform());
	camera->ChangeMode(Camera::MODE::NONE);
}

void GameScene::Update(void)
{
	grid_->Update();
	playerB_->Update();
}

void GameScene::Draw(void)
{
	grid_->Draw();
	playerB_->Draw();
}

void GameScene::Release(void)
{
	delete grid_;
}
