#include"../Archer.h"
#include"../../PlayerInput.h"
#include "PlArcher.h"

PlArcher::PlArcher(const SceneManager::CNTL _cntl)
{
	info_.cntrol_ = _cntl;
}

PlArcher::PlArcher(const InputManager::JOYPAD_NO _padNum)
{
	info_.cntrol_ = SceneManager::CNTL::PAD;
	padNum_ = _padNum;
}

void PlArcher::Init(void)
{
	obj_ = new Archer();
	obj_->Init();
	SetInitPos(playerNum_);
}

void PlArcher::Update(void)
{
	obj_->Update();
	if (!obj_->IsAlive())return;
	//入力
	//キー入力
	PlayerDodge* dodge = obj_->GetDodge();
	PlayerInput::GetInstance().Update(obj_, padNum_, info_.cntrol_);
	ActionInput(obj_, dodge);
	
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
}

void PlArcher::Draw(void)
{
	obj_->Draw();
}

void PlArcher::Release(void)
{
	obj_->Destroy();
	delete obj_;
	obj_ = nullptr;
}

void PlArcher::AtkInput(void)
{
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	//if ((obj_->GetIsAtk() || obj_->GetIsSkill()))return;
	//if (obj_->GetIsAtk()&&obj_->GetIsCool(PlayerBase::ATK_ACT::ATK))return;
	if (ins.CheckAct(ACT_CNTL::NMLATK))
	{
		AtkInit();
	}
}

void PlArcher::SkillOneInput(void)
{
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	int skillOne = static_cast<int>(ATK_ACT::SKILL1);
	//if ((obj_->GetIsAtk() || obj_->GetIsSkill()))return;
	//if (obj_->GetIsCool(PlayerBase::ATK_ACT::ATK))return;
	if (!obj_->GetIsCool(ATK_ACT::SKILL1))
	{
		if (ins.CheckAct(ACT_CNTL::SKILL_DOWN) && !obj_->IsAtkStart())
		{
			SkillOneInit();
		}
		//スキル(長押しでガード状態維持)
		if (ins.CheckAct(ACT_CNTL::SKILL_KEEP) && obj_->IsAtkStart())
		{
			//押している反応
			//CntUp(atkStartCnt_);
		}
		else if (ins.CheckAct(ACT_CNTL::SKILL_UP) && obj_->IsAtkStart())
		{
			if (obj_->GetAtkStartCnt() <= SKILL_ONE_START_NOCHARGE)
			{
				obj_->SetAtkStartTime(SKILL_ONE_START_NOCHARGE, ATK_ACT::SKILL1);
			}
			else
			{
				float atkStartCnt = obj_->GetAtkStartCnt();
				obj_->SetAtkStartTime(atkStartCnt, ATK_ACT::SKILL1);
			}
		}
	}
}

void PlArcher::AtkInit(void)
{
	float deltaTime = 1.0f / 60.0f;
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	//if (obj_->GetIsCool(ATK_ACT::ATK)||obj_->GetIsAtk())return;
	obj_->ChangeAct(ATK_ACT::ATK);
	obj_->SetAtkStartCnt(deltaTime);
	obj_->SetIsAtk(true);
}



void PlArcher::SkillOneInit(void)
{
	float deltaTime = 1.0f / 60.0f;
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	//if (obj_->GetIsCool(ATK_ACT::SKILL1) || obj_->GetIsSkill())return;
	obj_->ChangeAct(ATK_ACT::SKILL1);
	obj_->SetAtkStartCnt(deltaTime);
	obj_->SetIsSkill(true);
}

void PlArcher::SkillTwoInput(void)
{
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	//if ((obj_->GetIsAtk() || obj_->GetIsSkill()))return;
	if (obj_->GetIsCool(PlayerBase::ATK_ACT::SKILL2))return;
	if (ins.CheckAct(ACT_CNTL::SKILL_DOWN))
	{
		SkillTwoInit();
	}
}
void PlArcher::SkillTwoInit(void)
{
	float deltaTime = 1.0f / 60.0f;
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	//if (obj_->GetIsCool(ATK_ACT::SKILL2) || obj_->GetIsSkill())return;
	obj_->ChangeAct(ATK_ACT::SKILL2);
	obj_->SetAtkStartCnt(deltaTime);
	obj_->SetIsSkill(true);
}
