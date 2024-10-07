#include"../Manager/InputManager.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(void)
{
}

void PlayerBase::Destroy(void)
{
}

void PlayerBase::Init(void)
{
}

void PlayerBase::Update(void)
{
	KeyBoardControl();
}

void PlayerBase::Draw(void)
{
	DrawSphere3D(transform_.pos, 20.0f, 8, 0xff0000, 0xffffff, true);
}

void PlayerBase::Move(DIR _dir)
{
	VECTOR dir;
	switch (_dir)
	{
	case PlayerBase::DIR::FLONT:
		dir = AsoUtility::DIR_F;
		break;
	case PlayerBase::DIR::BACK:
		dir = AsoUtility::DIR_B;
		break;
	case PlayerBase::DIR::LEFT:
		dir = AsoUtility::DIR_L;
		break;
	case PlayerBase::DIR::RIGHT:
		dir = AsoUtility::DIR_R;
		break;
	default:
		break;
	}
	//à⁄ìÆï˚å¸
	VECTOR movePow = VScale(dir, SPEED_MOVE);
	//à⁄ìÆèàóù
	transform_.pos = VAdd(transform_.pos, movePow);
}

void PlayerBase::KeyBoardControl(void)
{
	auto& ins = InputManager::GetInstance();
	//ëO
	if(ins.IsNew(KEY_INPUT_W))
	{
		Move(DIR::FLONT);
	}
	//ç∂
	if (ins.IsNew(KEY_INPUT_A))
	{
		Move(DIR::LEFT);
	}
	//â∫
	if(ins.IsNew(KEY_INPUT_S))
	{
		Move(DIR::BACK);
	}
	//âE
	if(ins.IsNew(KEY_INPUT_D))
	{
		Move(DIR::RIGHT);
	}

	if (ins.IsNew(KEY_INPUT_E))
	{
		Attack();
	}

}

void PlayerBase::Turn(float deg, VECTOR axis)
{
	transform_.quaRot =
		transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));
}

void PlayerBase::Attack(void)
{
	
}
