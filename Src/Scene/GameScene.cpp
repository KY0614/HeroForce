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


	// �O���b�h���@�\�̎��Ԃ𐶐�
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


	// �J�������[�h�@�F�t���[�J����
	Camera* camera = SceneManager::GetInstance().GetCamera();
	//�J�����̑Ώېݒ�
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

	//�����蔻��
	Collision();

	//�����v�f�̔��f
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


	if (col.Search(ePos, pPos, enemyTest_->GetAtkStartRange()))
	{
		//��Ԃ�ύX
		enemyTest_->ChangeState(Enemy::STATE::ALERT);
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

void GameScene::LevelUpReflection()
{
	//�X�e�[�g�m�F
	if (level_->GetState() == LevelScreenManager::STATE::FIN)
	{
		return;
	}

	//�����Ńv���C���[�̋����𔽉f
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
