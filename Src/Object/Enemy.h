#pragma once
#include "UnitBase.h"

class Enemy :
    public UnitBase
{
public:

	//コンストラクタ


	//解放
	void Destroy(void)override;

	//初期化
	void Init(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;

};

