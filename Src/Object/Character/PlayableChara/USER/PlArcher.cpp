#include"../Archer.h"
#include"../../PlayerInput.h"
#include "PlArcher.h"

PlArcher::PlArcher(const SceneManager::CNTL _cntl)
{
	info_.cntrol_ = _cntl;
	input_ = std::make_shared<PlayerInput>();
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
	input_->Update(obj_, padNum_, info_.cntrol_);
	ActionInput(obj_, dodge,input_);
	
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
	//auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	if (input_->CheckAct(ACT_CNTL::NMLATK))
	{
		AtkInit();
	}
}

void PlArcher::SkillOneInput(void)
{
	//auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	int skillOne = static_cast<int>(ATK_ACT::SKILL1);
	if (!obj_->GetIsCool(ATK_ACT::SKILL1))
	{
		if (input_->CheckAct(ACT_CNTL::SKILL_DOWN) && !obj_->IsAtkStart())
		{
			SkillOneInit();
		}
		//スキル(長押しでガード状態維持)
		if (input_->CheckAct(ACT_CNTL::SKILL_KEEP) && obj_->IsAtkStart())
		{
			//押している反応
			//CntUp(atkStartCnt_);
		}
		else if (input_->CheckAct(ACT_CNTL::SKILL_UP) && obj_->IsAtkStart())
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
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	obj_->ChangeAct(ATK_ACT::ATK);
	obj_->SetAtkStartCnt(deltaTime);
	obj_->SetIsAtk(true);
}



void PlArcher::SkillOneInit(void)
{
	float deltaTime = 1.0f / 60.0f;
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	obj_->ChangeAct(ATK_ACT::SKILL1);
	obj_->SetAtkStartCnt(deltaTime);
	obj_->SetIsSkill(true);
}

void PlArcher::SkillTwoInput(void)
{
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	if (obj_->GetIsCool(PlayerBase::ATK_ACT::SKILL2))return;
	if (input_->CheckAct(ACT_CNTL::SKILL_DOWN))
	{
		SkillTwoInit();
	}
}
void PlArcher::SkillTwoInit(void)
{
	float deltaTime = 1.0f / 60.0f;
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	obj_->ChangeAct(ATK_ACT::SKILL2);
	obj_->SetAtkStartCnt(deltaTime);
	obj_->SetIsSkill(true);
}
