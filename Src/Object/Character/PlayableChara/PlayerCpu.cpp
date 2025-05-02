//#include"../Manager/Collision.h"
#include"../PlayerDodge.h"
#include "PlayerCpu.h"

PlayerCpu::PlayerCpu(void)
{
	
}
void PlayerCpu::SetParam(void)
{
	cpuStateChanges_.emplace(CPU_STATE::NORMAL, std::bind(&PlayerCpu::CpuChangeNml, this));
	cpuStateChanges_.emplace(CPU_STATE::ATTACK, std::bind(&PlayerCpu::CpuChangeAtk, this));
	cpuStateChanges_.emplace(CPU_STATE::BREAK, std::bind(&PlayerCpu::CpuChangeBreak, this));
}

void PlayerCpu::Update(void)
{
	PlayerBase::Update();
	//各状態の更新
	CpuStateUpdate();
	auto& ins = InputManager::GetInstance();
	//CPUを強制的に呼び出す
	if (ins.IsTrgDown(KEY_INPUT_X))
	{
		isCall_ = true;
	}
	if (!isMove2CallPlayer_			//プレイヤーに向かって動いてないとき		
		&& isCall_					//呼んだ時
		&& !AsoUtility::IsHitSpheres(//プレイヤーとの間隔外にいるとき
			trans_.pos				 
			, SEARCH_RANGE
			, userOnePos_
			, MY_COL_RADIUS)
		&& cpuState_ == CPU_STATE::BREAK)//
	{
		isMove2CallPlayer_ = true;
		ChangeState(CPU_STATE::NORMAL);

		//前の攻撃の初期化
		preAtk_ = ATK_ACT::MAX;

		//呼び出されて移動するときの加速度
		calledMoveSpeed_ = CALLED_MOVE_SPEED_SCALE;

		//プレイヤーに優先的についていきたいから敵を察知しないようにする
		isEnemySerch_ = false;

		//ターゲットを呼び出されたプレイヤーに設定する
		//ターゲットを呼び出されたプレイヤーに設定する
		SetTargetPos(userOnePos_);
	}



#ifdef DEBUG_ON
	{
		auto& ins = InputManager::GetInstance();
		const float SPEED = 5.0f;
		if (ins.IsNew(KEY_INPUT_W)) { userOnePos_.z += SPEED; }
		if (ins.IsNew(KEY_INPUT_D)) { userOnePos_.x += SPEED; }
		if (ins.IsNew(KEY_INPUT_S)) { userOnePos_.z -= SPEED; }
		if (ins.IsNew(KEY_INPUT_A)) { userOnePos_.x -= SPEED; }



		// 左スティックの横軸
		leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;

		//縦軸
		leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;
		auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));
		stickDeg_ = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(stickRad) + 90.0f));
		//前
		if (leftStickY_ < -1) { userOnePos_.z += SPEED; }
		//右
		else if (leftStickX_ > 1) { userOnePos_.x += SPEED; }
		//下
		else if (leftStickY_ > 1) { userOnePos_.z -= SPEED; }
		//左
		else if (leftStickX_ < -1) { userOnePos_.x -= SPEED; }

	}

#endif // DEBUG_ON

}

void PlayerCpu::CpuStateUpdate(void)
{
	cpuStateUpdate_();
}


void PlayerCpu::CpuMove(VECTOR _targetPos)
{
	//移動速度の更新
	speed_ = SPEED_MOVE * calledMoveSpeed_;
	isMove_ = true;

	//方向ベクトル取得
	VECTOR targetVec = GetTargetVec(_targetPos);

	//回転
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);

	//移動
	trans_.pos = VAdd(trans_.pos, targetVec);
}

void PlayerCpu::ChangeState(CPU_STATE _state)
{
	cpuState_ = _state;
	cpuStateChanges_[_state]();
}

void PlayerCpu::CpuReset(void)
{
	searchRange_ = SEARCH_RANGE;

	preAtk_ = ATK_ACT::MAX;

	isEnemySerch_ = false;

	isMove2CallPlayer_ = false;
}

//役割ごとのCPU処理を作るので残しておく
void PlayerCpu::CpuActUpdate(ATK_ACT _act)
{

}

void PlayerCpu::CpuChangeNml(void)
{
	cpuStateUpdate_ = std::bind(&PlayerCpu::CpuNmlUpdate, this);
}

void PlayerCpu::CpuChangeAtk(void)
{
	isCall_ = false;
	cpuStateUpdate_ = std::bind(&PlayerCpu::CpuAtkUpdate, this);
}

void PlayerCpu::CpuChangeBreak(void)
{
	breakCnt_ = 0.0f;
	cpuStateUpdate_ = std::bind(&PlayerCpu::CpuBreakUpdate, this);
}

void PlayerCpu::CpuNmlUpdate(void)
{
	//auto& col = Collision::GetInstance();
	const float MOVE_DELAY_MAX = 0.3f;

	//待機アニメーション
	//停止状態の時のアニメーション
	if (!isMove_ && !dodge_->IsDodge() && !IsAtkAction())
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
		speed_ = 0.0f;
	}
	//歩きアニメーション
	else
	{
		ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);
	}

	//移動量の初期化
	speed_ = 0.0f;

	//preAtk_ = ATK_ACT::MAX;

	if (!isEnemySerch_)
	{
		if (AsoUtility::IsHitSpheres(trans_.pos, FOLLOW_PLAYER_RADIUS, userOnePos_, FOLLOW_PLAYER_RADIUS))
		{
			//呼び出しが終わったらそれぞれの初期化
			isMove_ = false;
			isMove2CallPlayer_ = false;
			calledMoveSpeed_ = 1.0f;
			isCall_ = false;
			moveStartDelay_ = 0.0f;
		}
		else
		{
			//プレイヤーのところまで動く
			targetPos_ = userOnePos_;
			CntUp(moveStartDelay_);
			if (moveStartDelay_ >= MOVE_DELAY_MAX)
			{
				moveStartDelay_ = MOVE_DELAY_MAX;
				CpuMove(targetPos_);
			}
		}
	}
	else
	{
		//敵を察知したら、ゲームシーン側でSetTargetPosの中に敵の座標を入れて
		//それに向けて動かす
		CpuMove(targetPos_);
	}


}

void PlayerCpu::CpuAtkUpdate(void)
{
	//攻撃が終わっているなら状態遷移
	if (atk_.IsFinishMotion())
	{
		//前の攻撃の初期化
		preAtk_ = ATK_ACT::MAX;

		//休憩状態に遷移
		ChangeState(CPU_STATE::BREAK);
		return;
	}
	//前の攻撃が何もなかったら、攻撃種類を決める
	if (preAtk_ == ATK_ACT::MAX)
	{
		for (int i = ATK_TOTAL - 1; i > -1; i--)
		{
			if (!isCool_[i])
			{
				ChangeAct(static_cast<ATK_ACT>(i));
				preAtk_ = static_cast<ATK_ACT>(i);
				return;
			}
			else
			{
				continue;
			}
		}
	}
}

void PlayerCpu::CpuBreakUpdate(void)
{
	const float BREAK_TIME = 2.0f;
	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
	if (breakCnt_ > BREAK_TIME)
	{
		//ブレイクが終わったら通常状態へ移行
		ChangeState(CPU_STATE::NORMAL);
		return;
	}
	CntUp(breakCnt_);
}