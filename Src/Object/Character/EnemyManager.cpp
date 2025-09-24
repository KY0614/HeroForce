#include<cassert>
#include<random>
#include"../Manager/Generic/SceneManager.h"
#include"EnemySort/Enemy.h"
#include"EnemySort/EneArcher.h"
#include"EnemySort/EneAxe.h"
#include"EnemySort/EneBrig.h"
#include"EnemySort/EneGolem.h"
#include"EnemySort/EneMage.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager(std::vector<VECTOR> _pos) : createPos_(_pos)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		activeEnemys_[i] = nullptr;
		activeEnemysType_[i] = TYPE::MAX;
	}

	for (int a = 0; a < static_cast<int>(TYPE::MAX); a++)
	{
		activeTypeNum_[a] = 0;
	}

	allExp_ = 0.0f;
	activeNum_ = 0;
	createIntCnt_ = 0.0f;
	dunkCnt_ = 0;
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init(void)
{
	for (int a = 0; a < static_cast<int>(TYPE::MAX); a++)
	{
		activeTypeNum_[a] = 0;
	}

	dunkCnt_ = 0;

	allExp_ = 0.0f;

	activeNum_ = 0;

	createIntCnt_ = 0.0f;

	ProcessChangePhase(PHASE_ONE);
}

void EnemyManager::Update(void)
{
	//�J�E���^
	createIntCnt_ = createIntCnt_ + SceneManager::GetInstance().GetDeltaTime();

	//�G�̐�������
	if (createIntCnt_ >= CREATE_INTERVAL)
	{
		//�Ԋu�J�E���^������
		createIntCnt_ = 0.0f;

		//�G�̐���
		CreateEnemy();
	}

	//�������Ă���G�̏���
	for (int i = 0; i < activeNum_; i++)
	{
		//���S����
		if (!activeEnemys_[i]->IsAlive() 
			&& activeEnemys_[i]->IsFinishAnim(Enemy::ANIM::DEATH))
		{

			//�G�폜
			DeathEnemy(i);
			break;
		}

		//activeEnemys_[i]->SetTargetPos(_target);
		activeEnemys_[i]->Update();
	}
}

void EnemyManager::Draw(void)
{
	for (int i = 0; i < activeNum_; i++)activeEnemys_[i]->Draw();
}

void EnemyManager::Release(void)
{
	
}

void EnemyManager::CollisionStage(const Transform& stageTrans)
{
	auto& col = Collision::GetInstance();

	for (int i = 0; i < activeNum_; i++)
	{
		if (col.IsHitUnitStageObject(stageTrans.modelId, activeEnemys_[i]->GetTransform().pos, activeEnemys_[i]->GetRadius()))
		{
			activeEnemys_[i]->SetPrePos();

			//�ړ����
			activeEnemys_[i]->KeepCollStageDistance();
		}
	}
}



void EnemyManager::CreateEnemy(void)
{
	//�G���ő吔�����琶���������s��Ȃ�
	if (activeNum_ >= ENEMY_MAX)return;

	//�G�̐����p
	std::unique_ptr<Enemy> enm;

	//���[�v����
	int retry = 0;

	//�����Ŏ�ތ��߂�
	int typeRand = GetRand(static_cast<int>(TYPE::MAX) - 1);
	
	//�S�[�����@���́@������ނ̓G�������Ɉ������������������x
	while (typeRand == static_cast<int>(TYPE::GOLEM)|| activeTypeNum_[typeRand] >= ONETYPE_MAX && retry < RETRY_LIMIT)
	{
		//�����_���œG�����߂�
		typeRand = GetRand(static_cast<int>(TYPE::MAX) - 1);
		
		//���g���C��
		retry++;
	} 

	//TYPE�ɕϊ�
	TYPE type = static_cast<TYPE>(typeRand);

	//���[�v�����𒴂��Ă����珈������߂�
	if (retry >= RETRY_LIMIT)return;

	//�����n�_�������_���Ō��߂�
	int createPosPoint = GetRand(createPos_.size() - 1);
	//�������΍��W
	VECTOR createLocalPos = createPos_[createPosPoint];
	//�G�̐������W�����炷
	createLocalPos.x += activeNum_ * Enemy::COL_RADIUS;
	createLocalPos.y = 0.0f;
	createLocalPos.z += activeNum_ * Enemy::COL_RADIUS;

	//�C���X�^���X����
	switch (type)
	{
	case EnemyManager::TYPE::ARCHER:
		enm = std::make_unique<EneArcher>(createLocalPos);
		break;
	case EnemyManager::TYPE::AXE:
		enm = std::make_unique <EneAxe>(createLocalPos);
		break;
	case EnemyManager::TYPE::BRIG:
		enm = std::make_unique <EneBrig>(createLocalPos);
		break;
	case EnemyManager::TYPE::GOLEM:
		//�S�[�����̓{�X�L�����Ȃ̂ł����ł͐�������Ȃ�
		return;
	case EnemyManager::TYPE::MAGE:
		enm = std::make_unique <EneMage>(createLocalPos);
		break;
	default:
		return;
		break;
	}

	//�O�̂��߂̃G���[���p
	if (enm == nullptr)assert("�G�̐����Ŗ�肪����܂����B");

	//�G�̏�����
	enm->Init();

	//�G�̍X�V�����|�������Z�b�g
	activeEnemys_[activeNum_] = std::move(enm);

	//�G�̎�ނ�ۑ�
	activeEnemysType_[activeNum_] = type;

	//�J�E���^����
	int activeEnmType = static_cast<int>(activeEnemysType_[activeNum_]);
	activeTypeNum_[activeEnmType]++;
	activeNum_++;
}

void EnemyManager::CreateBoss(void)
{
	//�G���ő吔�����琶���������s��Ȃ�
	if (activeNum_ >= ENEMY_MAX)return;

	//�G�̐���
	std::unique_ptr<Enemy> enm;

	//�������΍��W
	VECTOR createLocalPos = createPos_[GOLEM_SPAWN];

	//�S�[�����̐���
	enm = std::make_unique<EneGolem>(createLocalPos);

	//�O�̂��߂̃G���[���p
	if (enm == nullptr)assert("�G�̐����Ŗ�肪����܂����B");

	//�G�̏�����
	enm->Init();

	//�G�̍X�V�����|�������Z�b�g
	activeEnemys_[activeNum_] = std::move(enm);

	//�G�̎�ނ�ۑ�
	activeEnemysType_[activeNum_] = TYPE::GOLEM;

	//�J�E���^����
	int activeEnmType = static_cast<int>(activeEnemysType_[activeNum_]);
	activeTypeNum_[activeEnmType]++;
	activeNum_++;
}

void EnemyManager::DeleteAllEnemy(void)
{
	for (int i = activeNum_ - 1; i >= 0;i--) {
		if (activeEnemys_[i] == nullptr || activeEnemysType_[i] == TYPE::MAX)
			continue;

		//�G�����ׂč폜
		activeEnemys_[i]->Destroy();
		activeEnemys_[i].reset();
		activeTypeNum_[static_cast<int>(activeEnemysType_[i])]--;
		activeEnemysType_[i] = TYPE::MAX;
		activeNum_--;
	}
}

void EnemyManager::DeathEnemy(int _num)
{
	//�|���ꂽ�G�̌o���l��ۑ�
	allExp_ += activeEnemys_[_num]->GetExp();
	dunkCnt_++;

	//�|���ꂽ�G�̏���
	activeEnemys_[_num]->Destroy();
	activeEnemys_[_num].reset();

	//�����̌���
	//���̎��_��activeNum_�͔z��̖����̔ԍ��������悤�ɂȂ�B
	activeNum_--;
	activeTypeNum_[static_cast<int>(activeEnemysType_[_num])]--;

	//�L���ȓG�̎�ނ̏�����
	activeEnemysType_[_num] = TYPE::MAX;

	//�z��̋󂫂𖄂߂邽�߂̃\�[�g
	//�z��̖����̕����󂫂Ɉړ�������
	//�|���ꂽ�G�������̕��Ȃ珈���͂��Ȃ�
	if (_num == activeNum_)return;

	//�}������
	//�����z������������z��Ɉڂ�
	activeEnemys_[_num] = std::move(activeEnemys_[activeNum_]);
	activeEnemysType_[_num] = activeEnemysType_[activeNum_];
}

void EnemyManager::ProcessChangePhase(const int _phase)
{
	//�G�̍폜
	DeleteAllEnemy();

	//�|�����G�̐�������
	dunkCnt_ = 0;

	//�G�̏����쐬��
	int createNum = 0;

	//�t�F�[�Y�ɂ���č쐬����G�̐���ς���
	switch (_phase)
	{
	case PHASE_ONE:
		//�t�F�[�Y1�̎�
		createNum = PHASE_ONE_INIT_CREATE_ENEMY;
		break;

	case PHASE_TWO:
		//�t�F�[�Y2�̎�
		createNum = PHASE_TWO_INIT_CREATE_ENEMY;
		break;

	case PHASE_LAST:
		//�{�X�̐���
		CreateBoss();
		return;

	default:
		return;
	}

	for (int i = 0; i < createNum; i++)
	{
		//�G�̏�������
		CreateEnemy();
	}
 }