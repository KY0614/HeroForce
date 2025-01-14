#pragma once
class Timer
{
public:

	static constexpr int TIME_MAX = 59;
	static constexpr int TIME_MIN = 0;

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

	bool isEnd_;

	int cnt_;	//カウンター

	void ReduceTime(void);

	Timer(void);
	~Timer(void);
};

