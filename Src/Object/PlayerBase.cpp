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
	frameAtk_ = 0;
}

void PlayerBase::Update(void)
{
	KeyBoardControl();
}

void PlayerBase::Draw(void)
{
	DrawSphere3D(transform_.pos, 20.0f, 8, 0xff0000, 0xffffff, true);
	DrawFormatString(0, 0, 0xffffff, "FrameATK(%d)\nisAtk(%d)", frameAtk_,IsAtk());
}

void PlayerBase::Move(VECTOR _dir)
{
	if (!IsAtk())
	{
		//移動方向
		VECTOR movePow = VScale(_dir, SPEED_MOVE);
		//移動処理
		transform_.pos = VAdd(transform_.pos, movePow);
	}
}

void PlayerBase::KeyBoardControl(void)
{
	auto& ins = InputManager::GetInstance();
	//前
	if(ins.IsNew(KEY_INPUT_W))
	{
		Move(AsoUtility::DIR_F);
	}
	//左
	if (ins.IsNew(KEY_INPUT_A))
	{
		Move(AsoUtility::DIR_L);
	}
	//下
	if(ins.IsNew(KEY_INPUT_S))
	{
		Move(AsoUtility::DIR_B);
	}
	//右
	if(ins.IsNew(KEY_INPUT_D))
	{
		Move(AsoUtility::DIR_R);
	}
	
	//攻撃（攻撃アニメーションのフレームが0以下だったら）
	if (ins.IsTrgDown(KEY_INPUT_E)&&frameAtk_<=0)
	{
		//攻撃のアニメーション開始
		frameAtk_ = FRAME_ATK_MAX;
	}
	Attack();

}

void PlayerBase::Turn(float deg, VECTOR axis)
{
	transform_.quaRot =
		transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));
}

bool PlayerBase::IsAtk(void)
{
	InputManager& ins = InputManager::GetInstance();
	
	if (frameAtk_ > 0||ins.IsTrgDown(KEY_INPUT_E))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PlayerBase::Attack(void)
{
	if (IsAtk())
	{
		if (frameAtk_ <= 0)
		{
			return;
		}
		frameAtk_--;
	}
	
}
