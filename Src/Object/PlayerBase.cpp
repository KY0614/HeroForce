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
	frameDodge_ = 0;
	color_ = 0xffffff;
}

void PlayerBase::Update(void)
{
	KeyBoardControl();

	//攻撃中か毎フレーム判定する
	Attack();

	//回避
	Dodge();

	
}

void PlayerBase::Draw(void)
{
	//球体
	DrawSphere3D(transform_.pos, 20.0f, 8, 0x0, color_, true);
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
	if(ins.IsNew(KEY_INPUT_W)){ Move(transform_.quaRot.GetForward());}

	//左
	if (ins.IsNew(KEY_INPUT_A)){ Move(transform_.quaRot.GetLeft());}

	//下
	if(ins.IsNew(KEY_INPUT_S)){ Move(transform_.quaRot.GetBack());}

	//右
	if(ins.IsNew(KEY_INPUT_D)){ Move(transform_.quaRot.GetRight());}
	
	//攻撃（攻撃アニメーションのフレームが0以下だったらフレームを設定）
	if (ins.IsTrgDown(KEY_INPUT_E)&&!IsAtk()){frameAtk_ = FRAME_ATK_MAX;}
	
	//回避
	if (ins.IsTrgDown(KEY_INPUT_N))
	{frameDodge_ = FRAME_DODGE_MAX;}

}

void PlayerBase::Turn(float deg, VECTOR axis)
{
	transform_.quaRot =
		transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));
}


void PlayerBase::Attack(void)
{
	if (IsAtk())
	{
		frameAtk_--;
		color_ = 0xff0000;
	}
	else
	{
		color_ = 0xffffff;
	}
	
}

void PlayerBase::Dodge(void)
{
	if (IsDodge())
	{
		frameDodge_--;
	}
	else
	{
		color_ = 0xffffff;
	}
}
