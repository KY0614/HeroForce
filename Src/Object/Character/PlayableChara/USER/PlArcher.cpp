#include"../Archer.h"
#include"../../PlayerInput.h"
#include "PlArcher.h"

PlArcher::PlArcher(const SceneManager::CNTL _cntl)
{
	info_.cntrol_ = _cntl;
}

PlArcher::PlArcher(const InputManager::JOYPAD_NO _padNum)
{
	padNum_ = _padNum;
}

void PlArcher::Init(void)
{
	obj_ = new Archer();
	obj_->Init();
}

void PlArcher::Update(void)
{
	obj_->Update();
	//入力
	//PlayerInput::GetInstance().Update(obj_, padNum_, info_.cntrol_);
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	
	//通常攻撃
	AtkInput();


	//現在のスキル番号(skillNo_)によって更新を変える
	PlayerBase::ATK_ACT skillNo = obj_->GetSkillNo();
	switch (skillNo)
	{
	case PlayerBase::ATK_ACT::SKILL1:
		SkillOneInput();
		break;
	case PlayerBase::ATK_ACT::SKILL2:
		SkillTwoInput();
		break;
	}


	//---------------------------------------------------------
	//スキル1
	//---------------------------------------------------------




	//---------------------------------------------------------
	//スキル2
	//---------------------------------------------------------





	//---------------------------------------------------------
}

void PlArcher::Draw(void)
{
	obj_->Draw();
}

void PlArcher::Release(void)
{
}

void PlArcher::AtkInput(void)
{
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	if (ins.CheckAct(ACT_CNTL::NMLATK))
	{
		AtkInit();
	}
}

void PlArcher::SkillOneInput(void)
{
}

void PlArcher::SkillTwoInput(void)
{
}

void PlArcher::AtkInit(void)
{
	float deltaTime = 1.0f / 60.0f;
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	if (obj_->GetIsCool(ATK_ACT::ATK))return;
	obj_->ChangeAct(ATK_ACT::ATK);
	obj_->SetAtkStartCnt(deltaTime);
	obj_->SetIsAtk(true);
}
