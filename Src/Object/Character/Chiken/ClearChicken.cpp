#include "ClearChicken.h"

ClearChicken::ClearChicken()
{
	//// ��ԊǗ�
	//stateChanges_.emplace(STATE::NONE, std::bind(&ChickenBase::ChangeStateNone, this));
	//stateChanges_.emplace(STATE::ALIVE, std::bind(&ChickenBase::ChangeStateAlive, this));
	//stateChanges_.emplace(STATE::DAMAGE, std::bind(&ChickenBase::ChangeStateDamage, this));
	//stateChanges_.emplace(STATE::DEATH, std::bind(&ChickenBase::ChangeStateDeath, this));
	//stateChanges_.emplace(STATE::END, std::bind(&ChickenBase::ChangeStateEnd, this));

	//// ������ԊǗ�
	//aliveChanges_.emplace(ALIVE_MOVE::IDLE, std::bind(&ChickenBase::ChangeAliveIdle, this));
	//aliveChanges_.emplace(ALIVE_MOVE::ROTATION, std::bind(&ChickenBase::ChangeAliveWalk, this));
	//aliveChanges_.emplace(ALIVE_MOVE::CALL, std::bind(&ChickenBase::ChangeAliveCall, this));
}

ClearChicken::~ClearChicken()
{
}

void ClearChicken::SetParam()
{
	hp_ = DEFAULT_LIFE;

	//�������̍s���������_���Ō��߂�
	aliveState_ = ALIVE_MOVE::CALL;

	//�������(���������̏�Ԃ����߂Ă���s��)
	ChangeState(STATE::ALIVE);

	//�A�j���[�V�������
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
}