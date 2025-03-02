#include<DxLib.h>
#include"../Application.h"
#include "Timer.h"

Timer* Timer::instance_ = nullptr;

Timer::Timer(void)
{
	minute_ = 1;
	second_ = 0;
	cnt_ = 0;
	isEnd_ = false;
}

Timer::~Timer(void)
{
}

void Timer::CreateInstance(void)
{
	if (instance_ != nullptr)return;
	instance_ = new Timer();
}

Timer& Timer::GetInstance(void)
{
	return *instance_;
}

void Timer::Update()
{
	cnt_++;
	//FPS参照のカウント更新
	//FPSと同じ値のときは一秒立ったということ
	if (cnt_ >= Application::DEFAULT_FPS) {
		cnt_ = 0;

		//時間制限を減らす
		ReduceTime();
	}
}

void Timer::Draw()
{
	DrawFormatString(0, 50, 0xff0000, "残り[%d:%d]", minute_, second_);
}

void Timer::Reset()
{
	minute_ = 1;
	second_ = 0;
	cnt_ = 0;
	isEnd_ = false;
}


void Timer::ReduceTime(void)
{
	//秒数を減らす
	second_--;

	//秒数が０未満になった時
	if (second_ < TIME_MIN) {
		//分数を減らす
		minute_--;
		//秒数を５９秒に
		second_ = TIME_MAX;
	}

	//分数が０未満のとき終了状態に
	if (minute_ <= TIME_MIN && second_ <= TIME_MIN)isEnd_ = true;
}