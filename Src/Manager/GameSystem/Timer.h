#pragma once
class Timer
{
public:

	void Update();
	void Draw();

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static Timer& GetInstance(void);

	void Reset();

	bool IsEnd();

private:

	static Timer* instance_;

	int minute_;	//分数
	int second_;	//秒数

	Timer(void);
	~Timer(void);
};

