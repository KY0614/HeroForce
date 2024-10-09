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
	void UpdateNml(void);
	void UpdateAtk(void);
	void UpdateDead(void);

	//描画
	void Draw(void)override;

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="_damage">ダメージ量</param>
	//void Damage(float _damage);

	//敵の攻撃の生存判定
	bool IsAtk(void);

private:
	
	STATE state_;	//現在の状態

	float atkCdt_;						//攻撃のクールダウンカウンタ

	std::vector<Transform> atkTrans_;	//攻撃の位置依存関係

	//移動
	void Move(const float _moveSpeed);

	//敵の攻撃処理
	void Attack(void);
};

