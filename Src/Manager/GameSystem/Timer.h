#pragma once
class Timer
{
	//a
public:

	//時間の上限・下限
	static constexpr int TIME_MAX = 59;
	static constexpr int TIME_MIN = 0;

	//フォントサイズ初期値
	static constexpr int TIME_FONT_SIZE = 50;

	//タイマー背景サイズ
	static constexpr float TIMER_BACK_RATE = 0.8f;
	static constexpr int TIMER_BACK_SIZE_X = 298 * TIMER_BACK_RATE;
	static constexpr int TIMER_BACK_SIZE_Y = 168 * TIMER_BACK_RATE;

	//数字大きさ
	static constexpr float NUM_RATE = 0.75f;

	//数字描画数
	static constexpr int NUM_CNT = 2;

	//数字の描画位置
	static constexpr int NUM_POS_X[NUM_CNT] = {
		Application::SCREEN_SIZE_X / 2 - 30,
		Application::SCREEN_SIZE_X / 2 + 30,
	};
	static constexpr int NUM_POS_Y = 64;

	void Update();
	void Draw();
	void Reset();	//デバッグ用
	void Release();
	void Destroy();

	const bool IsEnd(void) { return isEnd_; }

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static Timer& GetInstance(void);

private:

	static Timer* instance_;

	int minute_;	//分数
	int second_;	//秒数

	bool isEnd_;	//タイマー終了の知らせ

	int cnt_;	//カウンター

	//int font_;	//フォントハンドル
	//int fontSize_;	//フォントの大きさ
	//int strWidth_;	//文字列の横サイズ

	VECTOR pos_;	//位置

	int imgTimerBack_;	//背景
	int *imgNumbers_;	//数字画像
	
	void ReduceTime(void);

	Timer(void);
	~Timer(void);
};

