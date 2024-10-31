#pragma once
#include <vector>
#include "../Common/Vector2.h"

class Carsor;

class PowerSelect
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

	//強化要素の種類
	enum class TYPE
	{
		ATTACK,		//攻撃力の上昇
		DEFENSE,	//防御力の上昇
		SPEED,		//移動速度の上昇
		LIFE,		//最大体力の上昇
		EVENT1,		//パラメータ以外の効果用(例:ボス戦時ボスを毒状態にするなど)
		MAX
	};

	//強化要素数
	static constexpr int POWER_ELEMENT = 4;

	PowerSelect();
	~PowerSelect();

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

private:

	//選択用画像
	int img_;

	//強化要素数
	int element_;

	//強化項目
	struct Element
	{
		//拡大率
		float scl_;

		//座標
		Vector2 pos_;

		//種類
		TYPE type_;
	};

	//衝突判定

	//エレメント
	std::vector<Element> ele_;

	//状態
	STATE state_;

	//カーソル
	std::vector<Carsor> carsors_;

};

