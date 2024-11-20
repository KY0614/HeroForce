#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/Collision.h"
#include "../Object/Grid.h"
#include "../Object/Character/PlayerBase.h"
#include "../Object/Character/PlayableChara/PlAxeMan.h"
#include"../Object/Character/Enemy.h"
#include"../Object/Character/EnemySort/EneAxe.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/StageBase.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/System/LevelBase.h"
#include "GameScene.h"



#include "../Manager/InputManager.h"

#define   _DEBUG_COL

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
	playerTest_ = new PlAxe(SceneManager::PLAY_MODE::USER);
	playerTest_->Init();
	playerTest_->ChangeControll(SceneManager::CNTL::KEYBOARD);
	enemyTest_ = new EneAxe();
	enemyTest_->Init();
#endif

	//�v���C���[�ݒ�
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		players_[i] = std::make_unique<PlAxe>(SceneManager::PLAY_MODE::USER);
		players_[i].get()->Init();
	}

	//�G�̐���(�Ƃ肠������̂���)
	std::unique_ptr<Enemy> e=std::make_unique<EneAxe>();
	e.get()->Init();
	enemys_.push_back(e);


	//�J�����̐ݒ�
	auto cameras = SceneManager::GetInstance().GetCameras();
	for (int i = 0; i < cameras.size(); i++)
	{
		cameras[i].get()->SetFollow(&players_[i].get()->GetTransform());
		cameras[i].get()->ChangeMode(Camera::MODE::FOLLOW_SPRING);
	}
}

void GameScene::Update(void)
{
	//grid_->Update();
	level_->Update();

	//�v���C���[�@�����𓮂����Ă��܂�
	players_[0].get()->Update();

	for (auto& e : enemys_)
	{
		e.get()->Update();
	}
	

#ifdef _DEBUG_COL
	playerTest_->Update();
	enemyTest_->SetTargetPos(playerTest_->GetPos());
	enemyTest_->Update();
#endif

	auto& ins = InputManager::GetInstance();
	auto& mng = SceneManager::GetInstance();
	//�X�y�[�X��������^�C�g���ɖ߂�
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		mng.ReturnSolo();
		mng.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	//�����蔻��
	Collision();
}

void GameScene::Draw(void)
{
	sky_->Draw();
	//grid_->Draw();
#ifdef _DEBUG_COL
	playerTest_->Draw();
	enemyTest_->Draw();
#endif
	for (auto& p : players_)
		p.get()->Draw();

	for (auto& e : enemys_)
	{
		e.get()->Draw();
	}

	stage_->Draw();
	level_->Draw();

	playerTest_->DrawDebug();
}

void GameScene::Release(void)
{
	level_->Release();
	delete level_;
	delete sky_;
	stage_->Release();
	delete stage_;
	delete grid_;


	for (auto& p : players_)
	{
		p.get()->Destroy();
	}

	for (auto& e : enemys_)
	{
		e.get()->Destroy();
	}

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
