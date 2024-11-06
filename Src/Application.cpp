#include <DxLib.h>
#include "Manager/ResourceManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

//�t�@�C���w��p�X
const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_ENEMY = "Data/Model/Enemy/";
const std::string Application::PATH_PLAYER = "Data/Model/Player/";
const std::string Application::PATH_STAGE = "Data/Model/Stage/";
const std::string Application::PATH_OBJECT = "Data/Model/Object/";
const std::string Application::PATH_EFFECT = "Data/Effect/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// �A�v���P�[�V�����̏����ݒ�
	SetWindowText("3DShooting");

	// �E�B���h�E�T�C�Y
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLib�̏�����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// �L�[���䏉����
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// ���\�[�X�Ǘ�������
	ResourceManager::CreateInstance();

	// �V�[���Ǘ�������
	SceneManager::CreateInstance();

	//FPS�p������
	currentFrame_ = 0;
	lastFrame_ = 0;
}

void Application::Run(void)
{

	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		Sleep(1);	//�V�X�e���ɏ�����Ԃ�
		currentFrame_ = GetNowCount();	//���݂̃t���[�������l��

		//���݂̃t���[���ƍŌ�̎��s�t���[���̍��������l�𒴂�����X�V�������s���B
		if (currentFrame_ - lastFrame_ >= frameRate)
		{
			lastFrame_ = currentFrame_;	//�t���[���̍X�V
			InputManager::GetInstance().Update();
			sceneManager.Update();	//�X�V
		}

		sceneManager.Draw();

		ScreenFlip();

	}

}

void Application::Destroy(void)
{

	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();
	
	// DxLib�I��
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}
