#pragma once
#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Common/Vector2.h"

class Cursor
{
public:

	//初期位置
	static constexpr int DEFAULT_POS_X = 150;
	static constexpr int DEFAULT_POS_Y = 150;

	//プレイヤー最大人数
	static constexpr int NUM_MAX = 4;

	//移動量
	static constexpr int MOVE_POW = 14;

	//半径
	static constexpr int RADIUS = 32;

	//角度
	static constexpr float ANGLE = 315.0f;

	//カーソルサイズ
	static constexpr float SCALE_RATE = Application::SCREEN_SIZE_X * 1.5f / Application::DEFA_SCREEN_SIZE_X;

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

	//コンストラクタ
	Cursor();

	//デストラクタ
	~Cursor() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="plNum"></param>プレイヤーナンバー
	/// <param name="img"></param>画像
	void Init(const int _plNum,const int _img);

	//更新
	void Update();

	//描画
	void Draw();

	//情報をリセットする
	void Reset();

	//操作関連の設定
	void SetControllKey(const int _right, const int _left, const int _up, const int _down, const int _decide);

	//決定の設定
	void SetDecide(const bool _value);

	//座標返す
	inline const Vector2 GetPos() const { return pos_; }

	//決定済みか確認
	inline const bool IsDecide() const { return decide_; }

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