#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Manager/InputManager.h"
#include "../../Utility/AsoUtility.h"
#include "PlayerShip.h"

PlayerShip::PlayerShip(void)
{
}

PlayerShip::~PlayerShip(void)
{
}

void PlayerShip::Init(void)
{
	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));
	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();

	transform_.quaRotLocal = Quaternion();

	transform_.Update();
}

void PlayerShip::Update(void)
{
	// 回転操作
	ProcessTurn();

	// 自機の前方移動処理

	// 移動処理 = 座標 + 移動量
	// 移動量　	= 方向 * スピード

	// 方向（前方方向）
	VECTOR dir = transform_.quaRot.GetForward();
	//VECTOR dir = transform_.GetForward();

	// 移動量
	VECTOR movePow = VScale(dir, SPEED_MOVE);

	// 移動
	transform_.pos = VAdd(transform_.pos, movePow);

	transform_.Update();
}

void PlayerShip::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void PlayerShip::Release(void)
{
}

const Transform& PlayerShip::GetTransform(void) const
{
	return transform_;
}

void PlayerShip::ProcessTurn(void)
{
	auto& ins = InputManager::GetInstance();

	// 十字キーを押して自機を回転させる
	if (ins.IsNew(KEY_INPUT_UP)) { Turn(SPEED_ROT_DEG_X, AsoUtility::AXIS_X); }
	if(ins.IsNew(KEY_INPUT_DOWN)){ Turn(-SPEED_ROT_DEG_X, AsoUtility::AXIS_X); }
	if(ins.IsNew(KEY_INPUT_RIGHT)){ Turn(SPEED_ROT_DEG_Y, AsoUtility::AXIS_Y); }
	if(ins.IsNew(KEY_INPUT_LEFT)){ Turn(-SPEED_ROT_DEG_Y, AsoUtility::AXIS_Y); }
}

void PlayerShip::Turn(float deg, VECTOR axis)
{
	// デグリーからラジアンへ変換
	float rad= AsoUtility::Deg2RadF(deg);

	// 回転量(オイラー角)と回転軸を使用して、クォータニオン回転を作る
	Quaternion rotPow = Quaternion::AngleAxis(rad, axis);

	// 自機を回転(合成)させる
	transform_.quaRot = transform_.quaRot.Mult(rotPow);
}
