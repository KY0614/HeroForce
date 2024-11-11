#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "SelectScene.h"

SelectScene::SelectScene(void)
{

}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	Load();

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	//人数選択から
	ChangeSelect(SELECT::NUMBER);

	KeyConfigSetting();

	pos_ = { Application::SCREEN_SIZE_X/2,Application::SCREEN_SIZE_Y / 2,0.0f };

	key = KEY_CONFIG::NONE;
}

void SelectScene::Update(void)
{
	KeyConfigSetting();

	//// シーン遷移
	//InputManager& ins = InputManager::GetInstance();
	//if (ins.IsTrgDown(KEY_INPUT_SPACE))
	//{
	//	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	//}

	//選択中の種類ごとの更新処理
	switch (select_)
	{
	case SELECT::NUMBER:
		NumberUpdate();
		break;

	case SELECT::ROLE:
		RoleUpdate();
		break;

	default:
		break;
	}
}

void SelectScene::Draw(void)
{
	auto& ins = InputManager::GetInstance();
	InputManager::JOYPAD_IN_STATE pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	Vector2 mPos = ins.GetMousePos();

	DrawDebug();

	//マウス用
	DrawCircle(mPos.x, mPos.y, 20, 0xFF0000, true);

	//キーとコントローラー用
	switch (ctrl_)
	{
	case SelectScene::CONTROL::KEY:
		DrawCircle(pos_.x, pos_.y, 20, 0xFF00FF, true);
		break;
	
	case SelectScene::CONTROL::PAD:
		DrawCircle(pos_.x, pos_.y, 20, 0x00FF00, true);
		break;
	
	default:
		break;
	}
}

void SelectScene::Release(void)
{
}

void SelectScene::NumberUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsTrgMouseLeft())
	{
		ChangeSelect(SELECT::ROLE);
	}

	float pow = 5.0f;

	switch (key)
	{
	case SelectScene::KEY_CONFIG::NONE:
		break;

	case SelectScene::KEY_CONFIG::UP:
		MoveCursor(pow,{ 0.0f,-1.0f,0.0f });
		break;

	case SelectScene::KEY_CONFIG::DOWN:
		MoveCursor(pow, { 0.0f,1.0f,0.0f });
		break;

	case SelectScene::KEY_CONFIG::LEFT:
		MoveCursor(pow, { -1.0f,0.0f,0.0f });
		break;

	case SelectScene::KEY_CONFIG::RIGHT:
		MoveCursor(pow, { 1.0f,0.0f,0.0f });
		break;

	case SelectScene::KEY_CONFIG::DECIDE:
		break;

	default:
		break;
	}

}

void SelectScene::RoleUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsTrgMouseLeft())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
}

void SelectScene::DrawDebug(void)
{

	DrawString(0, 0, "select", 0xFFFFFF);
	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0xFFFFFF,
		"%d",
		key);

	switch (select_)
	{
	case SelectScene::SELECT::NUMBER:
		DrawString(Application::SCREEN_SIZE_X / 2, 0, "number", 0xFFFFFF);
		break;
	case SelectScene::SELECT::ROLE:
		DrawString(Application::SCREEN_SIZE_X / 2, 0, "role", 0xFFFFFF);
		break;
	default:
		break;
	}
}

void SelectScene::ChangeSelect(SELECT select)
{
	select_ = select;
}

void SelectScene::ProcessSelect(void)
{
}

void SelectScene::KeyConfigSetting(void)
{
	auto& ins = InputManager::GetInstance();
	InputManager::JOYPAD_NO no;

	if (GetJoypadInputState(DX_INPUT_PAD1) && CheckHitKeyAll() == 0)
	{
		ctrl_ = CONTROL::PAD;
	}
	else if(CheckHitKeyAll() != 0) {
		ctrl_ = CONTROL::KEY;
	}

	key = KEY_CONFIG::NONE;

	// 左スティックの横軸
	int leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;

	//縦軸
	int leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;

	if (ins.IsNew(KEY_INPUT_UP) ||
		ins.IsNew(KEY_INPUT_W)	||
		leftStickY_ < -1)
	{
		key = KEY_CONFIG::UP;
	}

	if (ins.IsNew(KEY_INPUT_DOWN) ||
		ins.IsNew(KEY_INPUT_S) ||
		leftStickY_ > 1)
	{
		key = KEY_CONFIG::DOWN;
	}

	if (ins.IsNew(KEY_INPUT_LEFT) ||
		ins.IsNew(KEY_INPUT_A) ||
		leftStickX_ < -1)
	{
		key = KEY_CONFIG::LEFT;
	}

	if (ins.IsNew(KEY_INPUT_RIGHT) ||
		ins.IsNew(KEY_INPUT_D) ||
		leftStickX_ > 1)
	{
		key = KEY_CONFIG::RIGHT;
	}
}

void SelectScene::MoveCursor(float pow, VECTOR dir)
{
	pos_.x += dir.x * pow;
	pos_.y += dir.y * pow;
	pos_.z += dir.z * pow;
}

void SelectScene::Load(void)
{
	//画像読み込み
	img_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SELECT_UI).handleId_;
	if (img_ == -1)
	{
		return;
	}
}