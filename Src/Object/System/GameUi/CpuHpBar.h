#pragma once
#include "GamaUIBase.h"

class CpuHpBar : public GamaUIBase
{
public:

	//大きさ
	static constexpr VECTOR SIZE = { 58, 17, 0 };

	//相対座標
	static constexpr VECTOR LOCAL_HPBAR_POS = { 0, 5, 0 };

	//画像の拡大率
	static constexpr float IMG_RATE = 1.2f;

	//コンストラクタ
	CpuHpBar();

	//デストラクタ
	~CpuHpBar();

	//読み込み
	void Load()override;

	//描画
	void Draw()override;

private:


};

