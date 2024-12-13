#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/DataBank.h"
#include "../Scene/SelectScene.h"
#include "SelectImage.h"

SelectImage::SelectImage(SelectScene& select) : selectScene_(select)
{
	imgPlayerNum_ = nullptr;
	imgLeftPoint_ = -1;
	imgRightPoint_ = -1;

	playerNum_ = 1;
	isPad_ = false;
	role_ = 0;

	keyPressTime_ = 0.0f;
	interval_ = 0.0f;
	press_ = false;
}

void SelectImage::Destroy(void)
{
}

void SelectImage::Init(void)
{
	Load();

	pointL_ = { LEFT_POS_X,POINT_POS_Y,POINT_SCALE,POINT_SCALE,false,imgLeftPoint_ };	
	
	pointR_ = { RIGHT_POS_X,POINT_POS_Y,POINT_SCALE,POINT_SCALE,false,imgRightPoint_ };
}

void SelectImage::Update(void)
{
	switch (selectScene_.GetSelect())
	{
	case SelectScene::SELECT::NUMBER:
		NumberUpdate();
		break;

	case SelectScene::SELECT::OPERATION:
		OperationUpdate();
		break;

	case SelectScene::SELECT::ROLE:
		RoleUpdate();
		break;

	default:
		break;
	}
}

void SelectImage::Draw(void)
{
	//将来的にはDrawPolygon3Dで描画

	switch (selectScene_.GetSelect())
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
	
	DrawFormatString(Application::SCREEN_SIZE_X - 100,0, 0x000000, "L : %d", pointL_.isToggle_);
	DrawFormatString(Application::SCREEN_SIZE_X - 100,20, 0x000000, "R : %d", pointR_.isToggle_);
}

void SelectImage::Update1(void)
{
	OperationUpdate();
}

void SelectImage::Load(void)
{
	//画像
	imgPlayerNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_NUM).handleIds_;

	imgRightPoint_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RIGHT_POINT).handleId_;
	if (imgRightPoint_ == -1)
	{
		return;
	}

	imgLeftPoint_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEFT_POINT).handleId_;
	if (imgLeftPoint_ == -1)
	{
		return;
	}
}

void SelectImage::NumberUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	//右の矢印がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//人数を１追加(中身は1～4に収める)
			playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1 : interval_;
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//左
	if (pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//人数を１削除(中身は1～4に収める)
			playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM;
			if (playerNum_ == 0)playerNum_ = 4;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM : interval_;
			if (playerNum_ == 0)playerNum_ = 4;
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//スペースキー押下で決定&入力デバイス選択へ
	if (selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		//プレイヤー人数の設定
		data.Input(SceneManager::PLAY_MODE::USER, playerNum_);
		//ディスプレイの設定
		data.Input(DataBank::INFO::DHISPLAY_NUM, playerNum_);
		data.Input(DataBank::INFO::USER_NUM, playerNum_);

		//CPU人数の設定(CPUは１人から３人)
		data.Input(SceneManager::PLAY_MODE::CPU, (SceneManager::PLAYER_NUM-playerNum_));

		//ウィンドウ複製の準備
		SceneManager::GetInstance().RedySubWindow();

		//カメラの設定
		auto cameras = SceneManager::GetInstance().GetCameras();
		for (int i = 0; i < cameras.size(); i++)
		{ 
			cameras[i]->SetPos(SelectScene::DEFAULT_CAMERA_POS, SelectScene::DEFAULT_TARGET_POS);
			cameras[i]->ChangeMode(Camera::MODE::FIXED_POINT);
		}

		selectScene_.ChangeSelect(SelectScene::SELECT::OPERATION);
	}

	//選択する矢印
	if (!pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}
}

void SelectImage::OperationUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	data.Input(SceneManager::CNTL::PAD, 2);
	data.Input(SceneManager::CNTL::PAD, 3);
	data.Input(SceneManager::CNTL::PAD, 4);

	//右の矢印がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//キーを押した際に選択しているものを反転(2種類しかないので)
			isPad_ = !isPad_;
		}
		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, isPad_ = !isPad_ : interval_;
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//左
	if (pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//キーを押した際に選択しているものを反転(2種類しかないので)
			isPad_ = !isPad_;
		}
		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, isPad_ = !isPad_ : interval_;
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//スペースキー押下で決定&役職選択へ
	if (selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		//1Pの操作の設定
		(isPad_) ? data.Input(SceneManager::CNTL::PAD, 1) : data.Input(SceneManager::CNTL::KEYBOARD, 1);
		//selectedCntl_ = (isPad_) ? SceneManager::CNTL::PAD : SceneManager::CNTL::KEYBOARD;

		selectScene_.ChangeSelect(SelectScene::SELECT::ROLE);
	}

	//選択する矢印
	if (!pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}
}

void SelectImage::RoleUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	//右の三角形がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//役職を選択
			role_ = (role_ + 1) % SceneManager::PLAYER_NUM;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, role_ = (role_ + 1) % SceneManager::PLAYER_NUM : interval_;
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	if (pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//役職を選択
			role_ = (role_ - 1 + SceneManager::PLAYER_NUM) % SceneManager::PLAYER_NUM;
		}
		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, role_ = (role_ - 1 + SceneManager::PLAYER_NUM) % SceneManager::PLAYER_NUM : interval_;
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//スペースキー押下でゲーム画面へ
	if (selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		//役職の設定
		data.Input(static_cast<SceneManager::ROLE>(role_), playerNum_);

		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	//選択する三角形
	if (!pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}
}

void SelectImage::NumberDraw(void)
{
	//人数選択画像の描画
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f, 0.0f,
		imgPlayerNum_[playerNum_ - 1],	//配列は0～3なので
		true, false);

	//左の矢印の描画(右の矢印は暗めにする)
	if (pointL_.isToggle_)
	{
		//右の矢印画像を描画し、減算ブレンドする
		pointR_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
		pointR_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//ブレンドモードを戻す

		pointL_.PointDraw();
	}
	//右の矢印の描画(左の矢印は暗めにする)
	if (pointR_.isToggle_)
	{
		//左の矢印画像を描画し、減算ブレンドする
		pointL_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
		pointL_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//ブレンドモードを戻す

		pointR_.PointDraw();
	}

	//どちらの矢印も選んでいないときの描画
	if (!pointL_.isToggle_ && !pointR_.isToggle_)
	{
		//普通に描画
		pointL_.PointDraw();
		pointR_.PointDraw();
		//↑の上の画像に減算ブレンドする
		SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
		pointL_.PointDraw();
		pointR_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "num : %d", playerNum_);
}

void SelectImage::OperationDraw(void)
{
	//人数選択画像の描画
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f, 0.0f,
		imgPlayerNum_[(int)(isPad_)],	//配列は0～3なので
		true, false);

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "ope");
}

void SelectImage::RoleDraw(void)
{
	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "role");
}

void SelectImage::Point::PointDraw(void)
{
	DrawRotaGraph(pos.x, pos.y,
		POINT_SCALE_RATE, 0.0f,
		imgHandle_,
		true, false);
}