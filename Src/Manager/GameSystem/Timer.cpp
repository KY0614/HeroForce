#include<DxLib.h>
#include"../Application.h"
#include "Timer.h"

Timer* Timer::instance_ = nullptr;
//a

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
	//FPSQÆ‚ÌƒJƒEƒ“ƒgXV
	//FPS‚Æ“¯‚¶’l‚Ì‚Æ‚«‚Íˆê•b—§‚Á‚½‚Æ‚¢‚¤‚±‚Æ
	if (cnt_ >= Application::DEFAULT_FPS) {
		cnt_ = 0;

		//ŠÔ§ŒÀ‚ğŒ¸‚ç‚·
		ReduceTime();
	}
}

void Timer::Draw()
{
	DrawFormatString(0, 50, 0xff0000, "c‚è[%d:%d]", minute_, second_);
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
	//•b”‚ğŒ¸‚ç‚·
	second_--;

	//•b”‚ª‚O–¢–‚É‚È‚Á‚½
	if (second_ < TIME_MIN) {
		//•ª”‚ğŒ¸‚ç‚·
		minute_--;
		//•b”‚ğ‚T‚X•b‚É
		second_ = TIME_MAX;
	}

	//•ª”‚ª‚O–¢–‚Ì‚Æ‚«I—¹ó‘Ô‚É
	if (minute_ <= TIME_MIN && second_ <= TIME_MIN)isEnd_ = true;
}