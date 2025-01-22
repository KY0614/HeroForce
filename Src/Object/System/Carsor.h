#pragma once
#include "../Common/Vector2.h"

class Carsor
{
public:

	static constexpr int MOVE_POW = 5;

	static constexpr int RADIUS = 10;

	//キー操作
	struct Key
	{
		int right_;
		int left_;
		int up_;
		int down_;

		int stickX_;
		int stickY_;
		int decide_;
	};
	// 初期化処理
	Carsor();
	~Carsor();

	void Init();
	void Update();
	void Draw();

	//初期化処理
	void Reset();

	//画像読み込み
	void Load();

	//操作関連の設定
	void SetContllorKey(const int right, const int left, const int up, const int down, const int decide);

	//座標返す
	Vector2 GetPos() const { return pos_; }

	bool IsDecide() const { return decide_; }


private:

	int img_;	//画像

	Vector2 pos_;	//座標

	bool decide_;	//決定

	Key key_;	//キー管理
};