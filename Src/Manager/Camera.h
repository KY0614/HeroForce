#pragma once
#include <DxLib.h>
#include <cmath>
#include <DirectXMath.h>
#include "../Common/Quaternion.h"

class Transform;

class Camera
{

public:

	// カメラスピード：NEAR
	static constexpr float SPEED = 10.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// カメラ位置から注視点までの相対座標
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };

	// 追従対象からカメラ位置までの相対座標(完全追従)
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 25.0f, -80.0f };

	// 追従対象からカメラ位置までの相対座標(ばね付き)
	static constexpr VECTOR RELATIVE_F2C_POS_SPRING = { 0.0f, 40.0f, 150.0f };

	// 移動速度の最大値
	static constexpr float MAX_MOVE_SPEED = 3.5f;

	// 加速
	static constexpr float MOVE_ACC = 0.5;

	// 減速
	static constexpr float MOVE_DEC = 0.05f;


	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// 定点カメラ
		FREE,			// フリーモード
		FOLLOW,			// 追従モード
		FOLLOW_SPRING	// ばね付き追従モード
	};

	VECTOR Lerp(VECTOR start, VECTOR end, float t) {
		return VGet(
			start.x + (end.x - start.x) * t,
			start.y + (end.y - start.y) * t,
			start.z + (end.z - start.z) * t
		);
	}

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);

	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowSpring(void);

	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;

	// カメラモードの変更
	void ChangeMode(MODE mode);

	// 追従対象の設定
	void SetFollow(const Transform* follow);

private:

	// 追従対象
	const Transform* followTransform_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラの注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	// カメラの回転
	Quaternion rot_;

	// カメラの速度(移動量)
	VECTOR velocity_;

	// カメラを初期位置に戻す
	void SetDefault(void);

	//カメラシェイク
	void Shake(void);
	
	//カメラシェイクさせるための準備
	void SetShake(float intensity,float duration);

	float shakeIntensity; // 振動の強さ
	float shakeDuration;  // 振動の持続時間
	float shakeTime;      // 現在の振動時間
	

	// 移動操作
	void ProcessMove(void);

	// 移動
	void Move(void);

	// 加速
	void Accele(float speed);

	// 減速（スピードを減少させる）
	void Decelerate(float speed);

	float moveSpeed_;

	VECTOR moveDir;
};

