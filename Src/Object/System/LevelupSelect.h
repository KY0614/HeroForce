#pragma once
#include <vector>
#include "../Common/Vector2.h"
#include "LevelScreenManager.h"

class Carsor;

class LevelupSelect
{
public:

	//状態
	enum class STATE
	{
		NONE,
		EXPANSION,
		SELECT,
		FIN, 
	};

	//選択項目数
	static constexpr int SELECT_ELEMENT = 4;

	LevelupSelect();
	~LevelupSelect();

	void Init();
	void Update();
	void Draw();

	//変数の初期化
	void Reset();

	//読み込み
	void Load();

	//初期座標の設定
	void SetFirstPos();

	//状態の設定
	void SetState(const STATE state);

	//状態の受け取り
	STATE GetState() const { return state_; }

	//種類の受け取り
	inline LevelScreenManager::TYPE GetType() { return selectType_; }
	
	inline LevelScreenManager::TYPE GetRandType();

private:

	//選択用画像
	int img_;

	//強化要素数
	int element_;

	LevelScreenManager::TYPE selectType_;

	//強化項目
	struct Element
	{
		//拡大率
		float scl_;

		//座標
		Vector2 pos_;

		//種類
		LevelScreenManager::TYPE type_;
	};

	//エレメント
	std::vector<Element> ele_;

	//状態
	STATE state_;

	//カーソル
	std::vector<Carsor> carsors_;

};

