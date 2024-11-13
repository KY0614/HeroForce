#include <vector>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Manager/DataBank.h"
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

	SetDevice(DEVICE::NONE);

	color_ = 0xFF0000;	//赤

	num = 0;
}

void SelectScene::Update(void)
{
	//キーの設定
	KeyConfigSetting();

	//入力デバイスの取得
	//GetDevice();

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

	DrawDebug();

	//キーとコントローラー用
	switch (device_)
	{
	case SelectScene::DEVICE::NONE:
		DrawCircle(pos_.x, pos_.y, 20, 0x999999, true);
		break;	
	
	case SelectScene::DEVICE::KEY:
		DrawCircle(pos_.x, pos_.y, 20, 0xFF00FF, true);
		break;
	
	case SelectScene::DEVICE::PAD:
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
	DataBank& data = DataBank::GetInstance();

	//カーソル移動処理
	ProcessCursor();

#ifdef DEBUG_RECT
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		rc[i] = { 250,300,200,200 };
		rc[i].pos.x = (rc[i].pos.x * i) + 250;

		if (IsHitRect(rc[i], ins.GetMousePos(), 20) /*||
			IsHitRect(rc[i], ins.GetMousePos(), 20) ||
			IsHitRect(rc[i], ins.GetMousePos(), 20) ||
			IsHitRect(rc[i], ins.GetMousePos(), 20)*/)
		{
			rc[i].color_ = 0xFF9999;
			num = i + 1;
			//左クリック押下で役職選択へ
			if (ins.IsTrgMouseLeft())
			{
				data.Input(SceneManager::CNTL::KEYBOARD, i);
				num = i + 1;
				rc[i].color_ = 0xFF0000;
				ChangeSelect(SELECT::ROLE);
			}
		}
		else {
			rc[i].color_ = 0xFF0000;
		}

	}
#endif // DEBUG_RECT

	//if (GetJoinDevice() == DEVICE::KEY)
	//{
	//	SetDevice(DEVICE::KEY);
	//	//data.Input(SceneManager::CNTL::KEYBOARD, 1);
	//}
	//else
	//{
	//	SetDevice(DEVICE::PAD);
	//	//data.Input(SceneManager::CNTL::PAD, 1);
	//}

}

void SelectScene::RoleUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	
	//左クリック押下でゲームシーンへ
	if (ins.IsTrgMouseLeft())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

#ifdef DEBUG_RECT
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		rc[i] = { 250,300,200,200 };
		rc[i].pos.x = (rc[i].pos.x * i) + 250;

		if (IsHitRect(rc[i], ins.GetMousePos(), 20) /*||
			IsHitRect(rc[i], ins.GetMousePos(), 20) ||
			IsHitRect(rc[i], ins.GetMousePos(), 20) ||
			IsHitRect(rc[i], ins.GetMousePos(), 20)*/)
		{
			rc[i].color_ = 0x99FF99;
			num = i + 1;
			//左クリック押下で役職選択へ
			if (ins.IsTrgMouseLeft())
			{
				//data.Input(SceneManager::CNTL::KEYBOARD, i);
				num = i + 1;
				rc[i].color_ = 0xFF0000;
				ChangeSelect(SELECT::ROLE);
			}
		}
		else {
			rc[i].color_ = 0x00FF00;
		}

	}
#endif // DEBUG_RECT

	//カーソル移動処理
	ProcessCursor();
}

void SelectScene::DrawDebug(void)
{
	InputManager& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();

#ifdef DEBUG_RECT

	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		rc[i].Draw(rc[i].color_);
		DrawFormatString(rc[i].pos.x, rc[i].pos.y,
			0xFFFFFF, "%d", i + 1);
	}

#endif // DEBUG_RECT

	DrawString(0, 0, "select", 0xFFFFFF);
	DrawString(0, 800, "緑：パッド入力,ピンク：キー入力", 0xFFFFFF);
	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0xFFFFFF,
		"%d",
		key);
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 20, 0xFFFFFF, "number : %d", num);

	//マウス用
	DrawCircle(mPos.x, mPos.y, 20, 0x341685, true);

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

	// 何も押されてないときは移動しないように
	key = KEY_CONFIG::NONE;
	
	// 左スティックの横軸
	int leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;

	//縦軸
	int leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;

	if (ins.IsNew(KEY_INPUT_UP) ||
		ins.IsNew(KEY_INPUT_W)	)
	{
		key = KEY_CONFIG::UP;
		SetDevice(DEVICE::KEY);
	}	
	
	if (leftStickY_ < -1)
	{
		key = KEY_CONFIG::UP;
		SetDevice(DEVICE::PAD);
	}

	if (ins.IsNew(KEY_INPUT_DOWN) ||
		ins.IsNew(KEY_INPUT_S) )
	{
		key = KEY_CONFIG::DOWN;
		SetDevice(DEVICE::KEY);
	}	
	
	if (leftStickY_ > 1)
	{
		key = KEY_CONFIG::DOWN;
		SetDevice(DEVICE::PAD);
	}

	if (ins.IsNew(KEY_INPUT_LEFT) ||
		ins.IsNew(KEY_INPUT_A) ||
		leftStickX_ < -1)
	{
		key = KEY_CONFIG::LEFT;
	}	

	if (leftStickX_ < -1)
	{
		key = KEY_CONFIG::LEFT;
		SetDevice(DEVICE::PAD);
	}
	
	if (ins.IsNew(KEY_INPUT_RIGHT) ||
		ins.IsNew(KEY_INPUT_D))
	{
		key = KEY_CONFIG::RIGHT;
		SetDevice(DEVICE::KEY);
	}

	if (leftStickX_ > 1)
	{
		key = KEY_CONFIG::RIGHT;
		SetDevice(DEVICE::PAD);
	}
}

void SelectScene::ProcessCursor(void)
{
	//移動量
	float pow = 5.0f;

	switch (key)
	{
	case SelectScene::KEY_CONFIG::NONE:
		break;

	case SelectScene::KEY_CONFIG::UP:
		MoveCursor(pow, { 0.0f,-1.0f,0.0f });
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

void SelectScene::MoveCursor(float pow, VECTOR dir)
{
	pos_.x += dir.x * pow;
	pos_.y += dir.y * pow;
	pos_.z += dir.z * pow;
}

SelectScene::DEVICE SelectScene::GetDevice(void)
{
	//DEVICE currentDv = device_;
	//KEY_CONFIG currentKey = key;

	//if (currentDv != device_)
	//{
	//	device_ = currentDv;
	//}

	//if (GetJoypadInputState(DX_INPUT_PAD1) != 0 && CheckHitKeyAll() <= 0)
	//{
	//	device_ = DEVICE::PAD;
	//}
	//else if (CheckHitKeyAll() != 0) {
	//	device_ = DEVICE::KEY;
	//}

	return device_;
}

SelectScene::DEVICE SelectScene::GetJoinDevice(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_RETURN)	||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		return device_;
	}
}

void SelectScene::SetDevice(DEVICE device)
{
	device_ = device;
}

bool SelectScene::IsHitRect(Rect& rc, Vector2 pos, int r)
{
	auto diffX = pos.x - rc.pos.x;	//終点から始点を引く
	auto diffY = pos.y - rc.pos.y;	//終点から始点を引く
	if (fabsf(diffX) > r + rc.w / 2 ||
		fabsf(diffY) > r + rc.h / 2) {
		return false;
	}
	else {
		return true;
	}
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

void SelectScene::Rect::Draw(unsigned int color)
{
	DxLib::DrawBox(
		(int)Left(),
		(int)Top() ,
		(int)Right() ,
		(int)Bottom(), color, true);
}
