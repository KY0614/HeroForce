#pragma once
#include"../../../../Utility/AsoUtility.h"
#include"../../../Common/Transform.h"
#include"../Manager/Generic/SceneManager.h"
#include "../../../UnitBase.h"

class SelectCharacter : public UnitBase
{
public:

	//キャラクター関連
	static constexpr float ROT_SPEED = 0.5f;			//キャラクターの回転速度

	//キャラクターアニメーション関連
	static constexpr float ANIM_SPEED = 20.0f;			//キャラクターのアニメーション速度

	static constexpr float CHICKEN_SPEED = 40.0f;		//チキンのアニメーション速度

	static constexpr int IDLE_ANIM = 36;				//IDLEアニメーション番号

	static constexpr int KNIGHT_ANIM = 1;				//騎士用アニメーション番号

	static constexpr int AXE_ANIM = 2;					//斧使い用アニメーション番号

	static constexpr int MAGE_ANIM = 61;				//魔法使いアニメーション番号

	static constexpr int ARCHER_ANIM = 6;				//弓使いアニメーション番号

	static constexpr int CHICKEN_SWINGHAND_ANIM = 6;	//チキン用アニメーション番号

	//コンストラクタ
	SelectCharacter(void);

	//デストラクタ
	~SelectCharacter(void) = default;

	//解放
	virtual void Destroy(void);

	//基本処理の４つは仮想関数化するのでしっかりオーバーライドするように
	//初期化
	virtual void Init(void)override;
	//更新
	virtual void Update(void)override;
	//描画
	virtual void Draw(void)override;

	/// <summary>
	/// Transform配列の先頭の座標を取得(他の座標も同じなので)
	/// </summary>
	/// <returns>Transform配列の先頭の座標</returns>
	VECTOR GetFrontPos(void)const { return transArray_[0].pos; }

	/// <summary>
	/// チキンの座標を取得
	/// </summary>
	/// <returns>チキンの座標</returns>
	VECTOR GetChickenPos(void)const { return trans_.pos; };

	//セッター(設定用関数)---------------------------------------------------

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

	/// <summary>
	/// 一定時間ごとにアニメーションを変える
	/// </summary>
	void UpdateAnimCycle(void);

private:

	//役職
	int role_;

	//アニメーション変更時間(transArrayに合わせて配列)
	float animChangeTime_[SceneManager::PLAYER_NUM];

	/// <summary>
	/// 3Dモデルの初期化処理
	/// </summary>
	void Init3DModel(void);
};