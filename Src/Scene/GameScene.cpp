#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/Collision.h"
#include "../Object/Character/PlayerBase.h"
#include "../Object/Character/PlayableChara/PlAxeMan.h"
#include "../Object/Character/PlayableChara/PlArcher.h"
#include"../Object/Character/Enemy.h"
#include"../Object/Character/Chiken/ChickenManager.h"
#include"../Object/Character/EnemySort/EneAxe.h"
#include"../Object/Character/EnemySort/EneGolem.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/System/LevelScreenManager.h"
#include "../Object/System/UnitPositionLoad.h"
#include"FazeResult.h"
#include "GameScene.h"

#include "../Manager/Generic/InputManager.h"



GameScene::GameScene(void)
{
	stage_ = nullptr;
	sky_ = nullptr;
	fader_ = nullptr;
	level_ = nullptr;
	chicken_ = nullptr;
	unitLoad_ = nullptr;
	fazeResult_ = nullptr;

	isPhaseChanging_ = false;
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	unitLoad_ = std::make_unique<UnitPositionLoad>();
	unitLoad_->Init();

	stage_ = std::make_unique<StageManager>();
	stage_->Init();

	sky_ = std::make_unique<SkyDome>();
	sky_->Init();

	level_ = std::make_unique<LevelScreenManager>();
	level_->Init();


#ifdef _DEBUG_COL
	playerTest_ = new PlAxe(SceneManager::PLAY_MODE::USER);
	playerTest_->Init();
	playerTest_->ChangeControll(SceneManager::CNTL::KEYBOARD);
	enemyTest_ = new EneAxe();
	enemyTest_->Init();
#endif

	chicken_ = std::make_unique<ChickenManager>(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::CPU));
	chicken_->Init();

	//�v���C���[�ݒ�
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		players_[i] = std::make_unique<PlArcher>(SceneManager::PLAY_MODE::USER, InputManager::JOYPAD_NO::PAD1);
		players_[i]->Init();
	}

	//�G�̐���(�Ƃ肠������̂���)
	std::unique_ptr<Enemy> e=std::make_unique<EneAxe>();
	e->Init();
	enemys_.push_back(std::move(e));

	std::unique_ptr<Enemy> g = std::make_unique<EneGolem>();
	g->Init();
	enemys_.push_back(std::move(g));

	//�J�����̐ݒ�
	auto cameras = SceneManager::GetInstance().GetCameras();
	for (int i = 0; i < cameras.size(); i++)
	{
		cameras[i]->SetFollow(&players_[i]->GetTransform());
		cameras[i]->ChangeMode(Camera::MODE::FOLLOW_SPRING);
	}

	//�t�F�[�_�[�̍쐬
	fader_ = std::make_unique<Fader>();
	fader_->Init();
	//�t�F�[�Y���U���g�̍쐬
	fazeResult_ = std::make_unique<FazeResult>();
	fazeResult_->Init();
}

void GameScene::Update(void)
{
	//�t�F�[�Y���U���g
	if (isFazeRezult_)
	{
		fazeResult_->Update();

		if (fazeResult_->IsEnd())
		{
			//�t�F�[�Y���U���g���I�������̂Ŗ��]
			fader_->SetFade(Fader::STATE::FADE_IN);
			fazeResult_->Reset();
			isFazeRezult_ = false;
		}
		return;
	}

	//�t�F�[�Y�J�ڒ�
	if (isPhaseChanging_)
	{
		fader_->Update();
		//�t�F�[�h��������
		Fade();
		return;
	}

	//grid_->Update();
	level_->Update();

	//�v���C���[�@�����𓮂����Ă��܂�
	players_[0]->Update();

	for (auto& e : enemys_)
	{
		e->SetTargetPos(players_[0]->GetPos());
		e->Update();
	}
	

#ifdef _DEBUG_COL
	playerTest_->Update();
	enemyTest_->SetTargetPos(playerTest_->GetPos());
	enemyTest_->Update();
#endif
	chicken_->SetTargetPos(players_[0]->GetPos());
	chicken_->Update();

	//�����蔻��
	Collision();

	//�����v�f�̔��f
	LevelUpReflection();



	//���������
	auto& ins = InputManager::GetInstance();
	auto& mng = SceneManager::GetInstance();
	//�X�y�[�X��������^�C�g���ɖ߂�
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		mng.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		ChangePhase();
	}
}

void GameScene::Draw(void)
{
	if (isFazeRezult_)
	{
		fazeResult_->Draw();
		return;
	}

	sky_->Draw();
#ifdef _DEBUG_COL
	playerTest_->Draw();
	enemyTest_->Draw();
#endif

	for (auto& p : players_)
		p->Draw();

	for (auto& e : enemys_){
		e->Draw();
	}

	stage_->Draw();
	chicken_->Draw();
	level_->Draw();

	fader_->Draw();

	if (fader_->GetState() == Fader::STATE::FADE_KEEP)
	{
		DrawPhase();
	}
}

void GameScene::Release(void)
{
	level_->Release();
	stage_->Release();

	SceneManager::GetInstance().ResetCameras();
	SceneManager::GetInstance().ReturnSolo();

	for (auto& p : players_)
	{
		p->Destroy();
	}

	for (auto& e : enemys_)
	{
		e->Destroy();
	}

#ifdef _DEBUG_COL
	playerTest_->Destroy();
	delete playerTest_;
	enemyTest_->Destroy();
	delete enemyTest_;
#endif
}


//�����蔻��i�����ڂɊ�������̂̂݁j
//�����蔻�葍��
void GameScene::Collision(void)
{
	auto& col = Collision::GetInstance();

	CollisionEnemy();
	CollisionPlayer();

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


	if (col.Search(ePos, pPos, enemyTest_->GetAtkStartRange())&&enemyTest_->GetState()==Enemy::STATE::NORMAL)
	{
		//��Ԃ�ύX
		enemyTest_->ChangeState(Enemy::STATE::ALERT);
	}

		//�v���C���[�U������
		//�U�����ł��肻�̍U������x���������Ă��Ȃ���
		if (pAtk.IsAttack() && !pAtk.isHit_)
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

		//�v���C���[��CPU�̎������T�[�`������
		if (playerTest_->GetPlayMode() == SceneManager::PLAY_MODE::CPU)
		{
			//�v���C���[�����G
			if (col.Search(pPos, ePos, playerTest_->GetSearchRange())
				&& enemyTest_->IsAlive() && !playerTest_->GetIsCalledPlayer())
			{
				//�G���T�[�`��������Ԃ�
				playerTest_->SetisEnemySerch(true);
				playerTest_->SetTargetPos(ePos);
			}
			else if (!enemyTest_->IsAlive())
			{
				//�G���T�[�`��������Ԃ�
				playerTest_->SetisEnemySerch(false);
			}

			if (col.Search(playerTest_->GetPos(), enemyTest_->GetPos(), playerTest_->GetAtkStartRange())
				&& playerTest_->GetState() == PlayerBase::CPU_STATE::NORMAL
				&& enemyTest_->IsAlive()
				&& !playerTest_->GetIsCalledPlayer())
			{
				//��Ԃ�ύX
				playerTest_->ChangeState(PlayerBase::CPU_STATE::ATTACK);
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

//�G�֌W�̓����蔻��
void GameScene::CollisionEnemy(void)
{
	auto& col = Collision::GetInstance();

	//�����蔻��(��ɍ��G)
	for (auto& e : enemys_)
	{
		//�G�l�̈ʒu�ƍU�����擾
		VECTOR ePos = e->GetPos();
		UnitBase::ATK eAtk = e->GetAtk();

		//�����镪����(�̂��ɑS�����ɕς���)
		VECTOR pPos = players_[0]->GetPos();

		//���G
		//�͈͓��ɓ����Ă���Ƃ�
		if (col.Search(ePos, pPos, e->GetSearchRange())) {
			//�ړ����J�n
			e->SetIsMove(true);
		}else {
			//�ړ����~
			e->SetIsMove(false);
		}

		//�ʏ��Ԏ� && �U���͈͓��Ƀv���C���[����������U�����J�n
		if (col.Search(ePos, pPos, e->GetAtkStartRange()) && e->GetState() == Enemy::STATE::NORMAL) {
			//��Ԃ�ύX
			e->ChangeState(Enemy::STATE::ALERT);
		}

		//�U������
		//�A�^�b�N�� && �U�����肪�I�����Ă��Ȃ��Ƃ�
		if (eAtk.IsAttack() && !eAtk.isHit_)
		{
			//�e�v���C���[�Ɠ����蔻������
			for (auto& p : players_)
			{
				//�U����������͈� && �v���C���[��������Ă��Ȃ��Ƃ�
				if (col.IsHitAtk(*e, *p) && !playerTest_->IsDodge())
				{
					//�_���[�W
					p->Damage();
					//�g�p�����U���𔻒�I����
					e->SetIsHit(true);
				}
			}
		}
	}
}

void GameScene::CollisionPlayer(void)
{
	auto& col = Collision::GetInstance();

	for (auto& p : players_)
	{
		auto pPos = p->GetPos();
		auto pAtk = p->GetAtk();

		//�v���C���[��CPU�̎������T�[�`������
		if (p->GetPlayMode() == SceneManager::PLAY_MODE::CPU)CollisionPlayerCPU(*p, pPos);

		//�v���C���[�U������
		//�U�����Ă��Ȃ� || �U�������łɓ������Ă���
		if (!pAtk.IsAttack() || pAtk.isHit_)continue;

		for (auto& e : enemys_)
		{
			//�����蔻��
			if (col.IsHitAtk(*p, *e)) {
				//��e
				e->Damage(5, 4);
				//�U������̏I��
				p->SetIsHit(true);
			}
		}
		
	}
}

void GameScene::CollisionPlayerCPU(PlayerBase& _player, const VECTOR& _pPos)
{
	auto& col = Collision::GetInstance();

	//�G���T�[�`������
	_player.SetisEnemySerch(false);

	//�G�̌̕��s��
	for (auto& e : enemys_)
	{
		//�G�����S���Ă����珈�����Ȃ�
		if (!e->IsAlive())continue;

		//�G�l�̈ʒu�ƍU�����擾
		VECTOR ePos = e->GetPos();

		//�v���C���[�����G
		if (col.Search(_pPos, ePos, _player.GetSearchRange())
			&& !_player.GetIsCalledPlayer())
		{
			//�G���T�[�`��������Ԃ�
			_player.SetisEnemySerch(true);
			_player.SetTargetPos(ePos);
		}

		if (col.Search(_player.GetPos(), enemyTest_->GetPos(), _player.GetAtkStartRange())
			&& _player.GetState() == PlayerBase::CPU_STATE::NORMAL
			&& !_player.GetIsCalledPlayer())
		{
			//��Ԃ�ύX
			_player.ChangeState(PlayerBase::CPU_STATE::ATTACK);
		}
	}
}

void GameScene::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// ���]��
		if (fader_->IsEnd())	//���]�I��
		{
			// ���]���I��������A�t�F�[�h�����I��
			fader_->SetFade(Fader::STATE::NONE);
			isPhaseChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// �Ó]��
		if (fader_->IsEnd())	//�Ó]�I��
		{
			//�����̏������t�F�[�Y�J�ڂ��킩��₷���悤�Ȃ�n���ɕς���B
			// �Ó]���疾�]��
			//������x���S�Ó]�̎��Ԃ�p��
			fader_->SetFade(Fader::STATE::FADE_KEEP);

			//�t�F�[�Y���U���g�̏�Ԃ�
			isFazeRezult_ = true;
		}
		break;
	case Fader::STATE::FADE_KEEP:
		UpdatePhase();
		break;
	default:
		break;
	}
}

//�t�F�[�Y�֌W
//*********************************************************
void GameScene::ChangePhase(void)
{
	isPhaseChanging_ = true;
	fader_->SetFade(Fader::STATE::FADE_OUT);
}

void GameScene::UpdatePhase(void)
{
	/*phaseCnt_++;
	if (phaseCnt_ > PHASE_TIME)
	{
		fader_->SetFade(Fader::STATE::FADE_IN);
		phaseCnt_ = 0;
	}*/
}

void GameScene::DrawPhase(void)
{
	DrawString(0, 0, "�t�F�[�Y�J�ڒ�", 0xffffff, true);
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
