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
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/StageObject.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/Character/SelectPlayer.h"
#include "../Object/SelectImage.h"
#include "SelectScene.h"

SelectScene::SelectScene(void)
{

}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();
	
	//背景用ステージ
	stage_ = new StageManager();
	stage_->Init();

	//背景色を白に
	SetBackgroundColor(255, 255, 255);
	//背景のステージモデルやらを半透明に
	//float alpha = 0.5f;
	//MV1SetOpacityRate(skyDome_->GetTransform().modelId, alpha);
	//for (int i = 0; i < StageManager::MODELS; i++) {
	//	for (auto& s : stage_->GetTtans(static_cast<StageManager::MODEL_TYPE>(i)))
	//	{
	//		MV1SetOpacityRate(s.modelId, alpha);
	//	}
	//}

	//フォグの設定
	SetFogEnable(true);
	//白
	SetFogColor(255, 255, 255);
	SetFogStartEnd(-10000.0f, 15000.0f);

	//プレイヤー設定
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		players_[i] = std::make_unique<SelectPlayer>();
		players_[i]->Init();
	}

	//画像設定
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		images_[i] = std::make_unique<SelectImage>(*this);
		images_[i]->Init();
	}

	// カメラモード：定点カメラ
	auto camera = SceneManager::GetInstance().GetCameras();
	camera[0]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
	camera[0]->ChangeMode(Camera::MODE::FIXED_POINT);

	//人数選択から
	ChangeSelect(SELECT::NUMBER);

	KeyConfigSetting();

	key_ = KEY_CONFIG::NONE;

	ChangeDevice(SceneManager::CNTL::KEYBOARD);

	//playerNum_ = 1;

	isPad_ = false;

	role_ = 0;

	//図形用------------------------------------------------

	//三角形の中心座標と大きさ
	triL = { TRI_POS_X - 300,TRI_POS_Y ,TRI_SCALE,TRI_SCALE ,false };
	triR = { TRI_POS_X + 300,TRI_POS_Y ,TRI_SCALE,TRI_SCALE ,false };

	//三角形の色
	triL.color_ = GetColor(255, 255, 64);
	triR.color_ = GetColor(255, 255, 64);

	//四角形の中心座標と大きさ
	rc = { Application::SCREEN_SIZE_X/2/*750*/,Application::SCREEN_SIZE_Y / 2/*450*/,RECT_SCALE,RECT_SCALE };

	//四角形の色
	rc.color_ = GetColor(255, 0, 0);

	//三角形の描画座標
	triL.pos.x = rc.pos.x - TRI_SCALE - PRI_SPACE;
	triR.pos.x = rc.pos.x + TRI_SCALE + PRI_SPACE;

	//------------------------------------------------------

	//keyPressTime_ = 0.0f;
	//interval_ = 0.0f;

}

void SelectScene::Update(void)
{
	//キーの設定
	KeyConfigSetting();

	//どちらかを操作しているときにもう片方を操作できないように制御
	ControllDevice();

	//空を回転
	skyDome_->Update();

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

	trans_.Update();
	for (auto& i : tests_)
	{
		i.Update();
	}
}

void SelectScene::Draw(void)
{
	auto& ins = InputManager::GetInstance();

	skyDome_->Draw();
	stage_->Draw();

	//デバッグ描画
	DrawDebug();

	switch (select_)
	{
	case SelectScene::SELECT::NUMBER:
		NumberDraw();
		break;

	case SelectScene::SELECT::OPERATION:
		OperationDraw();
		break;

	case SelectScene::SELECT::ROLE:
		RoleDraw();
		break;

	default:
		break;
	}

	for (auto& i : images_)
	{
		i->Draw();
	}
}

void SelectScene::Release(void)
{
	SceneManager::GetInstance().ResetCameras();

	for (auto i : tests_)
	{
		MV1DeleteModel(i.modelId);
	}
	MV1DeleteModel(trans_.modelId);
}

void SelectScene::InitModel(void)
{
	//２次元配列にしてそれぞれ対応させる？


}

void SelectScene::NumberUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	for (auto& i : images_)
	{
		i->Update();
	}

#ifdef DEBUG_RECT

	//三角形のボタンを選択中だったら緑に非選択だったら黄色に
	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);

	//右の三角形がONの時にキーの右に値する入力をし続けると
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//人数を１追加(中身は1～4に収める)
			playerNum_ = (playerNum_ % PLAYER_NUM) + 1;
		}
		
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
				interval_ = 0.0f , playerNum_ = (playerNum_ % PLAYER_NUM) + 1 : interval_;
		}
	}
	else if(triR.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//左
	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//人数を１削除(中身は1～4に収める)
			playerNum_ = (playerNum_ + 3) % PLAYER_NUM;
			if (playerNum_ == 0)playerNum_ = 4;
		}
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
				interval_ = 0.0f, playerNum_ = (playerNum_ + 3) % PLAYER_NUM : interval_;
			if (playerNum_ == 0)playerNum_ = 4;
		}
	}
	else if (triL.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//プレイ人数の範囲内に数値を収める
	//playerNum_ = std::clamp(playerNum_, 1, PLAYER_NUM );	//１～４人プレイなので1～4まで

	//スペースキー押下で決定&入力デバイス選択へ
	if (key_ == KEY_CONFIG::DECIDE)
	{
		//プレイヤー人数の設定
		data.Input(SceneManager::PLAY_MODE::USER, playerNum_);
		//ディスプレイの設定
		data.Input(DataBank::INFO::DHISPLAY_NUM, playerNum_);
		data.Input(DataBank::INFO::USER_NUM, playerNum_);	

		//CPU人数の設定(CPUは１人から３人)
		data.Input(SceneManager::PLAY_MODE::CPU, (PLAYER_NUM) - playerNum_);

		//ウィンドウ複製の準備
		SceneManager::GetInstance().RedySubWindow();

		//カメラの設定
		auto cameras = SceneManager::GetInstance().GetCameras();
		for (int i = 0; i < playerNum_; i++)
		{
			cameras[i]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
			cameras[i]->SetFollow(&players_[i]->GetTransform());
			cameras[i]->ChangeMode(Camera::MODE::FIXED_POINT);
		}

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

	data.Input(SceneManager::CNTL::PAD, 2);
	data.Input(SceneManager::CNTL::PAD, 3);
	data.Input(SceneManager::CNTL::PAD, 4);

#ifdef DEBUG_RECT
	//三角形のボタンが選択中だったら緑に非選択だったら黄色に
	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);

	//右の三角形がONの時にキーの右に値する入力をし続けると
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//キーボードを選択
			isPad_ = true;
		}

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
		press_ = false;
	}

	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//キーボードを選択
			isPad_ = false;
		}

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
		press_ = false;
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
	//三角形のボタンが選択中だったら緑に非選択だったら黄色に
	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);

	//右の三角形がONの時にキーの右に値する入力をし続けると
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//役職を選択
			role_ = (role_ + 1) % PLAYER_NUM;
		}

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
				interval_ = 0.0f, role_ = (role_ + 1) % PLAYER_NUM : interval_;
		}
	}
	else if (triR.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//役職を選択
			role_ = (role_ - 1 + PLAYER_NUM) % PLAYER_NUM;
		}
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
				interval_ = 0.0f, role_ = (role_ - 1 + PLAYER_NUM) % PLAYER_NUM : interval_;
		}
	}
	else if (triL.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//役職数の範囲内に数値を収める
	//role_ = std::clamp(role_, 0, static_cast<int>(SceneManager::ROLE::ARCHER) + 1);	//４役職なので0～3まで

	//スペースキー押下でゲーム画面へ
	if (GetKeyConfig() == KEY_CONFIG::DECIDE)
	{
		//役職の設定
		data.Input(static_cast<SceneManager::ROLE>(role_),playerNum_);

		//押下したときの色
		rc.color_ = 0xFF0000;

		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
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

void SelectScene::NumberDraw(void)
{
#ifdef DEBUG_RECT

	DrawFormatString(rc.pos.x, rc.pos.y,
		0xFFFFFF, "%d", playerNum_);

	DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200, 0,
		0xFF9999, "プレイ人数選択中");

	DrawFormatString(0, 0,
		0xFFFFFF, "time : %.2f", keyPressTime_);
#endif // DEBUG_RECT
}

void SelectScene::OperationDraw(void)
{
#ifdef DEBUG_RECT
	for (int i = 0; i < playerNum_; i++) {
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
	}

#endif // DEBUG_RECT

}

void SelectScene::RoleDraw(void)
{
#ifdef DEBUG_RECT
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


	for (auto& p : players_)
	{
		p->SetRole(role_);
		p->Draw();
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

	//三角形描画
	triL.LeftDraw(triL.color_);
	triR.RightDraw(triR.color_);

#endif // DEBUG_RECT

	//現在の入力デバイス
	DrawFormatString(0, 820, 0x000000, "(key_:0)(pad:1) %d", GetDevice());
	//入力の種類
	DrawFormatString(0,
		800,
		0x000000,
		"%d",
		key_);

	////プレイ人数
	//DrawFormatString(Application::SCREEN_SIZE_X / 2, 0, 0x000000, "number : %d", playerNum_);
	//1Pかパッド操作かどうか
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 20, 0x000000, "operation : %d", isPad_);
	//役職
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 40, 0x000000, "role_ : %d", role_);

	DrawFormatString(Application::SCREEN_SIZE_X / 2, 100, 0x000000, "pos : %2.f,%2.f,%2.f", trans_.pos.x,trans_.pos.y,trans_.pos.z);

	// 左スティックの横軸
	int leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;
	//縦軸
	int leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;

	DrawFormatString(Application::SCREEN_SIZE_X / 2, 60, 0x000000, "stickX : %d", leftStickX_);
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 80, 0x000000, "stickY : %d", leftStickY_);

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
		if (ins.IsNew(KEY_INPUT_UP)		||	ins.IsNew(KEY_INPUT_W))	key_ = KEY_CONFIG::UP;
		if (ins.IsNew(KEY_INPUT_DOWN)	||	ins.IsNew(KEY_INPUT_S))	key_ = KEY_CONFIG::DOWN;
		if (ins.IsNew(KEY_INPUT_LEFT)	||	ins.IsNew(KEY_INPUT_A))	key_ = KEY_CONFIG::LEFT;
		if (ins.IsNew(KEY_INPUT_RIGHT)	||	ins.IsNew(KEY_INPUT_D))	key_ = KEY_CONFIG::RIGHT;

		//キーの押下判定(押した瞬間だけ)
		if (ins.IsTrgDown(KEY_INPUT_UP) ||	ins.IsTrgDown(KEY_INPUT_W))key_ = KEY_CONFIG::UP_TRG;
		if (ins.IsTrgDown(KEY_INPUT_DOWN) ||ins.IsTrgDown(KEY_INPUT_S))key_ = KEY_CONFIG::DOWN_TRG;
		if (ins.IsTrgDown(KEY_INPUT_LEFT) ||ins.IsTrgDown(KEY_INPUT_A))key_ = KEY_CONFIG::LEFT_TRG;
		if (ins.IsTrgDown(KEY_INPUT_RIGHT)||ins.IsTrgDown(KEY_INPUT_D))key_ = KEY_CONFIG::RIGHT_TRG;

		if (ins.IsTrgDown(KEY_INPUT_SPACE))key_ = KEY_CONFIG::DECIDE;
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
		if (leftStickX_ < stickXLeft)
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
	//返り値用のret等で運用すること
	//1Pの操作選択後であったら使用デバイスを固定(とりあえず)
	SceneManager::CNTL ret;
	if (selectedCntl_ == SceneManager::CNTL::KEYBOARD)	ret = SceneManager::CNTL::KEYBOARD;
	else if(selectedCntl_ == SceneManager::CNTL::PAD)	ret = SceneManager::CNTL::PAD;
	
	return ret;

	//if (selectedCntl_ == SceneManager::CNTL::KEYBOARD)
	//{
	//	ChangeDevice(SceneManager::CNTL::KEYBOARD);
	//	return SceneManager::CNTL::KEYBOARD;
	//}
	//ChangeDevice(SceneManager::CNTL::PAD);
	//return SceneManager::CNTL::PAD;
}

void SelectScene::ChangeDevice(SceneManager::CNTL device)
{
	device_ = device;
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