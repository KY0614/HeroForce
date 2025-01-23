#pragma once
class Timer
{
	//a
public:

	static constexpr int TIME_MAX = 59;
	static constexpr int TIME_MIN = 0;

	void Update();
	void Draw();
	void Reset();	//�f�o�b�O�p

	const bool IsEnd(void) { return isEnd_; }

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static Timer& GetInstance(void);

private:

	static Timer* instance_;

	int minute_;	//����
	int second_;	//�b��

	bool isEnd_;

	int cnt_;	//�J�E���^�[

	void ReduceTime(void);

	Timer(void);
	~Timer(void);
};

