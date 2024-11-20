#include <vector>
#include<algorithm>
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

	kPos_ = { Application::SCREEN_SIZE_X/2,Application::SCREEN_SIZE_Y / 2 };
	cPos_ = { Application::SCREEN_SIZE_X/2,Application::SCREEN_SIZE_Y / 2 };

	key = KEY_CONFIG::NONE;

	ChangeDevice(SceneManager::CNTL::KEYBOARD);

	color_ = 0xFF0000;	//赤

	playerNum_ = 0;

	num = 0;
	opr = 0;
	role = 0;

	//デバッグ用------------------------------------------------

	triL.color_ = GetColor(255, 255, 64);
	triR.color_ = GetColor(255, 255, 64);

	triL = { 450,450 ,150,150 ,false };
	triR = { 1050,450 ,150,150 ,false };
}

void SelectScene::Update(void)
{
	playerNum_ = std::clamp(playerNum_, 0, 4);

	//キーの設定
	KeyConfigSetting();

	//どちらかを操作しているときにもう片方を操作できないように制御
	ControllKey();

	//選択中の種類ごとの更新処理
	switch (select_)
	{
	case SELECT::NUMBER:  
		NumberUpdate();
		break;

	case SELECT::OPERATION:
		OperationUpdate();
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

	//デバッグ描画
	DrawDebug();
}

void SelectScene::Release(void)
{
}

void SelectScene::NumberUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DataBank& data = DataBank::GetInstance();

	//カーソル移動処理
	//ProcessCursor();

#ifdef DEBUG_RECT
	int PRI_SPACE = 100;
	int RECT_SCALE = 300;
	int SCALE = 150;
	int PRI_W = 100;
	int plusCol = 100;

	//四角形の座標と大きさ、色を決める
	rc = { 750,450,RECT_SCALE,RECT_SCALE };


	triL.pos.x = rc.pos.x - SCALE - PRI_SPACE;
	triR.pos.x = rc.pos.x + SCALE + PRI_SPACE;

	rc.color_ = GetColor(255, 0, 0);

	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);

	if (triR.isToggle_ &&
		ins.IsTrgDown(KEY_INPUT_RIGHT))
	{
		triR.color_ = GetColor(255, 255, 255);
		playerNum_++;
	}
	if (triL.isToggle_ &&
		ins.IsTrgDown(KEY_INPUT_LEFT))
	{
		triR.color_ = GetColor(255, 255, 255);
		playerNum_--;
	}

	//カーソルと四角形の当たり判定
	if (ins.IsTrgDown(KEY_INPUT_RIGHT)	&&
		!triR.isToggle_)
	{
		num += playerNum_ + 1;
		triR.isToggle_ = true;
		triL.isToggle_ = false;

		//左クリック押下で役職選択へ
		if (key == KEY_CONFIG::DECIDE)
		{
			//プレイヤー人数の設定
			data.Input(SceneManager::PLAY_MODE::USER, 1);

			//CPU人数の設定(CPUは１人から３人)
			data.Input(SceneManager::PLAY_MODE::CPU, (SceneManager::PLAYER_NUM - 1) - 1);

			//選択した数表示用
			num = playerNum_ + 1;
			playerNum_ = playerNum_;

			//押下したときの色
			rc.color_ = 0xFF0000;
			ChangeSelect(SELECT::OPERATION);
		}
	}
	
	if (ins.IsTrgDown(KEY_INPUT_LEFT) &&
		!triL.isToggle_)
	{
		triR.isToggle_ = false;
		triL.isToggle_ = true;
	}

#endif // DEBUG_RECT

}

void SelectScene::OperationUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DataBank& data = DataBank::GetInstance();

	//カーソル移動処理
	ProcessCursor();

#ifdef DEBUG_RECT
	int RECT_SPACE = 150;
	//for (int i = 0; i < 2; i++)
	//{
	//	//四角形の座標と大きさを決め、1個ずつ右にずらす
	//	rc[i] = { 500,450,200,200 };
	//	rc[i].pos.x += ((RECT_SPACE + rc[i].w) * i);

	//	//カーソルと四角形の当たり判定
	//	if (IsHitRect(rc[i], ins.GetMousePos(), 20))
	//	{
	//		rc[i].color_ = 0xFF9999;
	//		opr = i + 1;
	//		//左クリック押下で役職選択へ
	//		if (ins.IsTrgMouseLeft())
	//		{
	//			//1Pだけ操作方法変更可能
	//			data.Input(static_cast<SceneManager::CNTL>(i), 0);
	//			ChangeSelect(SELECT::ROLE);
	//			opr = i + 1;
	//		}
	//	}
	//	else {
	//		rc[i].color_ = 0xFF0000;
	//	}

	//}
#endif // DEBUG_RECT

}

void SelectScene::RoleUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DataBank& data = DataBank::GetInstance();
	
#ifdef DEBUG_RECT
	int RECT_SPACE = 100;
	//for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	//{
	//	//四角形の座標と大きさを決め、1個ずつ右にずらす
	//	rc[i] = { 350,450,200,200 };
	//	rc[i].pos.x += ((RECT_SPACE + rc[i].w) * i);

	//	if (IsHitRect(rc[i], ins.GetMousePos(), 20) )
	//	{
	//		rc[i].color_ = 0x99FF99;
	//		role = i + 1;
	//		//左クリック押下で役職選択へ
	//		if (ins.IsTrgMouseLeft())
	//		{
	//			//iは役職の種類
	//			data.Input(static_cast<SceneManager::ROLE>(i), 1);

	//			role = i + 1;
	//			rc[i].color_ = 0xFF0000;

	//			//ゲームシーンへ
	//			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	//		}
	//	}
	//	else {
	//		rc[i].color_ = 0x00FF00;
	//	}

	//}
#endif // DEBUG_RECT

	//カーソル移動処理
	ProcessCursor();
}

void SelectScene::DrawDebug(void)
{
	InputManager& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();

#ifdef DEBUG_RECT
	//選択用の四角形と選択している種類または数字を表示する

	switch (select_)
	{
	case SelectScene::SELECT::NUMBER:
		rc.Draw(rc.color_);

			rc.Draw(rc.color_);
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "%d", playerNum_ + 1);

			DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200, 0,
				0xFF9999, "プレイ人数選択中");
		
		break;

	case SelectScene::SELECT::OPERATION:

			rc.Draw(rc.color_);
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "key");
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "pad");

			DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200, 0,
				0xFF9999, "1P操作方法選択中");
		
		break;

	case SelectScene::SELECT::ROLE:
		for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
		{
			rc.Draw(rc.color_);
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "KNIGHT");
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "AXEMAN");
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "MAGE");
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "ARCHER");

			DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200, 0,
				0x99FF99, "役職選択中");
		}
		break;

	default:
		break;
	}
#endif // DEBUG_RECT

	DrawString(0, 800, "緑：パッド入力,ピンク：キー入力", 0xFFFFFF);
	DrawFormatString(0, 820, 0xFFFFFF, "(key:0)(pad:1) %d", device_);
	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0xFFFFFF,
		"%d",
		key);

	DrawFormatString(Application::SCREEN_SIZE_X / 2, 0, 0xFFFFFF, "number : %d", num);
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 20, 0xFFFFFF, "operation : %d", opr);
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 40, 0xFFFFFF, "role : %d", role);

	DrawFormatString(Application::SCREEN_SIZE_X / 2, 60, 0xFFFFFF, "L : %d", triL.isToggle_);
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 80, 0xFFFFFF, "R : %d", triR.isToggle_);

	triL.LeftDraw(triL.color_);
	triR.RightDraw(triR.color_);

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
		//キーの押下判定
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

		if (ins.IsTrgDown(KEY_INPUT_SPACE))
		{
			key = KEY_CONFIG::DECIDE;
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
		}
		if (leftStickX_ < -1)
		{
			key = KEY_CONFIG::LEFT;
		}
		if (leftStickX_ > 1)
		{
			key = KEY_CONFIG::RIGHT;
		}
		if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_BUTTON))
		{
			key = KEY_CONFIG::DECIDE;
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
	}
	else{
		kPos_.x += dir.x * pow;
		kPos_.y += dir.y * pow;
	}
}

SceneManager::CNTL SelectScene::GetDevice(void)
{
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

void SelectScene::Tri::LeftDraw(unsigned int color)
{
	DxLib::DrawTriangle(
		(int)LeftX_L(),
		(int)LeftY_L(),
		(int)TopX_L(),
		(int)TopY_L(),
		(int)RightX_L(),
		(int)RightY_L(),
		 color, true);
}

void SelectScene::Tri::RightDraw(unsigned int color)
{
	DxLib::DrawTriangle(
		(int)LeftX_R(),
		(int)LeftY_R(),
		(int)TopX_R(),
		(int)TopY_R(),
		(int)RightX_R(),
		(int)RightY_R(),
		color, true);
}