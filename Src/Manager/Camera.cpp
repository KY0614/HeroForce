#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/PlayerBase.h"
#include "Camera.h"

Camera::Camera(void)
{
	mode_ = MODE::NONE;
	pos_ = { 0.0f, 0.0f, 0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	rot_ = Quaternion::Identity();
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	//カメラの初期設定
	SetDefault();

	moveSpeed_ = 0.0f;

}

void Camera::Update(void)
{

}

void Camera::SetBeforeDraw(void)
{

	//クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::NONE:
		SetBeforeDrawFollow();
		break;

	case Camera::MODE::FIXED_POINT:
		//SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;

	case Camera::MODE::FOLLOW_SPRING:
		SetBeforeDrawFollowSpring();
		break;

	case Camera::MODE::SHAKE:
		SetBeforeDrawShake();
		break;
	}

	//カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	//Effekseer_Sync3DSetting();
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//何もしない
}

void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();

	ProcessMove();

	Decelerate(MOVE_DEC);

	Move();

}

void Camera::SetBeforeDrawFollow(void)
{
	//追従対象の位置
	VECTOR followPos = followTransform_->pos;

	//追従対象の向き
	Quaternion followRot = followTransform_->quaRot;

	//追従対象からカメラまでの相対座標
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	//カメラ位置の更新
	pos_ = VAdd(followPos, relativeCPos);

	//カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);

	//注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);

	//カメラの上方向
	cameraUp_ = followRot.PosAxis(rot_.GetUp());

}

void Camera::SetBeforeDrawFollowSpring(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_C))
	{
		ChangeMode(MODE::SHAKE);
	}

	//ばね定数(ばねの強さ)
	float POW_SPRING = 50.0f;

	//ばね定数（ばねの抵抗）
	float dampening = 2.0f * sqrt(POW_SPRING);

	//デルタタイム
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//3D酔いする人用
	//delta = 1.0f / 60.0f;

	//追従対象の位置
	VECTOR followPos = followTransform_->pos;

	//追従対象の向き
	Quaternion followRot = followTransform_->quaRot;
	VECTOR zero = { 0.0f,0.0f,0.0f };

	//カメラの方向を固定する用
	Quaternion forward = Quaternion::Euler(zero);

	//追従対象からカメラまでの相対座標
	VECTOR relativeCPos = forward.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	//理想位置
	VECTOR idealPos = VAdd(followPos, relativeCPos);

	//実際と理想の差
	VECTOR diff = VSub(pos_, idealPos);

	//力 = -バネの強さ × バネの伸び - 抵抗 × カメラの速度
	VECTOR force = VScale(diff, -POW_SPRING);
	force = VSub(force, VScale(velocity_, dampening));

	//速度の更新
	velocity_ = VAdd(velocity_, VScale(force, delta));

	//カメラ位置の更新
	pos_ = VAdd(pos_, VScale(velocity_, delta));

	//カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = forward.PosAxis(RELATIVE_C2T_POS);

	//注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);

	//カメラの上方向
	cameraUp_ = forward.PosAxis(rot_.GetUp());

}

void Camera::SetBeforeDrawShake(void)
{
	// 一定時間カメラを揺らす
	stepShake_ -= SceneManager::GetInstance().GetDeltaTime();

	if (stepShake_ < 0.0f)
	{
		pos_ = defaultPos_;
		ChangeMode(MODE::FOLLOW_SPRING);
		return;
	}

	// -1.0f～1.0f
	float f = sinf(stepShake_ * SPEED_SHAKE);

	// -1000.0f～1000.0f
	f *= 1000.0f;

	// -1000 or 1000
	int d = static_cast<int>(f);

	// 0 or 1
	int shake = d % 2;

	// 0 or 2
	shake *= 1;

	// -1 or 1
	shake -= 1;

	// 移動量
	VECTOR velocity = VScale(shakeDir_, (float)(shake)*WIDTH_SHAKE);

	// 移動先座標
	 pos_ = VAdd(defaultPos_, velocity);

	//float pow = WIDTH_SHAKE * sinf(stepShake_ * SPEED_SHAKE);
	//VECTOR velocity = VScale(shakeDir_, pow);
	//VECTOR newPos = VAdd(defaultPos_, velocity);
	//pos_ = newPos;

	DrawFormatString(0, 40, 0xFFFFFF, "pos : %.2f,%.2f,%.2f", pos_);
	DrawFormatString(0, 60, 0xFFFFFF, "pos : %.2f,%.2f,%.2f", targetPos_);
}

void Camera::SetBeforeDrawFollowDelay(void)
{
	//int screenX = Application::SCREEN_SIZE_X;
	//int screenY = Application::SCREEN_SIZE_Y;

	////左上座標
	//int topL = (screenX / 2) - 150; 
	////左下
	//int underL = screenX / 2 + 150; 
	////右上
	//int topR = screenY / 2 - 80;
	////右下
	//int underR = screenY / 2 + 80;


	////カメラが移動し始める画角
	//DrawBox(topL, topR,
	//	underL, underR,
	//	0xFFFFFF, false);


	//// 追従対象の位置
	//VECTOR followPos = followTransform_->pos;
	//VECTOR zero = { 0.0f,0.0f,0.0f };

	//// 追従対象の向き
	//Quaternion followRot = followTransform_->quaRot;

	////
	//VECTOR pos2D = ConvWorldPosToScreenPos(followPos);
	//VECTOR pos3D = ConvScreenPosToWorldPos(pos2D);

	//DrawFormatString(0, 70, 0xFFFFFF, "SCREENSIZE X : %d,Y : %d", screenX, screenY);
	//DrawFormatString(0, 90, 0xFFFFFF, "WP2SP : %.2f,%.2f,%.2f", pos2D.x, pos2D.y, pos2D.z);
	//DrawFormatString(0, 110, 0xFFFFFF, "Player : %.2f,%.2f,%.2f", followPos.x, followPos.y, followPos.z);


	//auto& ins = InputManager::GetInstance();

	//Vector2 mPos = ins.GetMousePos();

	////マウスの座標をワールド座標に変換
	//VECTOR startPos = { mPos.x,mPos.y,0.0f };
	//startPos = ConvScreenPosToWorldPos(startPos);

	////プレイヤーの座標をスクリーン座標に変換
	//VECTOR endPos = { followPos.x,followPos.y,followPos.z };

	//DrawLine3D(followPos, startPos, 0xff9999);

	////カメラの方向を固定する用
	//Quaternion forward = Quaternion::Euler(zero);

#pragma region 追従機能
	//// 画面境界を考慮してカメラの位置を調整
	//if (pos2D.x < -MOVE_SIZE_X/2
	//	||pos2D.x > MOVE_SIZE_X/2) {

	//	zero = followPos;

	//}

	//// 追従対象からカメラまでの相対座標
	//VECTOR relativeCPos = forward.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	//// カメラ位置の更新
	//pos_ = VAdd(zero, relativeCPos);

	//// カメラ位置から注視点までの相対座標
	//VECTOR relativeTPos = forward.PosAxis(RELATIVE_C2T_POS);

	//// 注視点の更新
	//targetPos_ = VAdd(pos_, relativeTPos);

	//// カメラの上方向
	//cameraUp_ = forward.PosAxis(rot_.GetUp());
#pragma endregion


	//DrawSphere3D(followPos, 20.0f, 10, 0x589763, 0x245354, false);
	//DrawSphere3D(targetPos_, 20.0f, 10, 0x00ff00, 0x00ff00, false);

}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

void Camera::ChangeMode(MODE mode)
{

	//カメラの初期設定
	//SetDefault();

	//カメラモードの変更
	mode_ = mode;

	//変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::FOLLOW_SPRING:
		break;
	case Camera::MODE::SHAKE:
		stepShake_ = TIME_SHAKE;
		shakeDir_ = VNorm({ 0.7f, 0.7f ,0.0f });
		defaultPos_ = pos_;
	}

}

const void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

void Camera::SetDefault(void)
{

	//カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	//注視点
	targetPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	//カメラの上方向
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	//カメラはX軸に傾いているが、
	//この傾いた状態を角度ゼロ、傾き無しとする
	rot_ = Quaternion::Identity();

	velocity_ = AsoUtility::VECTOR_ZERO;

}

void Camera::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	//移動
	//moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; Accele(MOVE_ACC); }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; Accele(MOVE_ACC); }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; Accele(MOVE_ACC); }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; Accele(MOVE_ACC); }

	//回転軸と量を決める
	const float ROT_POW = 1.0f;
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x = -1.0f; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x = 1.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y = -1.0f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y = 1.0f; }


	//カメラ座標を中心として、注視点を回転させる
	if (!AsoUtility::EqualsVZero(axisDeg))
	{
		//今回の回転量を合成
		Quaternion rotPow;
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.z), AsoUtility::AXIS_Z));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.x), AsoUtility::AXIS_X));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.y), AsoUtility::AXIS_Y));

		//カメラの回転の今回の回転量を加える（合成）
		rot_ = rot_.Mult(rotPow);

		//注視点の相対座標を回転させる
		VECTOR rotLocalPos = rot_.PosAxis(RELATIVE_C2T_POS);

		//注視点更新
		targetPos_ = VAdd(pos_, rotLocalPos);

		//カメラの上方向更新
		cameraUp_ = rot_.GetUp();
	}
}

void Camera::Move(void)
{
	//移動処理
	if (!AsoUtility::EqualsVZero(moveDir))
	{
		//移動 = 座標 + 移動量
		//移動量 = 方向 * スピード 

		//入力された方向をカメラの回転情報を使って、
		//カメラの進行方向に変換する
		VECTOR direction = rot_.PosAxis(moveDir);

		//移動量
		VECTOR movePow = VScale(direction, moveSpeed_);

		//移動処理
		pos_ = VAdd(pos_, movePow);

		targetPos_ = VAdd(targetPos_, movePow);
	}
}

void Camera::Accele(float speed)
{
	moveSpeed_ += speed;

	//速度制限(右方向)
	if (moveSpeed_ > MAX_MOVE_SPEED)
	{
		moveSpeed_ = MAX_MOVE_SPEED;
	}

	//速度制限(左方向)
	if (moveSpeed_ < -MAX_MOVE_SPEED)
	{
		moveSpeed_ = -MAX_MOVE_SPEED;
	}
}

void Camera::Decelerate(float speed)
{
	//右方向の移動を減速させる
	if (moveSpeed_ > 0.0f)
	{
		moveSpeed_ -= speed;
		if (moveSpeed_ < 0.0f)
		{
			moveSpeed_ = 0.0f;
		}
	}

	//左方向の移動を減速させる
	if (moveSpeed_ < 0.0f)
	{
		moveSpeed_ += speed;
		if (moveSpeed_ > 0.0f)
		{
			moveSpeed_ = 0.0f;
		}
	}
}
