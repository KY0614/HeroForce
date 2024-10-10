#pragma once
#include<vector>
#include "UnitBase.h"

class Enemy : public UnitBase
{
public:

	static constexpr float ATK_CDT = 50.0f;	//攻撃のクールダウンの時間

	//現在状態
	enum class STATE
	{
		NML		//通常
		,ATK	//攻撃
		,DEAD	//死亡
		,MAX
	};

	//攻撃の種類
	enum class ATK_PAT
	{

	};

	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy() = default;

	//解放
	void Destroy(void)override;

	//初期化
	void Init(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="_damage">ダメージ量</param>
	//void Damage(float _damage);

private:
	
	STATE state_;	//現在の状態

	float atkCdt_;			//攻撃のクールダウンカウンタ

	std::vector<ATK> atk_;	//攻撃情報

	//更新(通常)
	void UpdateNml(void);
	//更新(攻撃)
	void UpdateAtk(void);
	//更新(死亡)
	void UpdateDead(void);

	//移動
	void Move(void);
	//移動処理
	void ProcessMove(const float _moveSpeed, const float _deg);
	//方向転換
	void Turn(const float _deg, const VECTOR _axis);

	//敵の攻撃処理
	void Attack(void);

	//攻撃の生成
	void CreateAtk(void);

	//有効な攻撃を取得
	const ATK GetValidAtk(void);
};

