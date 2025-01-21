#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/Collision.h"
#include"../Manager/GameSystem/Timer.h"

#include"../Object/Character/EnemySort/Enemy.h"
#include"../Object/Character/EnemyManager.h"
#include"../Object/Character/PlayerManager.h"
#include"../Object/Character/Chiken/ChickenManager.h"
#include"../Object/Character/EnemySort/EneAxe.h"
#include"../Object/Character/EnemySort/EneGolem.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/System/LevelScreenManager.h"
#include "../Object/System/UnitPositionLoad.h"
#include "../Manager/GameSystem/DataBank.h"
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
	//�^�C�}�[�̐���
	Timer::CreateInstance();

	unitLoad_ = std::make_unique<UnitPositionLoad>();
	unitLoad_->Init();

	stage_ = std::make_unique<StageManager>();
	stage_->Init();

	sky_ = std::make_unique<SkyDome>();
	sky_->Init();

	level_ = std::make_unique<LevelScreenManager>();
	level_->Init();

	chicken_ = std::make_unique<ChickenManager>(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::CPU));
	chicken_->Init();

	//�v���C���[�ݒ�
	playerMng_ = std::make_unique<PlayerManager>();
	playerMng_->Init();

	//�G�}�l�[�W���̐���
	enmMng_ = std::make_unique<EnemyManager>();
	enmMng_->Init();

	//�J�����̐ݒ�
	auto cameras = SceneManager::GetInstance().GetCameras();
	for (int i = 0; i < cameras.size(); i++)
	{
		cameras[i]->SetFollow(&playerMng_->GetPlayer(i)->GetTransform());
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
	//�Q�[���I�[�o�[����
	if(IsGameOver())SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);

	//�t�F�[�Y���U���g
	if (isFazeRezult_)
	{
		fazeResult_->Update();

		if (fazeResult_->IsEnd())
		{
			//�t�F�[�Y���U���g���I�������̂Ŗ��]
			fader_->SetFade(Fader::STATE::FADE_IN);
			Timer::GetInstance().Reset();
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

	Timer::GetInstance().Update();
	//�^�C�}�[���I��������
	if (Timer::GetInstance().IsEnd())ChangePhase();

	level_->Update();


	//�v���C���[�̍X�V
	playerMng_->Update();


	//�G�̍X�V
	enmMng_->Update(playerMng_->GetPlayer(0)->GetPos());
	

	chicken_->SetTargetPos(playerMng_->GetPlayer(0)->GetPos());
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
	//�X�J�C�h�[��
	sky_->Draw();

	//�v���C���[
	playerMng_->Draw();

	//�G
	enmMng_->Draw();
	//�X�e�[�W
	stage_->Draw();
	//�`�L��
	chicken_->Draw();
	//���x��
	level_->Draw();
	//��������
	Timer::GetInstance().Draw();

	fader_->Draw();

	if (fader_->GetState() == Fader::STATE::FADE_KEEP)
	{
		DrawPhase();
	}
	DataBank& data = DataBank::GetInstance();
	DrawFormatString(0, 60, 0xff0000, "p1 role : %d", data.Output(1).role_);
	DrawFormatString(0, 80, 0xff0000, "p1 role : %d", data.Output(2).role_);
}

void GameScene::Release(void)
{
	level_->Release();
	stage_->Release();

	SceneManager::GetInstance().ResetCameras();
	SceneManager::GetInstance().ReturnSolo();

	playerMng_->Release();

	enmMng_->Release();
}


//�����蔻��i�����ڂɊ�������̂̂݁j
//�����蔻�葍��
void GameScene::Collision(void)
{
	auto& col = Collision::GetInstance();

	CollisionEnemy();
	CollisionPlayer();

}

//�G�֌W�̓����蔻��
void GameScene::CollisionEnemy(void)
{
	//�Փ˔���}�l�[�W���擾
	auto& col = Collision::GetInstance();
	//�G�̑����擾
	int maxCnt = enmMng_->GetActiveNum();

	//�����蔻��(��ɍ��G)
	for (int i = 0; i < maxCnt; i++)
	{
		//�G�̎擾
		Enemy* e = enmMng_->GetActiveEnemy(i);

		//�G�l�̈ʒu�ƍU�����擾
		VECTOR ePos = e->GetPos();
		UnitBase::ATK eAtk = e->GetAtk();

		//�����镪����(�̂��ɑS�����ɕς���)
		VECTOR pPos = playerMng_->GetPlayer(0)->GetPos();

		//���G
		//�͈͓��ɓ����Ă���Ƃ�
		if (col.Search(ePos, pPos, e->GetSearchRange())) {
			//�ړ����J�n
			e->SetIsMove(true);
		}
		else {
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
			for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
			{
				PlayerBase* p = playerMng_->GetPlayer(i);
				//�U����������͈� && �v���C���[��������Ă��Ȃ��Ƃ�
				if (col.IsHitAtk(*e, *p) && !p->IsDodge())
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
	//�Փ˔���}�l�[�W���擾
	auto& col = Collision::GetInstance();
	//�G�̑����擾
	int maxCnt = enmMng_->GetActiveNum();

	for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
	{
		PlayerBase* p = playerMng_->GetPlayer(i);

		auto pPos = p->GetPos();
		auto pAtk = p->GetAtk();

		//�v���C���[��CPU�̎������T�[�`������
		//if (p->GetPlayMode() == SceneManager::PLAY_MODE::CPU)CollisionPlayerCPU(*p, pPos);

		//�v���C���[�U������
		//�U�����Ă��Ȃ� || �U�������łɓ������Ă���
		if (!pAtk.IsAttack() || pAtk.isHit_)continue;

		for (int i = 0; i < maxCnt; i++)
		{
			//�G�̎擾
			Enemy* e = enmMng_->GetActiveEnemy(i);
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
	////�Փ˔���}�l�[�W���擾
	//auto& col = Collision::GetInstance();
	////�G�̑����擾
	//int maxCnt = enmMng_->GetActiveNum();

	////�G���T�[�`������
	//_player.SetisEnemySerch(false);


	////�G�̌̕��s��
	//for (int i = 0; i < maxCnt; i++)
	//{
	//	//�G�̎擾
	//	Enemy* e = enmMng_->GetActiveEnemy(i);

	//	//�G�����S���Ă����珈�����Ȃ�
	//	if (!e->IsAlive())continue;

	//	//�G�l�̈ʒu�ƍU�����擾
	//	VECTOR ePos = e->GetPos();

	//	//�v���C���[�����G
	//	if (col.Search(_pPos, ePos, _player.GetSearchRange())
	//		&& !_player.GetIsCalledPlayer())
	//	{
	//		//�G���T�[�`��������Ԃ�
	//		_player.SetisEnemySerch(true);
	//		_player.SetTargetPos(ePos);
	//	}

	//	if (col.Search(_player.GetPos(), ePos, _player.GetAtkStartRange())
	//		&& _player.GetState() == PlayerBase::CPU_STATE::NORMAL
	//		&& !_player.GetIsCalledPlayer())
	//	{
	//		//��Ԃ�ύX
	//		_player.ChangeState(PlayerBase::CPU_STATE::ATTACK);
	//	}
	//}
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

bool GameScene::IsGameOver(void)
{
	for (int i = 0; i < PlayerManager::PLAYER_NUM; i++) {
		PlayerBase* p = playerMng_->GetPlayer(i);

		if (p->IsAlive())return false;
	}
	return true;
}