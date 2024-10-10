#pragma once
#include"Common/Transform.h"


//test

class UnitBase
{
public:
	//コンストラクタ
	UnitBase(void);
	//デストラクタ
	~UnitBase(void);
	//解放
	virtual void Destroy(void);

	//基本処理の４つは仮想関数化するのでしっかりオーバーライドするように
	//初期化
	virtual void Init(void);
	//更新
	virtual void Update(void);
	//描画
	virtual void Draw(void);

	//ゲッター各種
	//生存確認(生存しているとtrue)
	const bool IsAlive(void)const {return hp_ > 0;};
	//位置
	const VECTOR GetPos(void)const {return trans_.pos;};
	//角度
	const VECTOR GetRot(void)const {return trans_.rot;};
	//大きさ
	const VECTOR GetScl(void)const {return trans_.scl;};
	//防御力
	const float GetDef(void)const {return def_;};

protected:

	int mdlId_;			//モデル保存
	int hp_;			//体力
	Transform trans_;	//位置情報関係
	float def_;			//防御力

};

