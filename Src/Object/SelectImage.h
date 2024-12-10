#pragma once
#include"../Utility/AsoUtility.h"
#include"Common/Transform.h"
#include "./UnitBase.h"

class SelectImage : public UnitBase
{
public:
	//コンストラクタ
	SelectImage();

	//デストラクタ
	~SelectImage() = default;

	//解放
	virtual void Destroy(void);

	//基本処理の４つは仮想関数化するのでしっかりオーバーライドするように
	//初期化
	virtual void Init(void)override;
	//更新
	virtual void Update(void)override;
	//描画
	virtual void Draw(void)override;

private:
	
	int playerNumImg_;
	int leftPointImg_;
	int rightPointImg_;



	void Load(void);
};

