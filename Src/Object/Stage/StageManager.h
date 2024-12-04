#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include"../Common/Transform.h"


class StageObject;

class StageManager
{
public:

	//衝突判定の仕方
	enum class HIT_TYPE
	{
		NONE,
		SPHERE,
		CAPSULE,
		MODEL,
	};

	//カプセル座標用
	enum class LR
	{
		L,
		R
	};

	//モデル種類
	enum class MODEL_TYPE
	{
		BARREL,
		BENCH,
		FENCES_LONG,
		FENCES_SHORT,
		GROUND,
		HOUSE_01,
		HOUSE_02,
		HOUSE_03,
		HOUSE_04,
		HOUSE_05,
		HUNGER_01,
		HUNGER_02,
		ROCK_01,
		ROCK_02,
		ROCK_03,
		SACK,
		TABLE,
		TREE_01,
		TREE_02,
		TREE_03,
		TREE_04,
		TREE_05,
		WAGON,
		WELL,
		WOOD,
		MAX
	};
	
	// ステージモデル数
	static constexpr int MODELS = static_cast<int>(MODEL_TYPE::MAX);

	//衝突判定用定数
	//球体で判定するオブジェクト
	static constexpr VECTOR BARREL_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };				//樽
	static constexpr VECTOR BENCH_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };				//ベンチ
	static constexpr VECTOR FENCES_LONG_COLI_RADIUS = { 30.0f, 0.0f, 3200.0f };			//フェンスロング
	static constexpr VECTOR FENCES_SHORT_COLI_RADIUS = { 30.0f, 0.0f, 1450.0f };		//フェンスショート
	static constexpr VECTOR HOUSE_01_COLI_RADIUS = { 470.0f, 0.0f, 260.0f };			//家1
	static constexpr VECTOR HOUSE_02_COLI_RADIUS = { 700.0f, 0.0f, 50.0f };				//家2
	static constexpr VECTOR HOUSE_03_COLI_RADIUS = { 700.0f, 0.0f, 50.0f };				//家3
	static constexpr VECTOR HOUSE_04_COLI_RADIUS = { 380.0f, 0.0f, 200.0f };			//家4
	static constexpr VECTOR HOUSE_05_COLI_RADIUS = { 580.0f, 0.0f, 100.0f };			//家5
	static constexpr VECTOR HUNGER_01_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };			//物干し1
	static constexpr VECTOR HUNGER_02_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };			//物干し2
	static constexpr VECTOR ROCK_01_COLI_RADIUS = { 140.0f, 0.0f, 0.0f };				//岩1
	static constexpr VECTOR ROCK_02_COLI_RADIUS = { 140.0f, 0.0f, 0.0f };				//岩2
	static constexpr VECTOR ROCK_03_COLI_RADIUS = { 140.0f, 0.0f, 0.0f };				//岩3
	static constexpr VECTOR SACK_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };				//包袋
	static constexpr VECTOR TABLE_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };				//机
	static constexpr VECTOR TREE_01_COLI_RADIUS = { 60.0f, 0.0f, 0.0f };				//木1
	static constexpr VECTOR TREE_02_COLI_RADIUS = { 60.0f, 0.0f, 0.0f };				//木2
	static constexpr VECTOR TREE_03_COLI_RADIUS = { 60.0f, 0.0f, 0.0f };				//木3
	static constexpr VECTOR TREE_04_COLI_RADIUS = { 120.0f, 0.0f, 0.0f };				//木4
	static constexpr VECTOR TREE_05_COLI_RADIUS = { 120.0f, 0.0f, 0.0f };				//木5
	static constexpr VECTOR WAGON_COLI_RADIUS = { 100.0f, 0.0f,30.0f };					//荷車
	static constexpr VECTOR WELL_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };				//井戸
	static constexpr VECTOR WOOD_COLI_RADIUS = { 100.0f, 0.0f, 50.0f };					//丸太

	//コンストラクタ
	StageManager(void);

	//デストラクタ
	~StageManager(void);

	//解放
	virtual void Destroy(void);

	//基本処理の４つは仮想関数化するのでしっかりオーバーライドするように
	virtual void Init(void);	//初期化
	virtual void Update(void);	//更新
	virtual void Draw(void);	//描画
	virtual void Release(void);	//解放

	//読み込むJSONファイルを選ぶ
	void SetJsonFile();

	//モデル読み込み
	void ModelLoad();

	//JSON読み込み
	void JsonLoad();

	//デバッグ用描画
	void DebugDraw();

	//ゲッター
	std::vector<Transform>GetTtans(MODEL_TYPE type);								//オブジェクトごとのトランスフォーム
	float GetRadius(MODEL_TYPE type);												//衝突判定用半径
	VECTOR GetHitCapsulePos(VECTOR pos, MODEL_TYPE type, Transform trans, LR lr);	//カプセル座標(LR:左右のどちらか指定)

protected:

	//JSONファイル
	std::string jsonFile_;

	//JSON名前
	std::vector<std::string> names_;

	//衝突判定用半径
	std::vector<VECTOR> radius_;

	//モデル保存
	std::vector<int> mdlId_;

	//衝突判定方法
	std::vector<HIT_TYPE> hitTypes_;	

	std::vector<StageObject*> objs_;
	std::map < MODEL_TYPE, std::vector<StageObject*>> stageMdls_;

};

