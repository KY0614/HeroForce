#pragma once
#include <vector>
#include <map>
#include <string>
#include"../Common/Transform.h"


class StageObject;

class StageBase
{
public:

	//衝突判定用定数
	static constexpr float ROCK01_COLI_RADIUS = 50.0f;	//岩1
	static constexpr float ROCK02_COLI_RADIUS = 50.0f;	//岩2
	static constexpr float STUMP_COLI_RADIUS = 40.0f;	//切り株
	static constexpr float TREE_COLI_RADIUS = 40.0f;	//木々

	enum class MODEL_TYPE
	{
		BUSH,
		FLOWWERS,
		ROCK_01,
		ROCK_02,
		STUMP,
		TERRAIN,
		TREE,
		MAX
	};

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

	//モデル読み込み
	void ModelLoad();

	//JSON読み込み
	void JsonLoad();

	//デバッグ用描画
	void DebugDraw();

	//ゲッター
	std::vector<Transform>
		GetTtans(MODEL_TYPE type);

	float GetRadius(MODEL_TYPE type);

protected:

	std::vector<int> mdlId_;			//モデル保存
	std::vector<Transform> trans_;		//位置情報関係

	std::vector<StageObject*> objs_;

	std::map < MODEL_TYPE, std::vector<StageObject*>> stageMdls_;

};

