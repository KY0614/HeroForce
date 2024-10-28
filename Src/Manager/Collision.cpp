#include"../Object/UnitBase.h"
#include"../Object/Character/PlayerBase.h"
#include"../Object/Character/Enemy.h"
#include "Collision.h"

const bool Collision::IsHitAtk(const PlayerBase& _player, const Enemy& _enemy)
{
    return false;
}

const bool Collision::IsHitAtk(const Enemy& _enemy, const PlayerBase& _player)
{
    return false;
}

const bool Collision::Search(const UnitBase& _chaser, const UnitBase& _target)
{
    return false;
}
