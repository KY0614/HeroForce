#include"../Knight.h"
#include "PlKnight.h"

PlKnight::PlKnight(const SceneManager::CNTL _cntl)
{
}

PlKnight::PlKnight(const InputManager::JOYPAD_NO _padNum)
{
}

void PlKnight::Init(void)
{
	obj_ = new Knight();
	obj_->Init();
}

void PlKnight::Update(void)
{
	obj_->Update();
	////通常攻撃
	////---------------------------------------------------------
	//if (ins.CheckAct(ACT_CNTL::SKILL_DOWN))
	//{
	//	InitSkill(skillNo_);
	//}
	////---------------------------------------------------------
	////スキル1
	////---------------------------------------------------------
	//	//入力
	//auto& ins = PlayerInput::GetInstance();
	//using ACT_CNTL = PlayerInput::ACT_CNTL;
	//if (ins.CheckAct(ACT_CNTL::SKILL_DOWN))
	//{
	//	//ボタンの押しはじめの時に値初期化
	//	SkillTwoInit();
	//}
	////スキル(長押しでガード状態維持)
	//if (ins.CheckAct(ACT_CNTL::SKILL_KEEP) && isSkill_)
	//{
	//	if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] > 0.0f)
	//	{
	//		moveAble_ = false;
	//		isCool_[static_cast<int>(SKILL_NUM::TWO)] = false;
	//		if (stepAnim_ >= 10.0f)
	//		{
	//			stepAnim_ = 10.0f;
	//		}
	//	}
	//}
	//else if (ins.CheckAct(ACT_CNTL::SKILL_UP) && isSkill_)
	//{
	//	isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
	//	isSkill_ = false;
	//	InitAtk();
	//}
	////---------------------------------------------------------
	////スキル2
	////---------------------------------------------------------





	////---------------------------------------------------------
}

void PlKnight::Draw(void)
{
	obj_->Draw();
}

void PlKnight::Release(void)
{
}
