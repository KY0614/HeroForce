#pragma once
#include <assert.h>
#include "../../../../Utility/AsoUtility.h"
#include "../../../Common/Transform.h"
#include "../../../../Manager/Generic/SceneManager.h"
#include "../../../../Manager/Generic/ResourceManager.h"
#include "../../../../Manager/GameSystem/DataBank.h"
#include "../PlayerBase.h"
#include "../../../UnitBase.h"

class OverPlayers : public UnitBase
{
public:

	//アニメーションスピード
	static constexpr float ANIM_SPEED = 20.0f;

	//アイドルアニメ番号
	static constexpr int IDLE_ANIM = 24;
	static constexpr int DEATH_ANIM = 23;

	//スケール
	static constexpr VECTOR SCALE = { 0.5f,0.5f,0.5f };

	//位置
	static constexpr VECTOR PLAYER_POS_1 = { -240.0f, -40.0f, -50.0f };
	static constexpr VECTOR PLAYER_POS_2 = { -50.0f, -50.0f, -10.0f };
	static constexpr VECTOR PLAYER_POS_3 = { 50.0f, -50.0f, -10.0f };
	static constexpr VECTOR PLAYER_POS_4 = { 240.0f, -50.0f, -50.0f };

	//向き
	static constexpr float PLAYER_ROT_1 = -30.0f;
	static constexpr float PLAYER_ROT_2 = -15.0f;
	static constexpr float PLAYER_ROT_3 = 15.0f;
	static constexpr float PLAYER_ROT_4 = 30.0f;

	//プレイヤー数
	static constexpr int PLAYERS = SceneManager::PLAYER_NUM;

	//コンストラクタ
	OverPlayers(void);

	//デストラクタ
	~OverPlayers(void) = default;

	virtual void Init(void)override;
	virtual void Update(void)override;
	virtual void Draw(void)override;

	//座標の設定
	void SetPos(VECTOR pos);

	//回転の設定
	void SetRot(Quaternion quo) { for (auto& tran : trans_) { tran.quaRotLocal = quo; } };

	//座標を渡す
	VECTOR GetPos(void) { for (auto& tran : transArray_) { return tran.pos; } };

	//配列の座標を渡す
	VECTOR GetPosArray(int i) { return transArray_[i].pos; };

private:

	//キャラごとのトランスフォーム
	Transform trans_[PLAYERS];

	//役職
	SceneManager::ROLE role_[PLAYERS];

	//3Dモデル初期化
	void Init3DModel(void);

	//アニメーション処理
	void Animation();

	//アニメーションの初期化
	void ResetAnim(const ANIM _anim, const float _speed);

	void FinishAnim()override;
};

