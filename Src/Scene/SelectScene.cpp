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

	kPos_ = { Application::SCREEN_SIZE_X/2,Application::SCREEN_SIZE_Y / 2,0.0f };
	cPos_ = { Application::SCREEN_SIZE_X/2,Application::SCREEN_SIZE_Y / 2,0.0f };

	key = KEY_CONFIG::NONE;

	ChangeDevice(SceneManager::CNTL::KEYBOARD);

	color_ = 0xFF0000;	//赤

	num = 0;
}

void SelectScene::Update(void)
{
	//キーの設定
	KeyConfigSetting();

	//どちらかを操作しているときにもう片方を操作sできないように制御
	ControllKey();

	//入力デバイスの取得
	//GetDevice();

	//// シーン遷移
	//InputManager& ins = InputManager::GetInstance();
	//if (ins.IsTrgDown(KEYBORD_INPUT_SPACE))
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

	DrawCircle(kPos_.x, kPos_.y, 20, 0xFF00FF, true);
	DrawCircle(cPos_.x, cPos_.y, 20, 0x00FF00, true);

	//キーとコントローラー用
	//switch (device_)
	//{
	//case SelectScene::SceneManager::CNTL::NONE:
	//	break;	
	//
	//case SelectScene::SceneManager::CNTL::KEYBORD:
	//	DrawCircle(kPos_.x, kPos_.y, 20, 0xFF00FF, true);
	//	break;
	//
	//case SelectScene::SceneManager::CNTL::PAD:
	//	DrawCircle(cPos_.x, cPos_.y, 20, 0x00FF00, true);
	//	break;
	//
	//default:
	//	break;
	//}
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
		//四角形の大きさを決め、1個ずつ右にずらす
		rc[i] = { 250,300,200,200 };
		rc[i].pos.x = (rc[i].pos.x * i) + 250;

		//カーソルと四角形の当たり判定
		if (IsHitRect(rc[i], ins.GetMousePos(), 20))
		{
			rc[i].color_ = 0xFF9999;
			num = i + 1;
			//左クリック押下で役職選択へ
			if (ins.IsTrgMouseLeft())
			{
				//とりあえずキーボード
				data.Input(SceneManager::CNTL::KEYBOARD, i);
				//プレイヤー人数の設定
				data.Input(SceneManager::PLAY_MODE::USER, i);
				//CPU人数の設定(CPUは１人から３人)
				data.Input(SceneManager::PLAY_MODE::CPU, (SceneManager::PLAYER_NUM - 1) - i);

				//選択した数表示用
				num = i + 1;
				//押下したときの色
				rc[i].color_ = 0xFF0000;
				ChangeSelect(SELECT::ROLE);
			}
		}
		else {
			rc[i].color_ = 0xFF0000;
		}

	}
#endif // DEBUG_RECT

	//if (GetJoinDevice() == SceneManager::CNTL::KEYBORD)
	//{
	//	ChangeDevice(SceneManager::CNTL::KEYBORD);
	//	//data.Input(SceneManager::CNTL::KEYBOARD, 1);
	//}
	//else
	//{
	//	ChangeDevice(SceneManager::CNTL::PAD);
	//	//data.Input(SceneManager::CNTL::PAD, 1);
	//}

}

void SelectScene::RoleUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DataBank& data = DataBank::GetInstance();
	
#ifdef DEBUG_RECT
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		rc[i] = { 250,300,200,200 };
		rc[i].pos.x = (rc[i].pos.x * i) + 250;

		if (IsHitRect(rc[i], ins.GetMousePos(), 20) )
		{
			rc[i].color_ = 0x99FF99;
			role = i + 1;
			//左クリック押下で役職選択へ
			if (ins.IsTrgMouseLeft())
			{
				//iは役職の種類
				data.Input(static_cast<SceneManager::ROLE>(i), 1);

				role = i + 1;
				rc[i].color_ = 0xFF0000;

				//ゲームシーンへ
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
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
	DrawFormatString(0, 820, 0xFFFFFF, "(key:0)(pad:1) %d", device_);
	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0xFFFFFF,
		"%d",
		key);
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 20, 0xFFFFFF, "number : %d", num);
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 40, 0xFFFFFF, "role : %d", role);

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

	switch (GetDevice())
	{
	case SceneManager::CNTL::KEYBOARD:
		if (ins.IsNew(KEY_INPUT_UP) ||
			ins.IsNew(KEY_INPUT_W))
		{
			key = KEY_CONFIG::UP;
			ChangeDevice(SceneManager::CNTL::KEYBOARD);
		}
		if (ins.IsNew(KEY_INPUT_DOWN) ||
			ins.IsNew(KEY_INPUT_S))
		{
			key = KEY_CONFIG::DOWN;
			ChangeDevice(SceneManager::CNTL::KEYBOARD);
		}
		if (ins.IsNew(KEY_INPUT_LEFT) ||
			ins.IsNew(KEY_INPUT_A))
		{
			key = KEY_CONFIG::LEFT;
			ChangeDevice(SceneManager::CNTL::KEYBOARD);
		}
		if (ins.IsNew(KEY_INPUT_RIGHT) ||
			ins.IsNew(KEY_INPUT_D))
		{
			key = KEY_CONFIG::RIGHT;
			ChangeDevice(SceneManager::CNTL::KEYBOARD);
		}
		break;

	case SceneManager::CNTL::PAD:
		ChangeDevice(SceneManager::CNTL::PAD);
		if (leftStickY_ < -1)
		{
			key = KEY_CONFIG::UP;
			
		}
		if (leftStickY_ > 1)
		{
			key = KEY_CONFIG::DOWN;
			//ChangeDevice(SceneManager::CNTL::PAD);
		}
		if (leftStickX_ < -1)
		{
			key = KEY_CONFIG::LEFT;
			//ChangeDevice(SceneManager::CNTL::PAD);
		}
		if (leftStickX_ > 1)
		{
			key = KEY_CONFIG::RIGHT;
			//ChangeDevice(SceneManager::CNTL::PAD);
		}
		break;
	default:
		break;
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
	if (device_ == SceneManager::CNTL::PAD) {
		cPos_.x += dir.x * pow;
		cPos_.y += dir.y * pow;
		cPos_.z += dir.z * pow;
	}
	else{
		kPos_.x += dir.x * pow;
		kPos_.y += dir.y * pow;
		kPos_.z += dir.z * pow;
	}
}

SceneManager::CNTL SelectScene::GetDevice(void)
{
	//DEVICE currentDv = device_;
	//KEYBORD_CONFIG currentKey = key;

	//if (currentDv != device_)
	//{
	//	device_ = currentDv;
	//}

	//if (GetJoypadInputState(DX_INPUT_PAD1) != 0 && CheckHitKeyAll() <= 0)
	//{
	//	device_ = SceneManager::CNTL::PAD;
	//}
	//else if (CheckHitKeyAll() != 0) {
	//	device_ = SceneManager::CNTL::KEYBORD;
	//}

	return device_;
}

bool SelectScene::GetJoinDevice(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		return device_ == SceneManager::CNTL::PAD;
	}	
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		return device_ == SceneManager::CNTL::KEYBOARD;
	}


}

void SelectScene::ChangeDevice(SceneManager::CNTL device)
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

void SelectScene::ControllKey(void)
{
	InputManager& ins = InputManager::GetInstance();
	int key = CheckHitKeyAll(DX_CHECKINPUT_KEY);		//キー入力を調べる(マウス、パッド以外)
	int padNum = GetJoypadNum();
	int padState = CheckHitKeyAll(DX_CHECKINPUT_PAD);	//キー入力を調べる(マウス、キーボード以外)

	//キーボード操作の時パッド操作をできないように
	if (key != 0	&&
		padState == 0)
	{
		ChangeDevice(SceneManager::CNTL::KEYBOARD);
	}//パッド操作の時キー操作をできないように
	else if (key <= 0 &&
		padNum > 0 &&
		padState != 0)
	{
		ChangeDevice(SceneManager::CNTL::PAD);
	}
}

void SelectScene::Load(void)
{

}

void SelectScene::Rect::Draw(unsigned int color)
{
	DxLib::DrawBox(
		(int)Left(),
		(int)Top() ,
		(int)Right() ,
		(int)Bottom(), color, true);
}
