#pragma once
#include <DxLib.h>
#include <cmath>
#include <DirectXMath.h>
#include "../Common/Quaternion.h"

class Transform;

class Camera
{

public:

	//カメラスピード：NEAR
	static constexpr float SPEED = 10.0f;

	//カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	//カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	//カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	//カメラ位置から注視点までの相対座標
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -400.0f, 500.0f };
//	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };

	//追従対象からカメラ位置までの相対座標(完全追従)
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 500.0f, -500.0f };
//	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 25.0f, -80.0f };

	//追従対象からカメラ位置までの相対座標(ばね付き)
	static constexpr VECTOR RELATIVE_F2C_POS_SPRING = { 0.0f, 40.0f, 150.0f };

	//移動速度の最大値
	static constexpr float MAX_MOVE_SPEED = 5.0f;

	//加速
	static constexpr float MOVE_ACC = 0.5f;

	//減速
	static constexpr float MOVE_DEC = 0.2f;

	// シェイク：時間
	static constexpr float TIME_SHAKE = 1.0f;

	// シェイク：幅
	static constexpr float WIDTH_SHAKE = 3.0f;

	// シェイク：スピード
	static constexpr float SPEED_SHAKE = 30.0f;

	//カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	//定点カメラ
		FREE,			//フリーモード
		FOLLOW,			//追従モード
		FOLLOW_SPRING,	//ばね付き追従モード
		SHAKE			// カメラ揺らし
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);

	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowSpring(void);
	void SetBeforeDrawShake(void);

	void SetBeforeDrawFollowDelay(void);

	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;

	//カメラモードの変更
	void ChangeMode(MODE mode);

	//追従対象の設定
	const void SetFollow(const Transform* follow);

	bool IsCheckFollow(VECTOR centerPos1, VECTOR size1, VECTOR centerPos2, VECTOR size2);

private:

	//追従対象
	const Transform* followTransform_;

	//カメラモード
	MODE mode_;
	MODE currentMode_;

	//カメラの位置
	VECTOR pos_;

	//カメラの注視点
	VECTOR targetPos_;

	//カメラの上方向
	VECTOR cameraUp_;

	//カメラの回転
	Quaternion rot_;

	//カメラの速度(移動量)
	VECTOR velocity_;

	//カメラを初期位置に戻す
	void SetDefault(void);

	//カメラシェイク
	void Shake(void);
	
	//カメラシェイクさせるための準備
	void SetShake(float intensity,float duration);

	// 画面揺らし用
	float stepShake_;

	VECTOR defaultPos_;

	VECTOR shakeDir_;
	

	//移動操作
	void ProcessMove(void);

	//移動
	void Move(void);

	//加速
	void Accele(float speed);

	//減速（スピードを減少させる）
	void Decelerate(float speed);

	// 移動量
	float moveSpeed_;

	//向き
	VECTOR moveDir;
};

