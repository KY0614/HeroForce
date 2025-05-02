#pragma once
#include "GamaUIBase.h"

class Enemy;

class EnemyHpBar : public GamaUIBase
{

public:

	
	static constexpr VECTOR LOCAL_HPBAR_POS = { 0, 30, 0 };
	static constexpr float IMG_RATE = 0.8f;
	static constexpr VECTOR SIZE = { 58* IMG_RATE - 7, 17* IMG_RATE - 2, 0 };

	EnemyHpBar();
	~EnemyHpBar() = default;

	void Load()override;
	void SetParam() override;
	void Draw(Enemy& enemy);

private:


};

