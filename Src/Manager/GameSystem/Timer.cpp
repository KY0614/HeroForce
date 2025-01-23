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
	//FPS�Q�Ƃ̃J�E���g�X�V
	//FPS�Ɠ����l�̂Ƃ��͈�b�������Ƃ�������
	if (cnt_ >= Application::DEFAULT_FPS) {
		cnt_ = 0;

		//���Ԑ��������炷
		ReduceTime();
	}
}

void Timer::Draw()
{
	DrawFormatString(0, 50, 0xff0000, "�c��[%d:%d]", minute_, second_);
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
	//�b�������炷
	second_--;

	//�b�����O�����ɂȂ�����
	if (second_ < TIME_MIN) {
		//���������炷
		minute_--;
		//�b�����T�X�b��
		second_ = TIME_MAX;
	}

	//�������O�����̂Ƃ��I����Ԃ�
	if (minute_ <= TIME_MIN && second_ <= TIME_MIN)isEnd_ = true;
}