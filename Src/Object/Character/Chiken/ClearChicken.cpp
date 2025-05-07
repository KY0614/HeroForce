#include "ClearChicken.h"

ClearChicken::ClearChicken()
{
}

void ClearChicken::SetParam()
{
	hp_ = LIFE;

	//生存時の行動をランダムで決める
	aliveState_ = ALIVE_MOVE::CALL;

	//初期状態(※生存時の状態を決めてから行う)
	ChangeState(STATE::ALIVE);

	//アニメーション状態
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
}