#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Character/PlayerBase.h"
#include "../Object/Character/PlayableChara/AxeMan.h"
#include"../Object/Character/Enemy.h"
#include"../Object/Character/EnemySort/EneAxe.h"
#include"../Object/Character/EnemySort/EneArcher.h"
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

#ifdef _DEBUG_COL
	playerTest_ = new PlAxe(SceneManager::PLAY_MODE::USER);
	playerTest_->Init();
	playerTest_->ChangeControll(SceneManager::CNTL::KEYBOARD);
	enemyTest_ = new EneArcher();
	enemyTest_->Init();
#endif

	// �J�������[�h�@�F�t���[�J����
	Camera* camera = SceneManager::GetInstance().GetCamera();
	//�J�����̑Ώېݒ�
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

	auto ePos = enemyTest_->GetPos();
	auto eAtk = enemyTest_->GetAtk();
	auto pPos = playerTest_->GetPos();
	auto pAtk = playerTest_->GetAtk();

	//�G�����G
	if (col.Search(ePos, pPos, enemyTest_->GetSearchRange()))
	{
		//�ړ����J�n
		enemyTest_->SetIsMove(true);
	}
	else
	{
		//�ړ����~
		enemyTest_->SetIsMove(false);
	}


	if (col.Search(ePos, pPos, enemyTest_->GetAtkStartRange()) 
		&& enemyTest_->GetState() == Enemy::STATE::NORMAL)
	{
		//��Ԃ�ύX
		enemyTest_->ChangeState(Enemy::STATE::ALERT);
	}


	//�v���C���[�����G
	if (col.Search(playerTest_->GetPos(), enemyTest_->GetPos(), playerTest_->GetAtkStartRange()))
	{
		//��Ԃ�ύX
		playerTest_->ChangeState(PlayerBase::STATE::ATTACK);
	}

	//�v���C���[�U������
	//�U�����ł��肻�̍U������x���������Ă��Ȃ���
	if (pAtk.IsAttack()&&!pAtk.isHit_)
	{
		//�����蔻��
		if (col.IsHitAtk(playerTest_, enemyTest_))	
		{
			//��e
			enemyTest_->Damage(5, 4);				
			//�U������̏I��
			playerTest_->SetIsHit(true);			
		}
	}

	//�G�̍U������
	//�A�^�b�N���ł���U�����肪�I�����Ă��Ȃ��Ƃ�
	if (eAtk.IsAttack() && !eAtk.isHit_)
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
