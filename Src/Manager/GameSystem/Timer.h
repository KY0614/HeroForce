#pragma once
class Timer
{
public:

	void Update();
	void Draw();

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static Timer& GetInstance(void);

	void Reset();

	bool IsEnd();

private:

	static Timer* instance_;

	int minute_;	//����
	int second_;	//�b��

	Timer(void);
	~Timer(void);
};

