#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/Collision.h"
#include "../Object/Grid.h"
#include "../Object/Character/PlayerBase.h"
#include"../Object/Character/Enemy.h"
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

	// �O���b�h���@�\�̎��Ԃ𐶐�
	grid_ = new Grid();
	grid_->Init();	

#ifdef _DEBUG_COL
	playerTest_ = new PlayerBase();
	playerTest_->Init();
	enemyTest_ = new Enemy();
	enemyTest_->Init();
#endif


	// �J�������[�h�@�F�t���[�J����
	Camera* camera = SceneManager::GetInstance().GetCamera();
	//�J�����̑Ώېݒ�
	camera->SetFollow(&playerTest_->GetTransform());
	camera->ChangeMode(Camera::MODE::FOLLOW_SPRING);
}

void GameScene::Update(void)
{
	grid_->Update();


#ifdef _DEBUG_COL
	playerTest_->Update();
	enemyTest_->Update();
#endif

	//�����蔻��
	Collision();
}

void GameScene::Draw(void)
{
	sky_->Draw();
	grid_->Draw();
#ifdef _DEBUG_COL
	playerTest_->Draw();
	enemyTest_->Draw();
#endif
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
#ifdef _DEBUG_COL
	playerTest_->Destroy();
	delete playerTest_;
	enemyTest_->Destroy();
	delete enemyTest_;
#endif
}


//�����蔻��i�����ڂɊ�������̂̂݁j
void GameScene::Collision(void)
{
	auto& col = Collision::GetInstance();


#ifdef _DEBUG_COL


	//�G�����G
	if (col.Search(enemyTest_->GetPos(), playerTest_->GetPos(), 2.0f))
	{
		//�ړ����J�n
	}

	//�v���C���[�U������
	//�U�����ł��肻�̍U������x���������Ă��Ȃ���
	if (playerTest_->GetAtk().IsAttack()&&!playerTest_->GetAtk().isHit_)
	{
		//�����蔻��
		if (col.IsHitAtk(playerTest_, enemyTest_))	
		{
			//��e
			enemyTest_->Damage(2, 4);				
			//�U������̏I��
			playerTest_->SetIsHit(true);			
		}
	}

	//�G�̍U������
	//�A�^�b�N���ł���U�����肪�I�����Ă��Ȃ��Ƃ�
	if (enemyTest_->GetAtk().IsAttack() && !enemyTest_->GetAtk().isHit_)
	{
		//�U����������͈͂ł���A�v���C���[��������Ă��Ȃ��Ƃ�
		if (col.IsHitAtk(enemyTest_, playerTest_) && !playerTest_->IsDodge())
		{
			//�_���[�W
			playerTest_->Damage();
			//�g�p�����U���𔻒�I����
			enemyTest_->SetIsHit(true);
		}
	}

#endif


}
