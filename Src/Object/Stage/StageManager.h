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

	//モデル種類
	enum class MODEL_TYPE
	{
		STAGE,	//本体
		DECO,	//装飾(衝突判定は行わない)
		MAX
	};

	//大きさ
	static constexpr VECTOR SCALE = { 1.0f,1.0f,1.0f };

	//初期位置
	static constexpr VECTOR DEFAULT_POS = { 0.0f,-120.0f,0.0f };
	
	// ステージモデル数
	static constexpr int MODELS = static_cast<int>(MODEL_TYPE::MAX);

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

	//ステージのトランスフォームを返す
	const Transform &GetTtans() const{ return trans_[static_cast<int>(MODEL_TYPE::STAGE)]; }	

protected:

	//トランスフォーム
	Transform trans_[MODELS];
	
	//デバッグ用描画
	void DebugDraw();

};

