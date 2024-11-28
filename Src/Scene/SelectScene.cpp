#include <vector>
#include <math.h>
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
	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	//人数選択から
	ChangeSelect(SELECT::NUMBER);

	KeyConfigSetting();

	key_ = KEY_CONFIG::NONE;

	ChangeDevice(SceneManager::CNTL::KEYBOARD);

	playerNum_ = 1;

	isPad_ = false;

	role_ = 0;

	//図形用------------------------------------------------

	//三角形の中心座標と大きさ
	triL = { 450,TRI_POS_Y ,TRI_SCALE,TRI_SCALE ,false };
	triR = { 1050,TRI_POS_Y ,TRI_SCALE,TRI_SCALE ,false };

	//三角形の色
	triL.color_ = GetColor(255, 255, 64);
	triR.color_ = GetColor(255, 255, 64);

	//四角形の中心座標と大きさ
	rc = { 750,450,RECT_SCALE,RECT_SCALE };

	//四角形の色
	rc.color_ = GetColor(255, 0, 0);

	//三角形の描画座標
	triL.pos.x = rc.pos.x - TRI_SCALE - PRI_SPACE;
	triR.pos.x = rc.pos.x + TRI_SCALE + PRI_SPACE;

	//------------------------------------------------------

	keyPressTime_ = 0.0f;
	interval_ = 0.0f;
}

void SelectScene::Update(void)
{
	//キーの設定
	KeyConfigSetting();

	//どちらかを操作しているときにもう片方を操作できないように制御
	ControllDevice();

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
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

#ifdef DEBUG_RECT
	int PRI_SPACE = 100;
	int RECT_SCALE = 300;
	int TRI_SCALE = 150;

	////四角形の座標と大きさ、色を決める
	//rc = { 750,450,RECT_SCALE,RECT_SCALE };
	//rc.color_ = GetColor(255, 0, 0);

	//三角形の描画座標
	triL.pos.x = rc.pos.x - TRI_SCALE - PRI_SPACE;
	triR.pos.x = rc.pos.x + TRI_SCALE + PRI_SPACE;

	//三角形のボタンを選択中だったら緑に非選択だったら黄色に
	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);

	//選択中に同じ方向のキーをもう一回押すと人数を加算
	if (triR.isToggle_&&
		GetKeyConfig() == KEY_CONFIG::RIGHT_TRG)
	{
		//人数を１追加
		playerNum_ += 1;
	}

	//右の三角形がONの時にキーの右に値する入力をし続けると
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		//色を白に
		triR.color_ = GetColor(255, 255, 255);

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME )
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f ,playerNum_ += 1: interval_;
		}
	}
	else if(triR.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
	}

	//選択中に同じ方向のキーをもう一回押すと人数を減算
	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT_TRG)
	{
		//人数を１削減
		playerNum_ -= 1;
	}

	//左
	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		//色を白に
		triL.color_ = GetColor(255, 255, 255);

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, playerNum_ -= 1 : interval_;
		}
	}
	else if (triL.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
	}

	//プレイ人数の範囲内に数値を収める
	playerNum_ = std::clamp(playerNum_, 1, SceneManager::PLAYER_NUM );	//１～４人プレイなので1～4まで

	//スペースキー押下で決定&入力デバイス選択へ
	if (key_ == KEY_CONFIG::DECIDE)
	{
		//プレイヤー人数の設定
		data.Input(SceneManager::PLAY_MODE::USER, playerNum_);

		//CPU人数の設定(CPUは１人から３人)
		data.Input(SceneManager::PLAY_MODE::CPU, (SceneManager::PLAYER_NUM) - playerNum_);

		//押下したときの色
		rc.color_ = 0xFF0000;
		ChangeSelect(SELECT::OPERATION);
	}

	//選択する三角形
	if (!triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		triR.isToggle_ = true;
		triL.isToggle_ = false;
	}

	if (!triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
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
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

#ifdef DEBUG_RECT
	int PRI_SPACE = 100;
	int RECT_SCALE = 300;
	int TRI_SCALE = 150;

	//四角形の座標と大きさ、色を決める
	rc = { 750,450,RECT_SCALE,RECT_SCALE };
	rc.color_ = GetColor(255, 0, 0);

	//三角形の描画座標
	triL.pos.x = rc.pos.x - TRI_SCALE - PRI_SPACE;
	triR.pos.x = rc.pos.x + TRI_SCALE + PRI_SPACE;

	//三角形のボタンが選択中だったら緑に非選択だったら黄色に
	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);


	//選択中に同じ方向のキーをもう一回押すと選択デバイスを変更
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT_TRG)
	{
		//キーボードを選択
		isPad_ = true;
	}

	//右の三角形がONの時にキーの右に値する入力をし続けると
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		//色を白に
		triR.color_ = GetColor(255, 255, 255);

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, isPad_ = true : interval_;
		}
	}
	else if (triR.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
	}

	//選択中に同じ方向のキーをもう一回押すと選択デバイスを変更
	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT_TRG)
	{
		//キーボードを選択
		isPad_ = false;
	}

	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		//色を白に
		triL.color_ = GetColor(255, 255, 255);

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, isPad_ = false : interval_;
		}
	}
	else if (triL.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
	}

	//スペースキー押下で決定&役職選択へ
	if (GetKeyConfig() == KEY_CONFIG::DECIDE)
	{
		//1Pの操作の設定
		(isPad_) ? data.Input(SceneManager::CNTL::PAD, 1) : data.Input(SceneManager::CNTL::KEYBOARD, 1);
		selectedCntl_ = (isPad_) ? SceneManager::CNTL::PAD : SceneManager::CNTL::KEYBOARD;
		
		//押下したときの色
		rc.color_ = 0xFF0000;

		ChangeSelect(SELECT::ROLE);
	}

	//選択する三角形
	if (!triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		triR.isToggle_ = true;
		triL.isToggle_ = false;
	}

	if (!triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		triR.isToggle_ = false;
		triL.isToggle_ = true;
	}

#endif // DEBUG_RECT

}

void SelectScene::RoleUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

#ifdef DEBUG_RECT
	int PRI_SPACE = 100;
	int RECT_SCALE = 300;
	int TRI_SCALE = 150;

	//四角形の座標と大きさ、色を決める
	rc = { 750,450,RECT_SCALE,RECT_SCALE };
	rc.color_ = GetColor(255, 0, 0);

	//三角形の描画座標
	triL.pos.x = rc.pos.x - TRI_SCALE - PRI_SPACE;
	triR.pos.x = rc.pos.x + TRI_SCALE + PRI_SPACE;

	//三角形のボタンが選択中だったら緑に非選択だったら黄色に
	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);

	//選択中に同じ方向のキーをもう一回押すと選択役職を変更
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT_TRG)
	{
		//役職を選択
		role_ += 1;
	}

	//右の三角形がONの時にキーの右に値する入力をし続けると
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		//色を白に
		triR.color_ = GetColor(255, 255, 255);

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, role_ += 1 : interval_;
		}
	}
	else if (triR.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
	}

	//選択中に同じ方向のキーをもう一回押すと選択役職を変更
	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT_TRG)
	{
		//役職を選択
		role_ -= 1;
	}

	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		//色を白に
		triL.color_ = GetColor(255, 255, 255);

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, role_ -= 1 : interval_;
		}
	}
	else if (triL.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
	}

	//役職数の範囲内に数値を収める
	role_ = std::clamp(role_, 0, static_cast<int>(SceneManager::ROLE::ARCHER) + 1);	//４役職なので0～3まで

	//スペースキー押下でゲーム画面へ
	if (GetKeyConfig() == KEY_CONFIG::DECIDE)
	{
		//役職の設定
		data.Input(static_cast<SceneManager::ROLE>(role_),playerNum_);

		//押下したときの色
		rc.color_ = 0xFF0000;

		ChangeSelect(SELECT::ROLE);
	}


	//選択する三角形
	if (!triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		triR.isToggle_ = true;
		triL.isToggle_ = false;
	}

	if (!triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		triR.isToggle_ = false;
		triL.isToggle_ = true;
	}

#endif // DEBUG_RECT

}

void SelectScene::DrawDebug(void)
{
	InputManager& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();

#ifdef DEBUG_RECT
	//選択用の四角形と選択している種類または数字を表示する
	rc.Draw(rc.color_);

	switch (select_)
	{
	case SelectScene::SELECT::NUMBER:

		DrawFormatString(rc.pos.x, rc.pos.y,
			0xFFFFFF, "%d", playerNum_);

		DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200, 0,
			0xFF9999, "プレイ人数選択中");

		DrawFormatString(0, 0,
			0xFFFFFF, "time : %.2f", keyPressTime_);

		break;

	case SelectScene::SELECT::OPERATION:
		if (!isPad_)
		{
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "key");
		}
		else
		{
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "pad");
		}

		DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200, 0,
			0xFF9999, "1P操作方法選択中");

		break;

	case SelectScene::SELECT::ROLE:
		if (role_ > static_cast<int>(SceneManager::ROLE::MAGE))
		{
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "ARCHER");
		}
		else if (role_ > static_cast<int>(SceneManager::ROLE::AXEMAN))
		{
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "MAGE");
		}
		else if (role_ > static_cast<int>(SceneManager::ROLE::KNIGHT))
		{
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "AXEMAN");
		}
		else 
		{
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "KNIGHT");
		}

		DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200, 0,
			0x99FF99, "役職選択中");

		break;

	default:
		break;
	}
#endif // DEBUG_RECT

	//現在の入力デバイス
	DrawFormatString(0, 820, 0xFFFFFF, "(key_:0)(pad:1) %d", GetDevice());
	//入力の種類
	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0xFFFFFF,
		"%d",
		key_);

	//プレイ人数
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 0, 0xFFFFFF, "number : %d", playerNum_);
	//1Pかパッド操作かどうか
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 20, 0xFFFFFF, "operation : %d", isPad_);
	//役職
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 40, 0xFFFFFF, "role_ : %d", role_);

	// 左スティックの横軸
	int leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;
	//縦軸
	int leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;

	DrawFormatString(Application::SCREEN_SIZE_X / 2, 60, 0xFFFFFF, "stickX : %d", leftStickX_);
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 80, 0xFFFFFF, "stickY : %d", leftStickY_);


	//三角形描画
	triL.LeftDraw(triL.color_);
	triR.RightDraw(triR.color_);
}

void SelectScene::ChangeSelect(SELECT select)
{
	select_ = select;
}

void SelectScene::KeyConfigSetting(void)
{
	auto& ins = InputManager::GetInstance();

	// 何も押されてないときは移動しないように
	key_ = KEY_CONFIG::NONE;
	
	// 左スティックの横軸
	int leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;

	//縦軸
	int leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;

	//0がスティックが動いていない状態
	
	//							//スティックを最大まで倒したときの数値
	int stickXRight = 900;		//0～1000
	int stickXLeft = -900;		//0～-1000
	int stickYDown = 900;		//0～1000
	int stickYUp = -900;		//0～-1000

	//スティックが倒されているかどうか
	bool isStickMoved = (leftStickX_ != 0 || leftStickY_ != 0);
	bool isStickPressed = false; // スティックが倒されたかどうかのフラグ
	bool lastStickState = false; // 最後のスティック状態（倒されているかどうか）

	switch (GetDevice())
	{
	case SceneManager::CNTL::KEYBOARD:
		ChangeDevice(SceneManager::CNTL::KEYBOARD);

		//キーの押下判定
		if (ins.IsNew(KEY_INPUT_UP) ||
			ins.IsNew(KEY_INPUT_W))
		{
			key_ = KEY_CONFIG::UP;
		}
		if (ins.IsNew(KEY_INPUT_DOWN) ||
			ins.IsNew(KEY_INPUT_S))
		{
			key_ = KEY_CONFIG::DOWN;
		}
		if (ins.IsNew(KEY_INPUT_LEFT) ||
			ins.IsNew(KEY_INPUT_A))
		{
			key_ = KEY_CONFIG::LEFT;
		}
		if (ins.IsNew(KEY_INPUT_RIGHT) ||
			ins.IsNew(KEY_INPUT_D))
		{
			key_ = KEY_CONFIG::RIGHT;
		}

		//キーの押下判定(押した瞬間だけ)
		if (ins.IsTrgDown(KEY_INPUT_UP) ||
			ins.IsTrgDown(KEY_INPUT_W))
		{
			key_ = KEY_CONFIG::UP_TRG;
		}
		if (ins.IsTrgDown(KEY_INPUT_DOWN) ||
			ins.IsTrgDown(KEY_INPUT_S))
		{
			key_ = KEY_CONFIG::DOWN_TRG;
		}
		if (ins.IsTrgDown(KEY_INPUT_LEFT) ||
			ins.IsTrgDown(KEY_INPUT_A))
		{
			key_ = KEY_CONFIG::LEFT_TRG;
		}
		if (ins.IsTrgDown(KEY_INPUT_RIGHT) ||
			ins.IsTrgDown(KEY_INPUT_D))
		{
			key_ = KEY_CONFIG::RIGHT_TRG;
		}

		if (ins.IsTrgDown(KEY_INPUT_SPACE))
		{
			key_ = KEY_CONFIG::DECIDE;
		}
		break;

	case SceneManager::CNTL::PAD:
		ChangeDevice(SceneManager::CNTL::PAD);
		if (leftStickY_ < -1)
		{
			key_ = KEY_CONFIG::UP;
			
		}
		if (leftStickY_ > 1)
		{
			key_ = KEY_CONFIG::DOWN;
		}
		if (leftStickX_ < -1)
		{
			key_ = KEY_CONFIG::LEFT;
		}
		if (leftStickX_ > 1)
		{
			key_ = KEY_CONFIG::RIGHT;
		}

		//
		if (leftStickY_ < -1)
		{
			key_ = KEY_CONFIG::UP;

		}
		if (leftStickY_ > 1)
		{
			key_ = KEY_CONFIG::DOWN;
		}
		if (leftStickX_ < -1)
		{
			key_ = KEY_CONFIG::LEFT;
		}
		if (leftStickX_ > 1)
		{
			key_ = KEY_CONFIG::RIGHT;
		}

		if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
		{
			key_ = KEY_CONFIG::DECIDE;
		}

		break;
	default:
		break;
	}
}

SceneManager::CNTL SelectScene::GetDevice(void)
{
	//1Pの操作選択後であったら使用デバイスを固定(とりあえず)
	if (selectedCntl_ == SceneManager::CNTL::KEYBOARD)
	{
		device_ = SceneManager::CNTL::KEYBOARD;
	}
	else if(selectedCntl_ == SceneManager::CNTL::PAD)
	{
		device_ = SceneManager::CNTL::PAD;
	}

	return device_;
}

SelectScene::KEY_CONFIG SelectScene::GetKeyConfig(void)
{
	return key_;
}

void SelectScene::ChangeDevice(SceneManager::CNTL device)
{
	device_ = device;
}

bool SelectScene::IsHitRect(Rect& rc, Vector2 pos, int r)
{
	auto diffX = pos.x - rc.pos.x;	//終点から始点を引く
	auto diffY = pos.y - rc.pos.y;	
	if (fabsf(diffX) > r + rc.w / 2 ||
		fabsf(diffY) > r + rc.h / 2) {
		return false;
	}
	else {
		return true;
	}
}

void SelectScene::ControllDevice(void)
{
	InputManager& ins = InputManager::GetInstance();
	int key_ = CheckHitKeyAll(DX_CHECKINPUT_KEY);		//入力を調べる(キーボード)
	int padNum = GetJoypadNum();
	int padState = CheckHitKeyAll(DX_CHECKINPUT_PAD);	//入力を調べる(パッド)

	//キーボード操作の時パッド操作をできないように
	if (key_ != 0	&&
		padState == 0)
	{
		ChangeDevice(SceneManager::CNTL::KEYBOARD);
	}//パッド操作の時キー操作をできないように
	else if (key_ <= 0 &&
		padNum > 0 &&
		padState != 0)
	{
		ChangeDevice(SceneManager::CNTL::PAD);
	}
}

//デバッグ用の図形描画------------------------------------------------------------------------

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