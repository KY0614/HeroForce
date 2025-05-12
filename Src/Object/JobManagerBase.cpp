#include "./Character/PlayerDodge.h"
#include "JobManagerBase.h"

JobManagerBase::JobManagerBase(void)
{
}

JobManagerBase::~JobManagerBase(void)
{
}

<<<<<<< HEAD
void JobManagerBase::ActionInput( PlayerBase* _player, PlayerDodge* _dodge)
=======
void JobManagerBase::SetInitPos(int _num)
{
	VECTOR pos = obj_->GetInitPos(_num);
	obj_->SetPos(pos);
}

void JobManagerBase::ActionInput(PlayerBase* _player, PlayerDodge* _dodge)
>>>>>>> Data2
{
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	//設定された向きに向かって動く
<<<<<<< HEAD
	if (ins.CheckAct(ACT_CNTL::MOVE)) 
	{ 
		_player->Move(ins.GetMoveDeg(), AsoUtility::AXIS_Y);
	}
	//動いてないときはスピード0にする
	else { obj_->SetMoveSpeed(0.0f); }

	if (ins.CheckAct(ACT_CNTL::CHANGE_SKILL)) { _player->SkillChange(); }
=======
	if (ins.CheckAct(ACT_CNTL::MOVE))
	{
		_player->Move(ins.GetMoveDeg(), AsoUtility::AXIS_Y);
	}
	//動いてないときはスピード0にする
	else { obj_->SetSpeed(0.0f); }

	if (ins.CheckAct(ACT_CNTL::CHANGE_SKILL))
	{
		_player->SkillChange();
	}

>>>>>>> Data2



	//回避
<<<<<<< HEAD
	if (ins.CheckAct(ACT_CNTL::DODGE) && _player->IsSkillable())
=======
	if (ins.CheckAct(ACT_CNTL::DODGE)&&!_dodge->IsCoolDodge())
>>>>>>> Data2
	{
		float dodgeCnt = _dodge->GetDodgeCnt();
		float deltaTime = 1.0f / 60.0f;
		_player->ResetAnim(PlayerBase::ANIM::DODGE, PlayerBase::SPEED_ANIM_DODGE);
		dodgeCnt += deltaTime;
		_dodge->SetDodgeCnt(dodgeCnt);
	}
<<<<<<< HEAD
}
=======
}
>>>>>>> Data2
