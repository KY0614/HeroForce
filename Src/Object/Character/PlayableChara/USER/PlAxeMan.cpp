#include"../AxeMan.h"
#include "PlAxeMan.h"


PlAxe::PlAxe(const SceneManager::CNTL _cntl)
{
}

PlAxe::PlAxe(const InputManager::JOYPAD_NO _padNum)
{
}

void PlAxe::Init(void)
{
	obj_ = new AxeMan();
	obj_->Init();
}

void PlAxe::Update(void)
{
	obj_->Update();
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	//�ʏ�U��
	//---------------------------------------------------------
	if (ins.CheckAct(ACT_CNTL::NMLATK) && !obj_->GetIsAtk())
	{
		if (obj_->GetIsCool(ATK_ACT::ATK))return;
		obj_->ChangeAct(ATK_ACT::ATK);
		obj_->ResetParam();
		obj_->SetAtkStartCnt(deltaTime);
		obj_->SetIsAtk(true);
	}


	////---------------------------------------------------------
	////�X�L��1
	////---------------------------------------------------------
	//auto& ins = PlayerInput::GetInstance();
	//using ACT_CNTL = PlayerInput::ACT_CNTL;
	//int skillOne = static_cast<int>(ATK_ACT::SKILL1);
	//if (!isCool_[static_cast<int>(ATK_ACT::SKILL1)])
	//{
	//	if (ins.CheckAct(ACT_CNTL::SKILL_DOWN) && !IsAtkStart())
	//	{
	//		InitSkill(skillNo_);
	//	}
	//	//�X�L��(�������ŃK�[�h��Ԉێ�)
	//	if (ins.CheckAct(ACT_CNTL::SKILL_KEEP) && IsAtkStart())
	//	{
	//		//�����Ă��锽��
	//		//CntUp(atkStartCnt_);
	//	}
	//	else if (ins.CheckAct(ACT_CNTL::SKILL_UP) && IsAtkStart())
	//	{
	//		if (atkStartCnt_ <= SKILL_ONE_START_NOCHARGE)
	//		{
	//			atkStartTime_[skillOne] = SKILL_ONE_START_NOCHARGE;
	//		}
	//		else
	//		{
	//			atkStartTime_[skillOne] = atkStartCnt_;
	//		}
	//	}
	//}



	////---------------------------------------------------------
	////�X�L��2
	////---------------------------------------------------------
	//	//����
	//auto& ins = PlayerInput::GetInstance();
	//using ACT_CNTL = PlayerInput::ACT_CNTL;
	//int skillOne = static_cast<int>(ATK_ACT::SKILL1);
	//if (ins.CheckAct(ACT_CNTL::SKILL_DOWN))
	//{
	//	InitSkill(skillNo_);
	//}




	//---------------------------------------------------------




}

void PlAxe::Draw(void)
{
	obj_->Draw();
}

void PlAxe::Release(void)
{
	
}
