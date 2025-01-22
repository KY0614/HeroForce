#pragma once
class Timer
{
public:

	//時間の上限・下限
	static constexpr int TIME_MAX = 59;
	static constexpr int TIME_MIN = 0;

	//フォントサイズ初期値
	static constexpr int TIME_FONT_SIZE = 50;

	void Update();
	void Draw();
	void Reset();	//デバッグ用

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

	int font_;	//フォントハンドル
	int fontSize_;	//フォントの大きさ
	int strWidth_;	//文字列の横サイズ

	VECTOR pos_;	//位置
	
	void ReduceTime(void);

	Timer(void);
	~Timer(void);
};

