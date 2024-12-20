#include"../Utility/AsoUtility.h"
#include"../Object/UnitBase.h"
#include"../Object/Character/PlayerBase.h"
#include"../Object/Character/Enemy.h"
#include "Collision.h"

Collision* Collision::instance_ = nullptr;


void Collision::CreateInstance(void)
{
    if (instance_ == nullptr)
    {
        instance_ = new Collision();
    }
}

Collision& Collision::GetInstance(void)
{
    return *instance_;
}
void Collision::Init(void)
{
   
}
//攻撃が当たったか
 // 引数は（攻撃した人、攻撃対象）の順
const bool Collision::IsHitAtk(const UnitBase& _chaser, const UnitBase& _target)
{
    return AsoUtility::IsHitSpheres(_chaser.GetAtk().pos_, _chaser.GetAtk().radius_, _target.GetPos(), _target.GetRadius());
}

const bool Collision::Search(const UnitBase& _chaser, const UnitBase& _target)
{
    return false;
}

const bool Collision::Search(const VECTOR _myPos, const VECTOR _targetPos, const float _rangeRadius) const
{
    //標的への方向ベクトルを取得
    VECTOR targetVec = VSub(_targetPos, _myPos);

    //大きさを求める
    float vecSize = hypot(targetVec.x, targetVec.z);
    //判定
    return _rangeRadius - vecSize > 0;
}
