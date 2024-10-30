#pragma once
#include <vector>
#include"../Common/Transform.h"


class StageObject;

class StageBase
{
public:

	// ステージモデル数
	static constexpr int STAGE1_MODELS = 7;

	//コンストラクタ
	StageBase(void);
	//デストラクタ
	~StageBase(void);
	//解放
	virtual void Destroy(void);

	//基本処理の４つは仮想関数化するのでしっかりオーバーライドするように
	//初期化
	virtual void Init(void);
	//更新
	virtual void Update(void);
	//描画
	virtual void Draw(void);
	//解放
	virtual void Release(void);

	void ModelLoad();

	void JsonLoad();

protected:

	std::vector<int> mdlId_;			//モデル保存
	Transform trans_;					//位置情報関係

	std::vector<StageObject*> objs_;

};

