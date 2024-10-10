#include<DxLib.h>
#include"../Manager/Resource.h"
#include"../Manager/InputManager.h"
#include"../Manager/ResourceManager.h"
#include"../Utility/AsoUtility.h"
#include "Enemy.h"

Enemy::Enemy()
{
}

void Enemy::Destroy(void)
{
}

void Enemy::Init(void)
{
	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_AXEMAN));

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
	if (atkCdt_ < ATK_CDT)state_ = STATE::NML;
	else state_ = STATE::ATK;

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
	Move();
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
	//DrawSphere3D(trans_.pos, 10.0f, 10, 0xff00ff, 0xff00ff, true);

	MV1DrawModel(trans_.modelId);
}

void Enemy::Move(void)
{
	//���͗p
	InputManager& ins = InputManager::GetInstance();

	//�ړ�(�f�o�b�O)
	if (ins.IsNew(KEY_INPUT_UP)) { ProcessMove(5.0f, 0.0f); }
	if (ins.IsNew(KEY_INPUT_RIGHT)){ ProcessMove(5.0f, 90.0f); }
	if (ins.IsNew(KEY_INPUT_DOWN)){ ProcessMove(5.0f, 180.0f); }
	if (ins.IsNew(KEY_INPUT_LEFT)){ ProcessMove(5.0f, 270.0f); }
}

void Enemy::ProcessMove(const float _moveSpeed, const float _deg)
{
	//�����]��
	Turn(_deg, AsoUtility::AXIS_Y);

	//�ړ�����
	VECTOR dir = trans_.quaRot.GetForward();

	//�ړ���
	VECTOR movePow = VScale(dir, _moveSpeed);

	//�ړ�
	trans_.pos = VAdd(trans_.pos, movePow);
}

void Enemy::Turn(float _deg, VECTOR _axis)
{
	//trans_.quaRot = trans_.quaRot.Mult(trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis));
	trans_.quaRot = trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}

void Enemy::Attack(void)
{
	//�U���N�[���_�E���J�E���^������
	atkCdt_ = 0.0f;
}