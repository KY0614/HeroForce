#pragma once
#include "GamaUIBase.h"

class Enemy;

class EnemyHpBar : public GamaUIBase
{

public:

	//相対座標
	static constexpr VECTOR LOCAL_HPBAR_POS = { 0, 30, 0 };

	//画像拡大率
	static constexpr float IMG_RATE = 0.8f;

	//大きさ
	static constexpr VECTOR SIZE = { 58* IMG_RATE - 7, 17* IMG_RATE - 2, 0 };

	//位置調整用
	static constexpr int HP_BAR_OFFSET_TOP = 3;
	static constexpr int HP_BAR_OFFSET_BOTTOM = 2;

	//コンストラクタ
	EnemyHpBar();

	//デストラクタ
	~EnemyHpBar() = default;

	//リソースの読み込み
	void Load()override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="enemy"></param>敵の情報
	void Draw(Enemy& _enemy);
};

