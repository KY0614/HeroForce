#pragma once
#include "../Manager/InputManager.h"
#include "../Common/Vector2.h"

class Carsor
{
public:

	//初期位置
	static constexpr int DEFAULT_POS_X = 150;
	static constexpr int DEFAULT_POS_Y = 150;

	//プレイヤー最大人数
	static constexpr int NUM_MAX = 4;

	//移動量
	static constexpr int MOVE_POW = 5;

	//半径
	static constexpr int RADIUS = 32;

	//角度
	static constexpr float ANGLE = 315.0f;

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

	void Init(const int plNum,const int img);//プレイヤーナンバーと画像種類を受け取る
	void Update();
	void Draw();

	//初期化処理
	void Reset();

	//操作関連の設定
	void SetContllorKey(const int right, const int left, const int up, const int down, const int decide);

	//決定の設定
	void SetDecide(const bool value);

	//座標返す
	Vector2 GetPos() const { return pos_; }

	//決定済みか確認
	bool IsDecide() const { return decide_; }


private:

	//プレイヤーナンバー
	int playerNum_;

	//画像
	int img_;

	//拡大率
	float rate_;

	//座標
	Vector2 pos_;	

	//決定
	bool decide_;	

	//キー管理
	Key key_;	

	//パッド情報
	InputManager::JOYPAD_NO pad_;
};