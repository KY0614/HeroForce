#include"../Archer.h"
#include "PlArcher.h"

PlArcher::PlArcher(const SceneManager::CNTL _cntl)
{
}

PlArcher::PlArcher(const InputManager::JOYPAD_NO _padNum)
{
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
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	////通常攻撃
	////---------------------------------------------------------
	//if (ins.CheckAct(ACT_CNTL::NMLATK))
	//{
	//	NmlAtkInit();
	//	isAtk_ = true;
	//}


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
