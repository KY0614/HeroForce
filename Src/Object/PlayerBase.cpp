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
	
	frameDodge_ = FRAME_DODGE_MAX;
	dodgeCdt_ = DODGE_CDT_MAX;
	color_ = 0xffffff;

	atk_.cnt_ = FRAME_ATK_MAX;
	atk_.duration_ = FRAME_ATK_DURATION;
	atk_.pow_ = 0;




	//モデルの初期化
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	float scale = 0.5f;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { 0.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	trans_.Update();
}

void PlayerBase::Update(void)
{
	//モデルの更新
	trans_.Update();
	//コントロール系
	KeyBoardControl();

	//攻撃中か毎フレーム判定する
	Attack();

	//回避
	Dodge();

	
}

void PlayerBase::Draw(void)
{
	MV1DrawModel(trans_.modelId);
	DrawDebug();
}

void PlayerBase::Move(float _deg, VECTOR _axis)
{
	if (!atk_.IsAttack()&&!IsDodge())
	{
		Turn(_deg, _axis);
		VECTOR dir = trans_.GetForward();
		//移動方向
		VECTOR movePow = VScale(dir, SPEED_MOVE);
		//移動処理
		trans_.pos = VAdd(trans_.pos, movePow);
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
	if (ins.IsTrgDown(KEY_INPUT_E)/*&&!atk_.IsAttack()*/)
	{
		if (!atk_.IsAttack())
		{
			atk_.cnt_++;
		}
		
	}

	if (ins.IsNew(KEY_INPUT_Q))
	{ Skill_1(); }
	
	//回避
	if (ins.IsTrgDown(KEY_INPUT_N)&&!IsDodge()&&!atk_.IsAttack()){ frameDodge_ = 0; }

}

void PlayerBase::DrawDebug(void)
{
	//球体
	//DrawSphere3D(trans_.pos, 20.0f, 8, 0x0, color_, true);
	//値見る用
	DrawFormatString(0, 0, 0xffffff, "FrameATK(%f)\nisAtk(%d)", atk_.cnt_, atk_.IsAttack());
}

void PlayerBase::Turn(float _deg, VECTOR _axis)
{
	//transform_.quaRot =
	//	transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));

	trans_.quaRot =
		trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}


void PlayerBase::Attack(void)
{
	if (atk_.IsAttack())
	{
		atk_.cnt_++;
		color_ = 0xff0000;
	}
	else
	{
		atk_.ResetCnt();
		color_ = 0xffffff;
	}
	
}

void PlayerBase::Dodge(void)
{
	//ドッジフラグがtrueになったら
	if (IsDodge()&&!IsCoolDodge())
	{
		frameDodge_++;
		if (frameDodge_ < FRAME_DODGE_MAX)
		{
			VECTOR dir = trans_.GetForward();
			//移動方向
			VECTOR movePow = VScale(dir, SPEED_DODGE);
			//移動処理
			trans_.pos = VAdd(trans_.pos, movePow);
		}
		else
		{
			dodgeCdt_ = 0;
		}
	}
	else
	{
		dodgeCdt_ ++;
		color_ = 0xffffff;
	}
}

void PlayerBase::Skill_1(void)
{
	skillNum_ = "Skill_1";
}

void PlayerBase::Skill_2(void)
{
	skillNum_ = "Skill_2";
}

