#pragma once
class Timer
{
public:

	//���Ԃ̏���E����
	static constexpr int TIME_MAX = 59;
	static constexpr int TIME_MIN = 0;

	//�t�H���g�T�C�Y�����l
	static constexpr int TIME_FONT_SIZE = 50;

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

	bool isEnd_;	//�^�C�}�[�I���̒m�点

	int cnt_;	//�J�E���^�[

	int font_;	//�t�H���g�n���h��
	int fontSize_;	//�t�H���g�̑傫��
	int strWidth_;	//������̉��T�C�Y

	VECTOR pos_;	//�ʒu
	
	void ReduceTime(void);

	Timer(void);
	~Timer(void);
};

