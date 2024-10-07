#pragma once
#include"Common/Transform.h"

class Enemy;

class EnemyAtk
{
public:
	
	static constexpr float ATK_DURATION = 10.0f;	//攻撃の持続時間

	//コンストラクタ
	EnemyAtk();
	
	//デストラクタ
	~EnemyAtk() = default;

	//解放
	void Destroy(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_parent">親のインスタンス</param>
	void Init(Enemy* _parent);
	//更新
	void Update(void);
	//描画
	void Draw(void);

	//生存判定(true:生存中)
	const bool IsAlive(void) const { return atkDuration_ < ATK_DURATION; };

private:

	Enemy* enemy_;	//親参照用

	Transform trans_;	//位置情報関係
	float atkDuration_;	//攻撃の持続時間カウンタ
};

