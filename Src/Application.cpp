#include <DxLib.h>
#include "Manager/ResourceManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

//ファイル指定パス
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

	// アプリケーションの初期設定
	SetWindowText("3DShooting");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// リソース管理初期化
	ResourceManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

	//FPS用初期化
	currentFrame_ = 0;
	lastFrame_ = 0;
}

void Application::Run(void)
{

	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		Sleep(1);	//システムに処理を返す
		currentFrame_ = GetNowCount();	//現在のフレーム数を獲得

		//現在のフレームと最後の実行フレームの差分が一定値を超えたら更新処理を行う。
		if (currentFrame_ - lastFrame_ >= frameRate)
		{
			lastFrame_ = currentFrame_;	//フレームの更新
			InputManager::GetInstance().Update();
			sceneManager.Update();	//更新
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
	
	// DxLib終了
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
