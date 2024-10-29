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
//UŒ‚‚ª“–‚½‚Á‚½‚©
 // ˆø”‚ÍiUŒ‚‚µ‚½lAUŒ‚‘ÎÛj‚Ì‡
const bool Collision::IsHitAtk(const UnitBase* _chaser, const UnitBase* _target)
{
    return AsoUtility::IsHitSpheres(_chaser->GetAtk().pos_, _chaser->GetAtk().radius_, _target->GetPos(), _target->GetRadius());
}

const bool Collision::Search(const UnitBase* _chaser, const UnitBase* _target)
{
    return false;
}
