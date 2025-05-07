#include "ClearPlayers.h"

using S_ROLE = SceneManager::ROLE;

ClearPlayers::ClearPlayers()
{
}

void ClearPlayers::InitAnim()
{	
	//アニメーションの設定
	animNum_.emplace(ANIM::IDLE, IDLE_ANIM);
	ResetAnim(ANIM::IDLE, ANIM_SPEED);
}