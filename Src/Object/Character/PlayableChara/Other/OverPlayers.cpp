#include "OverPlayers.h"

OverPlayers::OverPlayers()
{
}

void OverPlayers::InitAnim()
{	
	//アニメーションの設定
	animNum_.emplace(ANIM::IDLE, IDLE_ANIM);
	animNum_.emplace(ANIM::DEATH, DEATH_ANIM);
	ResetAnim(ANIM::DEATH, ANIM_SPEED);
}

void OverPlayers::FinishAnim()
{
	switch (anim_)
	{
	case UnitBase::ANIM::IDLE:
		//ループ再生
		stepAnim_ = 0.0f;
		break;

	case UnitBase::ANIM::DEATH:
		ResetAnim(ANIM::IDLE, ANIM_SPEED);
		break;
	}
}
