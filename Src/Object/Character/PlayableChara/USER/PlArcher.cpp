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
	//����
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	////�ʏ�U��
	////---------------------------------------------------------
	//if (ins.CheckAct(ACT_CNTL::NMLATK))
	//{
	//	NmlAtkInit();
	//	isAtk_ = true;
	//}


	//---------------------------------------------------------
	//�X�L��1
	//---------------------------------------------------------




	//---------------------------------------------------------
	//�X�L��2
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
