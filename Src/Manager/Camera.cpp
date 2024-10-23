#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Transform.h"
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

	SetShake(0.0f, 0.0f);

	moveSpeed_ = 0.0f;

	 isVibrating = false;
	 vibrationStrength = 0.1f; 
	 vibrationDuration = 20;	
	 currentVibrationTime = 0; 

}

void Camera::Update(void)
{
	Shake();
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

	case Camera::MODE::FOLLOW_DELAY:
		SetBeforeDrawFollowDelay();
		break;
	}

	//カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

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
	//ばね定数(ばねの強さ)
	float POW_SPRING = 24.0f;

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

	//追従対象からカメラまでの相対座標
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_SPRING);

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
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);

	//注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);

	//カメラの上方向
	cameraUp_ = followRot.PosAxis(rot_.GetUp());
}

void Camera::SetBeforeDrawFollowDelay(void)
{
	int screenX = Application::SCREEN_SIZE_X;
	int screenY = Application::SCREEN_SIZE_Y;

	//左上座標
	int topL = screenX / 2 - 150;
	//左下
	int underL = screenX / 2 + 150;
	//右上
	int topR = screenY / 2 - 80;
	//右下
	int underR = screenY / 2 + 80;

	//移動範囲の最大画角
	DrawBox(screenX / 2 - 400, screenY / 2 - 200,
		screenX / 2 + 400, screenY / 2 + 200,
		0xFF00FFF, false);

	//カメラが移動し始める画角
	DrawBox(topL, topR,
		underL, underR,
		0xFFFFFF, false);


	//追従対象の位置
	VECTOR followPos = followTransform_->pos;
	VECTOR zero = { 0.0f,0.0f,0.0f };


	//追従対象の向き
	Quaternion followRot = followTransform_->quaRot;

	//
	VECTOR pos2D = ConvWorldPosToScreenPos(followPos);
	VECTOR pos3D = ConvScreenPosToWorldPos(pos2D);

	DrawFormatString(0, 70, 0xFFFFFF, "SCREENSIZE X : %d,Y : %d", screenX, screenY);
	DrawFormatString(0, 90, 0xFFFFFF, "WP2SP : %.2f,%.2f,%.2f", pos2D.x, pos2D.y, pos2D.z);
	DrawFormatString(0, 110, 0xFFFFFF, "Player : %.2f,%.2f,%.2f", followPos.x, followPos.y, followPos.z);

	DrawCircle(pos3D.x + screenX/2, -pos3D.z , 50, 0xFF0000, false);

	auto& ins = InputManager::GetInstance();

	Vector2 mPos = ins.GetMousePos();

	//マウスの座標をワールド座標に変換
	VECTOR startPos = { mPos.x,mPos.y,0.0f };
	startPos = ConvScreenPosToWorldPos(startPos);


	VECTOR endPos = { pos2D.x,pos2D.y,pos2D.z };
	endPos = ConvScreenPosToWorldPos(endPos);



	//カメラの方向を固定する用
	VECTOR i = { 0.0f,0.0f,0.0f };
	Quaternion forward = Quaternion::Euler(i);

	//追従対象からカメラまでの相対座標
	VECTOR relativeCPos = forward.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	//カメラ位置の更新
	pos_ = VAdd(zero, relativeCPos);

	//カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = forward.PosAxis(RELATIVE_C2T_POS);

	//注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);

	//カメラの上方向
	cameraUp_ = forward.PosAxis(rot_.GetUp());
	//	}

	DrawLine3D(startPos, followPos, 0xff9999);


}

void Camera::Draw(void)
{
	DrawBox(Application::SCREEN_SIZE_X / 2 - 50, Application::SCREEN_SIZE_Y / 2 - 50,
		Application::SCREEN_SIZE_X / 2 + 50, Application::SCREEN_SIZE_Y / 2 + 50,
		0xFFFFFF,true);
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
	SetDefault();

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
	case Camera::MODE::FOLLOW_DELAY:
		break;
	}

}

const void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

//bool Camera::IsCollisionRect(Vector2 stPos1, Vector2 edPos1, Vector2 stPos2, Vector2 edPos2)
//{
//	if (stPos1.x < edPos2.x
//		&& stPos2.x < edPos1.x
//		&& stPos1.y < edPos2.y
//		&& stPos2.y < edPos1.y)
//	{
//		return true;
//	}
//
//	return false;
//}
//
//bool Camera::IsCollisionRectCenter(Vector2 centerPos1, Vector2 size1, Vector2 centerPos2, Vector2 size2)
//{
//	// 矩形1(左上座標、右上座標)
//	Vector2 stPos1 = centerPos1;
//	Vector2 edPos1 = centerPos1;
//	Vector2 hSize1 = { size1.x / 2, size1.y / 2 };
//
//	stPos1.x -= hSize1.x;
//	stPos1.y -= hSize1.y;
//	edPos1.x += hSize1.x;
//	edPos1.y += hSize1.y;
//
//	// 矩形２(左上座標、右上座標)
//	Vector2 stPos2 = centerPos2;
//	Vector2 edPos2 = centerPos2;
//	Vector2 hSize2 = { size2.x / 2, size2.y / 2 };
//
//	stPos2.x -= hSize2.x;
//	stPos2.y -= hSize2.y;
//	edPos2.x += hSize2.x;
//	edPos2.y += hSize2.y;
//
//	// 矩形同士の衝突判定
//	// 矩形１の左よりも、矩形２の右が大きい
//	// 矩形２の左よりも、矩形１の右が大きい
//	if (IsCollisionRect(stPos1, edPos1, stPos2, edPos2))
//	{
//		return true;
//	}
//	return false;
//}

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

void Camera::Shake(void)
{

}

void Camera::SetShake(float intensity, float duration)
{

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
