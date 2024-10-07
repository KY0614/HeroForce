#include<DxLib.h>
#include"../Manager/Resource.h"
#include"../Manager/InputManager.h"
#include"../Manager/ResourceManager.h"
#include"../Utility/AsoUtility.h"
#include"EnemyAtk.h"
#include "Enemy.h"

Enemy::Enemy()
{
}

void Enemy::Destroy(void)
{
}

void Enemy::Init(void)
{
	//trans_.SetModel()

	//�ϐ��̏�����
	trans_.scl = { 1.0f,1.0f,1.0f };
	trans_.pos = { 0.0f,0.0f,0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion();
	
	state_ = STATE::NML;

	atkCdt_ = 0.0f;
	
	trans_.Update();
}

void Enemy::Update(void)
{
	//�U���N�[���_�E�����Ȃ�ҋ@
	atkCdt_ < ATK_CDT
		? state_ = STATE::NML
		: state_ = STATE::ATK;

	//��Ԃ��Ƃ�Update
	switch (state_)
	{
	case Enemy::STATE::NML:
		UpdateNml();
		break;

	case Enemy::STATE::ATK:
		UpdateAtk();
		break;

	case Enemy::STATE::DEAD:
		UpdateDead();
		break;
	}
}

void Enemy::UpdateNml(void)
{
	//�N�[���_�E���J�E���^
	atkCdt_++;

	//�ړ�����
	Move(5.0f);
}

void Enemy::UpdateAtk(void)
{
	//�U������
	Attack();
}

void Enemy::UpdateDead(void)
{
}

void Enemy::Draw(void)
{
	DrawSphere3D(trans_.pos, 10.0f, 10, 0xff00ff, 0xff00ff, true);
}

std::vector<EnemyAtk*> Enemy::GetAtk(void)
{
	return atk_;
}

void Enemy::Move(const float _moveSpeed)
{
	//���͗p
	InputManager& ins = InputManager::GetInstance();

	//�ړ�����
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	//�ړ�(�f�o�b�O)
	if (ins.IsNew(KEY_INPUT_UP))
	{
		dir = VAdd(dir, trans_.quaRot.GetForward());
	}
	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		dir = VAdd(dir, trans_.quaRot.GetRight());
	}
	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		dir = VAdd(dir, trans_.quaRot.GetBack());
	}
	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		dir = VAdd(dir, trans_.quaRot.GetLeft());
	}

	//�ړ���
	VECTOR movePow = VScale(dir, _moveSpeed);

	//�ړ�
	trans_.pos = VAdd(trans_.pos, movePow);
}

void Enemy::Attack(void)
{
	//�U���̐���
	CreateAtk();

	//�U���N�[���_�E���J�E���^������
	atkCdt_ = 0.0f;
}

void Enemy::CreateAtk(void)
{
	//�L���ȍU�����擾
	EnemyAtk* atk = GetValidAtk();

	//�U���̐���
	atk->Init(this);
}

EnemyAtk* Enemy::GetValidAtk(void)
{
	//�g�p���������z���T��
	for (auto& atk : atk_)
	{
		//��ڂ��I����Ă���Ȃ�
		if (!atk->IsAlive())
		{
			//�ė��p
			return atk;
		}
	}

	//�Ȃ��̂ŐV�����U���p�ɔz���ǉ�
	EnemyAtk* newAtk = new EnemyAtk();
	atk_.emplace_back(newAtk);

	//�V����������̂�Ԃ�
	return newAtk;
}
