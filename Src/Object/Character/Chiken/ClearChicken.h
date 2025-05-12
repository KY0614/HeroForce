#pragma once
#include "ChickenBase.h"
class ClearChicken : public ChickenBase
{
public:

	//生存用体力
	static constexpr int LIFE = 999;

	//コンストラクタ
	ClearChicken();

	//デストラクタ
	~ClearChicken() = default;

private:
	
	//パラメータの設定
	void SetParam()override;
};

