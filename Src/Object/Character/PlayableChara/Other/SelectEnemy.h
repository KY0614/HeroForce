#pragma once
#include"../../../../Utility/AsoUtility.h"
#include"../../../Common/Transform.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Object/UnitBase.h"

class SelectEnemy : public UnitBase
{
public:
	static constexpr float ANIM_SPEED = 20.0f;	//アニメーション速度

	static constexpr int IDLE_ANIM = 41;		//IDLEアニメーション番号
	static constexpr int SPAWN_ANIM = 74;		//SPAWNアニメーション番号

	//コンストラクタ
	SelectEnemy(void);

	//デストラクタ
	~SelectEnemy(void) = default;

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
	/// 座標を設定する
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(VECTOR pos) { for (auto& tran : transArray_) { tran.pos = pos; } };
	void SetPosArray(VECTOR pos, int i) { transArray_[i].pos = pos; };

	/// <summary>
	/// 角度(向き)を設定する
	/// </summary>
	/// <param name="quo">角度</param>
	void SetRot(Quaternion quo) { for (auto& tran : transArray_) { tran.quaRotLocal = quo; } };

	//座標を取得
	VECTOR GetPos(void) { for (auto& tran : transArray_) { return tran.pos; } };

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <param name="i">配列の指定</param>
	/// <returns>指定した配列の座標</returns>
	VECTOR GetPosArray(int i) { return transArray_[i].pos; };

private:

	//アニメーション変更時間
	float animChangeTime_[SceneManager::PLAYER_NUM];
	bool isSpawn_[SceneManager::PLAYER_NUM];

	void Init3DModel(void);
};

