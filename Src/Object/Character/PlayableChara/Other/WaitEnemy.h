#pragma once
#include "../Object/UnitBase.h"

class WaitEnemy : public UnitBase
{
public:
	static constexpr float ANIM_SPEED = 20.0f;	//アニメーション速度

	static constexpr int IDLE_ANIM = 41;		//IDLEアニメーション番号
	static constexpr int SPAWN_ANIM = 74;		//SPAWNアニメーション番号

	//コンストラクタ
	WaitEnemy(void);

	//デストラクタ
	~WaitEnemy(void) = default;

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
	/// 配列の要素を指定し、座標を設定する
	/// </summary>
	/// <param name="pos">設定する座標</param>
	/// <param name="index">指定する要素番号</param>
	void SetPosAtIndex(VECTOR pos, int index) { transArray_[index].pos = pos; };

	/// <summary>
	/// 角度(向き)を設定する
	/// </summary>
	/// <param name="quo">角度</param>
	void SetRot(Quaternion quo) { for (auto& tran : transArray_) { tran.quaRotLocal = quo; } };

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <param name="index">配列の要素の指定</param>
	/// <returns>指定した配列要素の座標</returns>
	VECTOR GetPosAtIndex(int index) const{ return transArray_[index].pos; };

private:

	//アニメーション変更時間
	float animChangeTime_[SceneManager::PLAYER_NUM];

	//スポーン中かどうか true:スポーン中 false:まだスポーンしていない
	bool isSpawn_[SceneManager::PLAYER_NUM];

	/// <summary>
	/// 3Dモデルの初期化処理
	/// </summary>
	void Init3DModel(void);
};

