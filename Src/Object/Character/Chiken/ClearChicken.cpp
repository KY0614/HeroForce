#include "ClearChicken.h"

ClearChicken::ClearChicken()
{
	//// 状態管理
	//stateChanges_.emplace(STATE::NONE, std::bind(&ChickenBase::ChangeStateNone, this));
	//stateChanges_.emplace(STATE::ALIVE, std::bind(&ChickenBase::ChangeStateAlive, this));
	//stateChanges_.emplace(STATE::DAMAGE, std::bind(&ChickenBase::ChangeStateDamage, this));
	//stateChanges_.emplace(STATE::DEATH, std::bind(&ChickenBase::ChangeStateDeath, this));
	//stateChanges_.emplace(STATE::END, std::bind(&ChickenBase::ChangeStateEnd, this));

	//// 生存状態管理
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

	//生存時の行動をランダムで決める
	aliveState_ = ALIVE_MOVE::CALL;

	//初期状態(※生存時の状態を決めてから行う)
	ChangeState(STATE::ALIVE);

	//アニメーション状態
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
}