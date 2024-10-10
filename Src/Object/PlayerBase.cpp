#include"../Manager/InputManager.h"
#include"../Manager/ResourceManager.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(void)
{
}

void PlayerBase::Destroy(void)
{
}

void PlayerBase::Init(void)
{
	frameAtk_ = FRAME_ATK_MAX;
	frameDodge_ = FRAME_DODGE_MAX;
	color_ = 0xffffff;

	//モデルの初期化
	transform_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	float scale = 0.5f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transform_.Update();
}

void PlayerBase::Update(void)
{
	//モデルの更新
	transform_.Update();
	//コントロール系
	KeyBoardControl();

	//攻撃中か毎フレーム判定する
	Attack();

	//回避
	Dodge();

	
}

void PlayerBase::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void PlayerBase::Move(float _deg, VECTOR _axis)
{
	if (!IsAtk())
	{
		Turn(_deg, _axis);
		VECTOR dir = transform_.GetForward();
		//移動方向
		VECTOR movePow = VScale(dir, SPEED_MOVE);
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
		Move(0.0f, AsoUtility::AXIS_Y);
	}

	//右
	if (ins.IsNew(KEY_INPUT_D))
	{
		Move(90.0f, AsoUtility::AXIS_Y);
	}

	//下
	if(ins.IsNew(KEY_INPUT_S))
	{ 
		Move(180.0f, AsoUtility::AXIS_Y);
	}

	//左
	if(ins.IsNew(KEY_INPUT_A))
	{ 
		Move(270.0f, AsoUtility::AXIS_Y);
	}
	
	//攻撃（攻撃アニメーションのフレームが0以下だったらフレームを設定）
	if (ins.IsTrgDown(KEY_INPUT_E)&&!IsAtk()){frameAtk_ = 0;}
	
	//回避
	if (ins.IsTrgDown(KEY_INPUT_N)&&!IsDodge()&&!IsAtk())
	{
		frameDodge_ = 0;
	}

}

void PlayerBase::DrawDebug(void)
{
	//球体
	DrawSphere3D(transform_.pos, 20.0f, 8, 0x0, color_, true);
	//値見る用
	DrawFormatString(0, 0, 0xffffff, "FrameATK(%d)\nisAtk(%d)", frameAtk_, IsAtk());
}

void PlayerBase::Turn(float _deg, VECTOR _axis)
{
	//transform_.quaRot =
	//	transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));

	transform_.quaRot =
		transform_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}


void PlayerBase::Attack(void)
{
	if (IsAtk())
	{
		frameAtk_++;
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
		frameDodge_++;
		if (frameDodge_ < FRAME_DODGE_MAX)
		{
			VECTOR dir = transform_.GetForward();
			//移動方向
			VECTOR movePow = VScale(dir, SPEED_DODGE);
			//移動処理
			transform_.pos = VAdd(transform_.pos, movePow);
		}
	}
	else
	{
		dodgeCdt_ = 0;
		color_ = 0xffffff;
	}
}
