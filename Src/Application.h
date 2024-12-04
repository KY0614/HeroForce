#pragma once
#include <string>

class Application
{

public:

	// �X�N���[���T�C�Y
	static constexpr int SCREEN_SIZE_X = 800;
	/*static constexpr int SCREEN_SIZE_X = 1920;
	static constexpr int SCREEN_SIZE_Y = 1080;*/
	static constexpr int SCREEN_SIZE_Y = 600;

	//FPS
	static constexpr int DEFAULT_FPS = 60;
	static constexpr float frameRate = 1000 / 60;

	// �f�[�^�p�X�֘A
	//-------------------------------------------
	static const std::string PATH_IMAGE;
	static const std::string PATH_MODEL;
	static const std::string PATH_ENEMY;
	static const std::string PATH_PLAYER;
	static const std::string PATH_STAGE;
	static const std::string PATH_OBJECT;
	static const std::string PATH_EFFECT;
	static const std::string PATH_TEXT;
	static const std::string PATH_FONT;
	//-------------------------------------------

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static Application& GetInstance(void);

	// ������
	void Init(void);

	// �Q�[�����[�v�̊J�n
	void Run(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	// �����������^���s�̔���
	bool IsInitFail(void) const;

	// ��������^���s�̔���
	bool IsReleaseFail(void) const;

private:

	//�t���[���Œ�p
	int currentFrame_;	//���݂̃t���[����ۑ�
	int lastFrame_;		//�Ō�Ɏ��s�����t���[����ۑ�

	// �ÓI�C���X�^���X
	static Application* instance_;

	// ���������s
	bool isInitFail_;

	// ������s
	bool isReleaseFail_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	Application(void);

	// �R�s�[�R���X�g���N�^�����l
	Application(const Application&);

	// �f�X�g���N�^�����l
	~Application(void) = default;

};