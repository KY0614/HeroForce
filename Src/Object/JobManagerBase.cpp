#include "./Character/PlayerDodge.h"
#include "JobManagerBase.h"

JobManagerBase::JobManagerBase(void)
{
}

JobManagerBase::~JobManagerBase(void)
{
}

void JobManagerBase::SetInitPos(int _num)
{
	VECTOR pos = obj_->GetInitPos(_num);
	obj_->SetPos(pos);
}

void JobManagerBase::ActionInput(PlayerBase* _player, PlayerDodge* _dodge, std::weak_ptr<PlayerInput> _input)
{
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	//�ݒ肳�ꂽ�����Ɍ������ē���
	if (_input.lock()->CheckAct(ACT_CNTL::MOVE))
	{
		_player->Move(_input.lock()->GetMoveDeg(), AsoUtility::AXIS_Y);
	}
	//�����ĂȂ��Ƃ��̓X�s�[�h0�ɂ���
	else { obj_->SetSpeed(0.0f); }

	if (_input.lock()->CheckAct(ACT_CNTL::CHANGE_SKILL))
	{
		_player->SkillChange();
	}

	//���
	if (_input.lock()->CheckAct(ACT_CNTL::DODGE)&&!_dodge->IsCoolDodge())
	{
		float dodgeCnt = _dodge->GetDodgeCnt();
		float deltaTime = 1.0f / 60.0f;
		_player->ResetAnim(PlayerBase::ANIM::DODGE, PlayerBase::SPEED_ANIM_DODGE);
		dodgeCnt += deltaTime;
		_dodge->SetDodgeCnt(dodgeCnt);
	}
}