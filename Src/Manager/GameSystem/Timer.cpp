#include<DxLib.h>
#include "Timer.h"

Timer* Timer::instance_ = nullptr;

Timer::Timer(void)
{
	minute_ = 0;
	second_ = 0;
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

void Timer::Reset()
{
}

bool Timer::IsEnd()
{
	return false;
}

void Timer::Update()
{
}

void Timer::Draw()
{
	DrawFormatString(0, 0, 0xffffff, "Žc‚è[%d:%d]", minute_, second_);
}


