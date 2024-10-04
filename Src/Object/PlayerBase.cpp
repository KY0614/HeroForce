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
	//移動方向
	VECTOR movePow = VScale(dir, SPEED_MOVE);
	//移動処理
	transform_.pos = VAdd(transform_.pos, movePow);
}

void PlayerBase::KeyBoardControl(void)
{
	auto& ins = InputManager::GetInstance();
	//前
	if(ins.IsNew(KEY_INPUT_W))
	{
		Move(DIR::FLONT);
	}
	//左
	if (ins.IsNew(KEY_INPUT_A))
	{
		Move(DIR::LEFT);
	}
	//下
	if(ins.IsNew(KEY_INPUT_S))
	{
		Move(DIR::BACK);
	}
	//右
	if(ins.IsNew(KEY_INPUT_D))
	{
		Move(DIR::RIGHT);
	}
}

void PlayerBase::Turn(float deg, VECTOR axis)
{
	transform_.quaRot =
		transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));
}
