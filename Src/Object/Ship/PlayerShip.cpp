#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Manager/InputManager.h"
#include "../../Utility/AsoUtility.h"
#include "PlayerShip.h"

PlayerShip::PlayerShip(void)
{
}

PlayerShip::~PlayerShip(void)
{
}

void PlayerShip::Init(void)
{
	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));
	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();

	transform_.quaRotLocal = Quaternion();

	transform_.Update();
}

void PlayerShip::Update(void)
{
	// ��]����
	ProcessTurn();

	// ���@�̑O���ړ�����

	// �ړ����� = ���W + �ړ���
	// �ړ��ʁ@	= ���� * �X�s�[�h

	// �����i�O�������j
	VECTOR dir = transform_.quaRot.GetForward();
	//VECTOR dir = transform_.GetForward();

	// �ړ���
	VECTOR movePow = VScale(dir, SPEED_MOVE);

	// �ړ�
	transform_.pos = VAdd(transform_.pos, movePow);

	transform_.Update();
}

void PlayerShip::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void PlayerShip::Release(void)
{
}

const Transform& PlayerShip::GetTransform(void) const
{
	return transform_;
}

void PlayerShip::ProcessTurn(void)
{
	auto& ins = InputManager::GetInstance();

	// �\���L�[�������Ď��@����]������
	if (ins.IsNew(KEY_INPUT_UP)) { Turn(SPEED_ROT_DEG_X, AsoUtility::AXIS_X); }
	if(ins.IsNew(KEY_INPUT_DOWN)){ Turn(-SPEED_ROT_DEG_X, AsoUtility::AXIS_X); }
	if(ins.IsNew(KEY_INPUT_RIGHT)){ Turn(SPEED_ROT_DEG_Y, AsoUtility::AXIS_Y); }
	if(ins.IsNew(KEY_INPUT_LEFT)){ Turn(-SPEED_ROT_DEG_Y, AsoUtility::AXIS_Y); }
}

void PlayerShip::Turn(float deg, VECTOR axis)
{
	// �f�O���[���烉�W�A���֕ϊ�
	float rad= AsoUtility::Deg2RadF(deg);

	// ��]��(�I�C���[�p)�Ɖ�]�����g�p���āA�N�H�[�^�j�I����]�����
	Quaternion rotPow = Quaternion::AngleAxis(rad, axis);

	// ���@����](����)������
	transform_.quaRot = transform_.quaRot.Mult(rotPow);
}
