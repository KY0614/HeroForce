#include"../Knight.h"
#include "PlKnight.h"

PlKnight::PlKnight(const SceneManager::CNTL _cntl)
{
	info_.cntrol_ = _cntl;
}

PlKnight::PlKnight(const InputManager::JOYPAD_NO _padNum)
{
	info_.cntrol_ = SceneManager::CNTL::PAD;
	padNum_ = _padNum;
}

void PlKnight::Init(void)
{
	obj_ = new Knight();
	obj_->Init();
	SetInitPos(playerNum_);
}

void PlKnight::Update(void)
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






void PlKnight::Draw(void)
{
	obj_->Draw();
}

void PlKnight::Release(void)
{
	obj_->Destroy();
	delete obj_;
	obj_ = nullptr;
}

void PlKnight::AtkInput(void)
{
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	if(obj_->GetIsCool(PlayerBase::ATK_ACT::ATK))return;
	if (ins.CheckAct(ACT_CNTL::NMLATK) && !obj_->GetIsAtk()&&!obj_->GetIsSkill())
	{
		float deltaTime = 1.0f / 60.0f;
		if (obj_->GetIsCool(ATK_ACT::ATK))return;
		obj_->ChangeAct(ATK_ACT::ATK);
		obj_->ResetParam();
		obj_->SetAtkStartCnt(deltaTime);
		obj_->SetIsAtk(true);
		obj_->SetIsSkill(false);
	}
}

void PlKnight::SkillOneInput(void)
{
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	if (obj_->GetIsCool(PlayerBase::ATK_ACT::SKILL1))return;
	if (ins.CheckAct(ACT_CNTL::SKILL_DOWN))
	{
		SkillOneInit();
	}
}

void PlKnight::SkillTwoInput(void)
{
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	if (ins.CheckAct(ACT_CNTL::SKILL_DOWN)
		&&obj_->GetCoolTime(ATK_ACT::SKILL2)>= Knight::SKILL_TWO_START_COOLTIME)
	{
		//ボタンの押しはじめの時に値初期化
		SkillTwoInit();
	}
	//スキル(長押しでガード状態維持)
	if (ins.CheckAct(ACT_CNTL::SKILL_KEEP) && obj_->GetIsSkill())
	{
		if (obj_->GetCoolTime(ATK_ACT::SKILL2) > 0.0f)
		{
			obj_->SetMoveAble(false);
			obj_->SetIsCool(false, ATK_ACT::SKILL2);
			
			if (obj_->GetStepAnim() >= GUARD_ANIM_STEP)
			{
				obj_->SetStepAnim(GUARD_ANIM_STEP);
			}
		}
	}
	else if (ins.CheckAct(ACT_CNTL::SKILL_UP) && obj_->GetIsSkill())
	{
		obj_->SetIsCool(true, ATK_ACT::SKILL2);
		obj_->SetIsSkill(false);
		obj_->InitAtk();
	}
}
void PlKnight::SkillOneInit(void)
{
	//スキルごとにアニメーションを決めて、カウント開始
	float deltaTime = 1.0f / 60.0f;
	using ATK_ACT = PlayerBase::ATK_ACT;
	if (obj_->GetIsAtk() || obj_->GetIsSkill() || obj_->GetIsCool(ATK_ACT::SKILL1))return;
	obj_->ChangeAct(ATK_ACT::SKILL1);
	obj_->ResetParam();
	obj_->SetAtkStartCnt(deltaTime);
	obj_->SetMoveAble(false);
	obj_->SetIsAtk(false);
	obj_->SetIsSkill(true);
}
void PlKnight::SkillTwoInit(void)
{

	using ATK_ACT = PlayerBase::ATK_ACT;
	if (obj_->GetCoolTime(ATK_ACT::SKILL2) > Knight::GUARD_STARTABLE_COOL && !obj_->IsAtkStart())
	{
		obj_->SetIsCool(false, ATK_ACT::SKILL2);
		obj_->ChangeAct(static_cast<ATK_ACT>(ATK_ACT::SKILL2));
		obj_->ResetParam();
		//開始時に減ったクールタイムの残り
		float restCoolTime = obj_->GetCoolTime(ATK_ACT::SKILL2) - Knight::SKILL_TWO_START_COOLTIME;
		obj_->SetCoolTime(restCoolTime , ATK_ACT::SKILL2);
		obj_->SetDulation(restCoolTime);
		//atk_.duration_ = coolTime_[static_cast<int>(ATK_ACT::SKILL2)];
		//CntUp(atkStartCnt_);
		obj_->SetIsAtk(false);
		obj_->SetIsSkill(true);
	}
}
//近接攻撃用
