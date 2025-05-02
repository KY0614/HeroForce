#include "PlayerDodge.h"
#include "../Application.h"

void PlayerDodge::Init()
{
	dodgeCdt_ = DODGE_CDT_MAX;
	dodgeCnt_ = 0.0f;
}

void PlayerDodge::Update(Transform& transform)
{
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	//ドッジフラグがtrueになったら
	if (IsDodge() && !IsCoolDodge())
	{
		// カウントを加算
		dodgeCnt_ += deltaTime;

		// ドッジのカウントを追加
		//CntUp(dodgeCnt_);

		//スキル中に回避が出た時にスキルのカウントをリセット
		//atk_.ResetCnt();

		// 移動中
		if (IsDodge())
		{
			VECTOR dir = transform.GetForward();
			//移動方向
			VECTOR movePow = VScale(dir, SPEED_DODGE);
			//移動処理
			transform.pos = VAdd(transform.pos, movePow);
		}
		else
		{
			dodgeCdt_ = 0.0f;
		}
	}
	else
	{
		dodgeCdt_ += deltaTime;
		ResetDodgeFrame();
#ifdef DEBUG_ON
		color_Col_ = 0xffffff;
#endif // DEBUG_ON
	}
}

const bool PlayerDodge::IsDodge(void)const
{
	return 0.0f < dodgeCnt_ && dodgeCnt_ < FRAME_DODGE_MAX; 
}

const bool PlayerDodge::IsCoolDodge(void)const
{
	return dodgeCdt_ < DODGE_CDT_MAX;
}

void PlayerDodge::Reset()
{
	dodgeCdt_ = DODGE_CDT_MAX;
}

const bool PlayerDodge::IsDodgeable()const
{ 
	return !IsDodge() &&  !IsCoolDodge(); 
}

//void PlayerDodge::Dodge(Transform* trans)
//{
////	//ドッジフラグがtrueになったら
////	if (IsDodge() && !IsCoolDodge())
////	{
////		// ドッジのカウントを追加
////		CntUp(dodgeCnt_);
////
////		//スキル中に回避が出た時にスキルのカウントをリセット
////		atk_.ResetCnt();
////
////		// 移動中
////		if (dodgeCnt_ < FRAME_DODGE_MAX)
////		{
////			VECTOR dir = trans->GetForward();
////			//移動方向
////			VECTOR movePow = VScale(dir, SPEED_DODGE);
////			//移動処理
////			trans->pos = VAdd(trans_.pos, movePow);
////		}
////		else
////		{
////			dodgeCdt_ = 0.0f;
////		}
////	}
////	else
////	{
////		CntUp(dodgeCdt_);
////		ResetDodgeFrame();
////#ifdef DEBUG_ON
////		color_Col_ = 0xffffff;
////#endif // DEBUG_ON
//	}
////}
