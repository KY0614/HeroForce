#pragma once
#include"../../../../Utility/AsoUtility.h"
#include"../../../Common/Transform.h"
#include"../Manager/Generic/SceneManager.h"
#include "../../../UnitBase.h"

class SelectPlayer : public UnitBase
{
public:
	static constexpr float ROT_SPEED = 0.5f;

	static constexpr float ANIM_SPEED = 20.0f;
	static constexpr float CHICKEN_SPEED =40.0f;
	static constexpr int IDLE_ANIM = 36;
	static constexpr int KNIGHT_ANIM = 1;
	static constexpr int AXE_ANIM = 2;
	static constexpr int MAGE_ANIM = 61;
	static constexpr int ARCHER_ANIM = 6;
	static constexpr int SWING_ANIM = 6;

	//コンストラクタ
	SelectPlayer(void);

	//デストラクタ
	~SelectPlayer(void) = default;

	//解放
	virtual void Destroy(void);

	//基本処理の４つは仮想関数化するのでしっかりオーバーライドするように
	//初期化
	virtual void Init(void)override;
	//更新
	virtual void Update(void)override;
	//描画
	virtual void Draw(void)override;

	//セッター
	void SetRole(int role){ role_ = role; };

	/// <summary>
	/// 役職を設定する
	/// </summary>
	/// <param name="role">役職</param>
	void SetRole(int role){ role_ = role - 1; };

	/// <summary>
	/// 座標を設定する
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(VECTOR pos);

	/// <summary>
	/// 座標を設定する(チキン用)
	/// </summary>
	/// <param name="pos">座標</param>
	void SetChickenPos(VECTOR pos){trans_.pos = pos;};

	/// <summary>
	/// 角度(向き)を設定する
	/// </summary>
	/// <param name="quo">角度</param>
	void SetRot(Quaternion quo) { for (auto& tran : transArray_) { tran.quaRotLocal = quo; } };

	/// <summary>
	/// 角度(向き)を設定する(チキン用)
	/// </summary>
	/// <param name="quo">角度</param>
	void SetRotChicken(Quaternion quo) { trans_.quaRotLocal = quo;  };

	//
	VECTOR GetPos(void) { for (auto& tran : transArray_) { return tran.pos; } };
	VECTOR GetChickenPos(void) { return trans_.pos; };
	VECTOR GetPosArray(int i) { return transArray_[i].pos; };
	
	//アニメーションを変更する
	void ChangeAnim(void);

	/// <summary>
	/// 攻撃アニメーションを設定
	/// </summary>
	/// <param name="i">設定する配列の引数</param>
	void SetAtkAnim(int i);

	/// <summary>
	/// 通常アニメーションを設定
	/// </summary>
	/// <param name="i">設定する配列の引数</param>
	void SetIdleAnim(int i);

private:

	//役職
	int role_;

	//アニメーション変更時間
	float animChangeTime_[SceneManager::PLAYER_NUM];

	void Init3DModel(void);
};