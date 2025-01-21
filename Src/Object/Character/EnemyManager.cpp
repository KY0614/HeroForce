#include<cassert>

#include"EnemySort/Enemy.h"
#include"EnemySort/EneArcher.h"
#include"EnemySort/EneAxe.h"
#include"EnemySort/EneBrig.h"
#include"EnemySort/EneGolem.h"
#include"EnemySort/EneMage.h"

#include "EnemyManager.h"

void EnemyManager::Init(void)
{
	activeNum_ = 0;
	for (int i = 0; i < 4; i++)
	{
		CreateEnemy();
	}
}

void EnemyManager::Update(VECTOR _target)
{
	for (int i = 0; i < activeNum_; i++)
	{
		activeEnemys_[i]->SetTargetPos(_target);
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



void EnemyManager::CreateEnemy(void)
{
	//�G���ő吔�����琶���������s��Ȃ�
	if (activeNum_ >= ENEMY_MAX)return;

	Enemy* enm = nullptr;

	//�����Ŏ�ތ��߂�
	TYPE type = static_cast<TYPE>(GetRand(static_cast<int>(TYPE::MAX)-1));

	//�C���X�^���X����
	switch (type)
	{
	case EnemyManager::TYPE::ARCHER:
		enm = new EneArcher();
		break;
	case EnemyManager::TYPE::AXE:
		enm = new EneAxe();
		break;
	case EnemyManager::TYPE::BRIG:
		enm = new EneBrig();
		break;
	case EnemyManager::TYPE::GOLEM:
		//�S�[�����̓{�X�L�����Ȃ̂ł����ł͐������Ȃ�
		return;
		break;
	case EnemyManager::TYPE::MAGE:
		enm = new EneMage();
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
	activeEnemys_[activeNum_] = enm;

	//�J�E���^����
	activeNum_++;
}

void EnemyManager::DethEnemy(int _num)
{
	//�|���ꂽ�G�̏���
	activeEnemys_[_num]->Destroy();
	delete activeEnemys_[_num];

	//�����̌���
	//���̎��_��activeNum_�͔z��̖����̔ԍ��������悤�ɂȂ�B
	activeNum_--;

	//�z��̋󂫂𖄂߂邽�߂̃\�[�g
	//�z��̖����̕����󂫂Ɉړ�������
	//�|���ꂽ�G�������̕��Ȃ珈���͂��Ȃ�
	if (_num == activeNum_)return;

	//�}������
	//std::move�ł����������H������Ղ��悤�ɉ��L�̂悤�ɂ���
	activeEnemys_[_num] = activeEnemys_[activeNum_];

	//�����̏���
	activeEnemys_[activeNum_]->Destroy();
	delete activeEnemys_[activeNum_];
}
