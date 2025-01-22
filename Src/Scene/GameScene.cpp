#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Decoration/EffectManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/Collision.h"
#include"../Manager/GameSystem/Timer.h"

#include"../Object/Character/EnemySort/Enemy.h"
#include"../Object/Character/EnemyManager.h"
#include"../Object/Character/PlayerManager.h"
#include "../Manager/GameSystem/DataBank.h"
#include"../Object/Character/Chiken/ChickenManager.h"
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
	fazeCnt_ = 1;
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


	//�v���C���[�ݒ�
	playerMng_ = std::make_unique<PlayerManager>();
	playerMng_->Init();

	//�G�}�l�[�W���̐���
	enmMng_ = std::make_unique<EnemyManager>(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::ENEMY));
	enmMng_->Init();

	chicken_ = std::make_unique<ChickenManager>(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::CPU),
		stage_->GetTtans(),
		playerMng_->GetPlayer(0)->GetTransform());
	chicken_->Init();



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
			//�t�F�[�Y�J�E���g����
			fazeCnt_++;
			if(fazeCnt_ >LAST_FAZE)SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
			//�t�F�[�Y���U���g���I�������̂Ŗ��]�y�у��U���g���Z�b�g
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

	
	//���x���X�V
	level_->Update();
	//���x��UP���̓L�����N�^�[�B���X�V���Ȃ�
	if (level_->IsLevelUp())return;

	//�������ԍX�V
	Timer::GetInstance().Update();
	//�^�C�}�[���I��������
	if (Timer::GetInstance().IsEnd())ChangePhase();

	//�v���C���[�̍X�V
	playerMng_->Update();


	//�G�̍X�V
	enmMng_->Update(playerMng_->GetPlayer(0)->GetPos());
	
	//�j���g���X�V
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
		mng.ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
	}

	//if (ins.IsTrgDown(KEY_INPUT_RETURN))
	//{
	//	ChangePhase();
	//}
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
}

void GameScene::Release(void)
{
	level_->Release();

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

	//�X�e�[�W�Ƃ̔���
	enmMng_->CollisionStage(stage_->GetTtans());

	//�����蔻��(��ɍ��G)
	for (int i = 0; i < maxCnt; i++)
	{
		//�G�̎擾
		Enemy* e = enmMng_->GetActiveEnemy(i);

		//�G�l�̈ʒu�ƍU�����擾
		VECTOR ePos = e->GetPos();
		UnitBase::ATK eAtk = e->GetAtk();

		//�e�v���C���[�Ɠ����蔻������
		for (int i = 0; i < PlayerManager::PLAYER_NUM; i++) {

			PlayerBase* p = playerMng_->GetPlayer(i);
			VECTOR pPos = p->GetPos();


			//���G
			//�͈͓��ɓ����Ă���Ƃ�
			if (col.Search(ePos, pPos, e->GetSearchRange())) e->SetIsMove(true);	//�ړ����J�n}
			else e->SetIsMove(false);		//�ړ����~

				//�ʏ��Ԏ� && �U���͈͓��Ƀv���C���[����������U�����J�n
			if (col.Search(ePos, pPos, e->GetAtkStartRange()) && e->GetState() == Enemy::STATE::NORMAL) {
				//��Ԃ�ύX
				e->ChangeState(Enemy::STATE::ALERT);
			}

			//�U������

			//�A�^�b�N�� && �U�����肪�I�����Ă��Ȃ��Ƃ�������������B����ȊO�͂��Ȃ��̂Ŗ߂�
			if (!(eAtk.IsAttack() && !eAtk.isHit_))continue;

			//�U����������͈� && �v���C���[��������Ă��Ȃ��Ƃ�
			if (col.IsHitAtk(*e, *p) && !p->IsDodge())
			{
				//�_���[�W
				p->SetDamage(e->GetCharaPow(), eAtk.pow_);
				//�g�p�����U���𔻒�I����
				e->SetIsHit(true);
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

	//�X�e�[�W�Ƃ̔���
	playerMng_->CollisionStage(stage_->GetTtans());

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
				e->SetDamage(p->GetCharaPow(), pAtk.pow_);
				//�|�����G�̑���
				if (!e->IsAlive())DunkEnmCnt_++;
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

//void GameScene::CollisionStageUnit()
//{
//	auto& col = Collision::GetInstance();
//	stage_->GetTtans();
//	for (auto& p : players_)
//	{
//		if (col.IsHitUnitStageObject(stage_->GetTtans().modelId, p->GetPos(), 20.0f))
//		{
//			p->SetPos(p->GetPrePos());
//		}
//	}
//}

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
	//���U���g�Ɋ֌W����f�[�^�����
	DataBank& data = DataBank::GetInstance();

	data.Input(DataBank::INFO::FAZE_DUNK_ENEMY, DunkEnmCnt_);	//�|�����G��
	data.Input(DataBank::INFO::ALIVE_CHICKEN, chicken_->GetAliveNum());		//�j���g��������

	//���U���g�Ŏ擾
	fazeResult_->SetResult();
	if (fazeCnt_ == LAST_FAZE)fazeResult_->SetLast();

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
	//�v���C���[���Ƃɋ������f
	int plNum = DataBank::GetInstance().Output(DataBank::INFO::USER_NUM);
	for (int i = 0; i < plNum; i++)
	{
		if (level_->GetPreType(i) != LevelScreenManager::TYPE::MAX)
		{
			level_->EffectSyne(*playerMng_->GetPlayer(i), i);
		}
	}
	
	//�X�e�[�g�m�F
	if (level_->GetState() != LevelScreenManager::STATE::NONE)
	{
		//�ʏ펞�ȊO�͏������Ȃ�
		return;
	}

	for (int i = 0; i < plNum; i++)
	{
		level_->Reflection(*playerMng_->GetPlayer(i), i);
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