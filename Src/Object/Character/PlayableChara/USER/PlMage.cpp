#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"
#include"../Mage.h"
#include "PlMage.h"

PlMage::PlMage(const SceneManager::CNTL _cntl)
{
	info_.cntrol_ = _cntl;
}

PlMage::PlMage(const InputManager::JOYPAD_NO _padNum)
{
	info_.cntrol_ = SceneManager::CNTL::PAD;
	padNum_ = _padNum;
}

void PlMage::Init(void)
{
	obj_ = new Mage();
	obj_->Init();
	SetInitPos(playerNum_);
}

void PlMage::Update(void)
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

void PlMage::Draw(void)
{
	obj_->Draw();
}

void PlMage::Release(void)
{
	obj_->Destroy();
	delete obj_;
	obj_ = nullptr;
}

void PlMage::AtkInput(void)
{
}

void PlMage::SkillOneInput(void)
{
}

void PlMage::SkillTwoInput(void)
{
}
