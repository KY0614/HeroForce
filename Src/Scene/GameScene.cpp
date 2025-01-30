#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/Collision.h"
#include"../Manager/GameSystem/Timer.h"
#include"../Manager/Decoration/SoundManager.h"

#include"../Object/Character/EnemySort/Enemy.h"
#include"../Object/Character/EnemyManager.h"
#include"../Object/Character/PlayerManager.h"
#include"../Object/Character/Chiken/ChickenManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/System/LevelScreenManager.h"
#include "../Object/System/UnitPositionLoad.h"
#include "../Manager/GameSystem/DataBank.h"
#include"../Object/Character/PlayerDodge.h"
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

	//�X�e�[�W
	stage_ = std::make_unique<StageManager>();
	stage_->Init();
	//�X�J�C�h�[��
	sky_ = std::make_unique<SkyDome>();
	sky_->Init();
	//���x���֌W
	level_ = std::make_unique<LevelScreenManager>();
	level_->Init();

	//�v���C���[�ݒ�
	playerMng_ = std::make_unique<PlayerManager>();
	playerMng_->Init();

	//�G�}�l�[�W���̐���
	enmMng_ = std::make_unique<EnemyManager>(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::ENEMY));
	enmMng_->Init();
	//�j���g���̐���
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

	SoundInit();

}

void GameScene::Update(void)
{
	//�Q�[���I�[�o�[����
	if(IsGameOver())SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	if (!playerMng_->GetPlayer(0)->IsAlive())SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);

	//�t�F�[�Y���U���g
	if (isFazeRezult_)
	{
		fazeResult_->Update();

		//���U���g���I�������Ƃ�
		if (fazeResult_->IsEnd())
		{
			//�t�F�[�Y�J�E���g����
			fazeCnt_++;
			//�J�E���g��ŏI�t�F�[�Y�����傫���Ȃ�����N���A�V�[����
			if(fazeCnt_ >LAST_FAZE)SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);

			//�G�̓���ւ�
			enmMng_->ProcessChangePhase(3);

			//�t�F�[�Y���U���g���I�������̂Ŗ��]�y�у��U���g���Z�b�g�E�^�C�}�[�������EBGM�̍Đ�
			fader_->SetFade(Fader::STATE::FADE_IN);
			Timer::GetInstance().Reset();
			fazeResult_->Reset();
			isFazeRezult_ = false;
			if (fazeCnt_ == LAST_FAZE)SoundManager::GetInstance().Play(SoundManager::SOUND::GAME_LAST);
			else SoundManager::GetInstance().Play(SoundManager::SOUND::GAME_NOMAL);
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


	level_->Update();

	if (level_->IsLevelUp())return;

	Timer::GetInstance().Update();
	//�^�C�}�[���I��������
	if (Timer::GetInstance().IsEnd())ChangePhase();



	//�v���C���[�̍X�V
	playerMng_->Update();


	//�G�̍X�V
	enmMng_->Update();
	

	chicken_->SetTargetPos(playerMng_->GetPlayer(0)->GetPos());
	chicken_->Update();

	//�����蔻��
	Collision();

	//�����v�f�̔��f
	LevelUpReflection();



	//���������
	auto& ins = InputManager::GetInstance();
	auto& mng = SceneManager::GetInstance();
	////�X�y�[�X��������^�C�g���ɖ߂�
	//if (ins.IsTrgDown(KEY_INPUT_SPACE))
	//{
	//	mng.ChangeScene(SceneManager::SCENE_ID::TITLE);
	//}

	//if (ins.IsTrgDown(KEY_INPUT_RETURN))
	//{
	//	ChangePhase();
	//}

	if (ins.IsTrgDown(KEY_INPUT_K))
		playerMng_->GetPlayer(0)->SetDamage(20, 20);
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
	stage_->Destroy();

	SceneManager::GetInstance().ResetCameras();
	SceneManager::GetInstance().ReturnSolo();
	Timer::GetInstance().Reset();

	playerMng_->Release();

	enmMng_->Release();
}


void GameScene::SoundInit(void)
{
	//BGM�̏�����
	auto& snd = SoundManager::GetInstance();

	//�ʏ�
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAME_NOMAL,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_NOMAL_BGM).handleId_);
	//�{�X��
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAME_NOMAL,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_LAST_BGM).handleId_);

	//�Q�[���V�[���J�n���̓m�[�}����BGM���Đ�
	snd.Play(SoundManager::SOUND::GAME_NOMAL);

	//���ʉ��ݒ�
	//�v���C���[���S
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::DETH_PLAYER,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_DETH_SND).handleId_);
	//�G���S
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::DETH_ENEMY,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_DETH_SND).handleId_);
	//�j���g�����S
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::DETH_CHICKEN,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::CHICKEN_DETH_SND).handleId_);
	//�U����炢��
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::HIT,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::HIT_SND).handleId_);
}

//�����蔻��i�����ڂɊ�������̂̂݁j
//�����蔻�葍��
void GameScene::Collision(void)
{
	auto& col = Collision::GetInstance();


	CollisionChicken();
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

	enmMng_->CollisionStage(stage_->GetTtans());

	//�����蔻��(��ɍ��G)
	for (int i = 0; i < maxCnt; i++)
	{
		//�G�̎擾
		Enemy* e = enmMng_->GetActiveEnemy(i);

		//�G�l�̈ʒu�ƍU�����擾
		VECTOR ePos = e->GetPos();
		
		//�G�l�̍��G����
		bool isPlayerFound = false;

		for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
		{
			PlayerBase* p = playerMng_->GetPlayer(i);
			VECTOR pPos = p->GetPos();

			//�͈͓��ɓ����Ă���Ƃ�

			//�ʏ��Ԏ� && �U���͈͓��Ƀv���C���[����������U�����J�n
			if (col.Search(ePos, pPos, e->GetAtkStartRange()) && e->GetState() == Enemy::STATE::NORMAL) {
				//��Ԃ�ύX
				e->ChangeState(Enemy::STATE::ALERT);
			}

			if (col.Search(ePos, pPos, e->GetSearchRange())) {
				//�v���C���[��_��
				e->ChangeSearchState(Enemy::SEARCH_STATE::PLAYER_FOUND);
				e->SetTargetPos(pPos);

				//������
				isPlayerFound = true;
			}
			else if (!isPlayerFound && e->GetSearchState() != Enemy::SEARCH_STATE::CHICKEN_FOUND) {
				//�N���_���Ă��Ȃ�
				e->ChangeSearchState(Enemy::SEARCH_STATE::CHICKEN_SEARCH);
			}

			//�U������
			for (int a = 0 ; a < e->GetAtks().size() ; a++)
			{
				//�U�������Z�b�g
				e->SetAtk(e->GetAtks()[a]);

				//�Z�b�g���Ă��������Ƃ��Ă���
				UnitBase::ATK eAtk = e->GetAtk();

				//�A�^�b�N�� && �U�����肪�I�����Ă��Ȃ��Ƃ�������������B����ȊO�͂��Ȃ��̂Ŗ߂�
				if (eAtk.IsAttack() && !eAtk.isHit_) {


					//�U����������͈� && �v���C���[��������Ă��Ȃ��Ƃ�
					if (col.IsHitAtk(*e, *p)/* && !p->GetDodge()->IsDodge()*/)
					{
						//�_���[�W
						p->SetDamage(e->GetCharaPow(), eAtk.pow_);
						//�g�p�����U���𔻒�I����
						e->SetAtksIsHit(a,true);
					}
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
				e->Damage(e->GetDamage(), pAtk.pow_);

				//���񂾂�o���l����
				if (!e->IsAlive())level_->AddExp(e->GetExp());

				//�|�����G�̑���
				if (!e->IsAlive())dunkEnmCnt_++;
				//�U������̏I��
				p->SetIsHit(true);
			}
		}
		
	}
}

void GameScene::CollisionChicken(void)
{
	auto& col = Collision::GetInstance();

	int chickenNum = chicken_->GetChickenAllNum();

	for (int i = 0; i < chickenNum; i++) {
		auto c = chicken_->GetChicken(i);
		//�j���g��������ł����玟��
 		if (!c->IsAlive())continue;

		//�G�̑����擾
		int maxCnt = enmMng_->GetActiveNum();
		//�U������

		for (int r = 0; r < maxCnt; r++)
		{

			//�G�̎擾
			Enemy* e = enmMng_->GetActiveEnemy(r);

			//�G�l�̈ʒu�ƍU�����擾
			VECTOR ePos = e->GetPos();

			//���G
			//�͈͓��ɓ����Ă���Ƃ�
			if (col.Search(ePos, c->GetPos(), e->GetSearchRange())) {
				//�ړ����J�n
	
				//�{��_��
				e->ChangeSearchState(Enemy::SEARCH_STATE::CHICKEN_FOUND);
				e->SetTargetPos(c->GetPos());
			}
			else if(e->GetSearchState() != Enemy::SEARCH_STATE::PLAYER_FOUND) {
				//�ړ����J�n

				//�܂��T����
				e->ChangeSearchState(Enemy::SEARCH_STATE::CHICKEN_SEARCH);
				e->SetPreTargetPos(c->GetPos());
			}

			//�ʏ��Ԏ� && �U���͈͓��Ƀv���C���[����������U�����J�n
			if (col.Search(ePos, c->GetPos(), e->GetAtkStartRange()) && e->GetState() == Enemy::STATE::NORMAL) {
				//��Ԃ�ύX
				e->ChangeState(Enemy::STATE::ALERT);
			}

			//�U������
			for (int a = 0; a < e->GetAtks().size(); a++)
			{
				//�U�������Z�b�g
				e->SetAtk(e->GetAtks()[a]);

				//�Z�b�g���Ă��������Ƃ��Ă���
				UnitBase::ATK eAtk = e->GetAtk();

				//�A�^�b�N�� && �U�����肪�I�����Ă��Ȃ��Ƃ�������������B����ȊO�͂��Ȃ��̂Ŗ߂�
				if (!(eAtk.IsAttack() && !eAtk.isHit_))continue;

				//�U����������͈� && �v���C���[��������Ă��Ȃ��Ƃ�
				if (col.IsHitAtk(*e, *c))
				{
					//�_���[�W
					c->SetDamage(e->GetCharaPow());
					//�g�p�����U���𔻒�I����
					e->SetAtksIsHit(a,true);
				}
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
	//BGM�̒�~
	if (fazeCnt_ == LAST_FAZE)SoundManager::GetInstance().Stop(SoundManager::SOUND::GAME_LAST);
	else SoundManager::GetInstance().Stop(SoundManager::SOUND::GAME_NOMAL);

	//���U���g�Ɋ֌W����f�[�^�����
	DataBank& data = DataBank::GetInstance();

	data.Input(DataBank::INFO::FAZE_DUNK_ENEMY, dunkEnmCnt_);	//�|�����G��
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