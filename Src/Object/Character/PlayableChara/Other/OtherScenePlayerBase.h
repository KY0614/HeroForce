#pragma once
#include <Dxlib.h>
#include "../../../../Manager/Generic/SceneManager.h"
#include "../../../UnitBase.h"

class OtherScenePlayerBase : public UnitBase
{
public:

	//アニメーションスピード
	static constexpr float ANIM_SPEED = 20.0f;

	//アイドルアニメ番号
	static constexpr int IDLE_ANIM = 61;

	//スケール
	static constexpr VECTOR SCALE = { 0.5f,0.5f,0.5f };

	//プレイヤー数
	static constexpr int PLAYERS = SceneManager::PLAYER_NUM;

	//位置
	static constexpr VECTOR PLAYER_POS[PLAYERS] = {
		{-240.0f, -40.0f, -50.0f},
		{ -50.0f, -50.0f, -10.0f },
		{ 50.0f, -50.0f, -10.0f },
		{ 240.0f, -50.0f, -50.0f } };

	//向き
	static constexpr float PLAYER_ROT[PLAYERS] = {
		-30.0f,
		-15.0f,
		15.0f,
		30.0f };

	//コンストラクタ
	OtherScenePlayerBase();

	//デストラクタ
	~OtherScenePlayerBase() = default;

	void Init()override;
	void Update()override;
	void Draw()override;

protected:

	/// <summary>
	/// アニメーションの初期化
	/// </summary>
	/// <param name="_anim"></param>アニメーション種類
	/// <param name="_speed"></param>アニメーションスピード
	void ResetAnim(const ANIM _anim, const float _speed) override;

private:

	//キャラごとのトランスフォーム
	Transform trans_[PLAYERS];

	//役職
	SceneManager::ROLE role_[PLAYERS];

	//アニメーションの初期化
	virtual void InitAnim() = 0;

	//3Dモデル初期化
	void Init3DModel();

	//アニメーション処理
	void Animation();
};