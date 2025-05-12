#pragma once
#include "GamaUIBase.h"

class CpuHpBar : public GamaUIBase
{
public:

	static constexpr VECTOR SIZE = { 58, 17, 0 };
	static constexpr VECTOR LOCAL_HPBAR_POS = { 0, 5, 0 };
	static constexpr float IMG_RATE = 1.2f;

	CpuHpBar();
	~CpuHpBar();

	void Load()override;
	void SetParam() override;
	void Draw()override;

private:


};

