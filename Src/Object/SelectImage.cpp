#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/DataBank.h"
#include "../Scene/SelectScene.h"
#include "SelectImage.h"

SelectImage::SelectImage(SelectScene& select) : selectScene_(select)
{
	imgLeftPoint_ = -1;
	imgRightPoint_ = -1;

	playerNum_ = 1;
}

void SelectImage::Destroy(void)
{
}

void SelectImage::Init(void)
{
	Load();
}

void SelectImage::Update(void)
{

}

void SelectImage::Draw(void)
{
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f, 0.0f,
		imgPlayerNum_[playerNum_ + 1],	//配列は0～3なので
		true, false);

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "%d", playerNum_);

	DrawRotaGraph(Application::SCREEN_SIZE_X / 2 + (POINT_SCALE * 5), Application::SCREEN_SIZE_Y / 2,
		POINT_SCALE_RATE, 0.0f,
		imgRightPoint_,
		true, false);

	DrawRotaGraph(Application::SCREEN_SIZE_X / 2 - (POINT_SCALE * 5), Application::SCREEN_SIZE_Y / 2,
		POINT_SCALE_RATE, 0.0f,
		imgLeftPoint_,
		true, false);
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

//void SelectImage::NumberUpdate(void)
//{
//	InputManager& ins = InputManager::GetInstance();
//	DataBank& data = DataBank::GetInstance();
//	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();
//
//	//右の矢印がONの時にキーの右に値する入力をし続けると
//	if (/*triR.isToggle_ &&*/
//		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
//	{
//		if (!press_)
//		{
//			press_ = true;
//
//			//人数を１追加(中身は1～4に収める)
//			playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1;
//		}
//
//		//色を白に
//		triR.color_ = GetColor(255, 255, 255);
//
//		//キーが押されている間経過時間を加算していく
//		keyPressTime_ += delta;
//
//		//経過時間がある一定時間経った場合
//		if (keyPressTime_ > SELECT_TIME)
//		{
//			//インターバルを加算していく
//			interval_ += delta;
//
//			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
//			(interval_ > INTERVAL_TIME) ?
//				interval_ = 0.0f, playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1 : interval_;
//		}
//	}
//	else if (triR.isToggle_)
//	{
//		keyPressTime_ = 0.0f;
//		interval_ = INTERVAL_TIME;
//		press_ = false;
//	}
//
//	//左
//	if (/*triL.isToggle_ &&*/
//		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
//	{
//		if (!press_)
//		{
//			press_ = true;
//
//			//人数を１削除(中身は1～4に収める)
//			playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM;
//			if (playerNum_ == 0)playerNum_ = 4;
//		}
//		//色を白に
//		triL.color_ = GetColor(255, 255, 255);
//
//		//キーが押されている間経過時間を加算していく
//		keyPressTime_ += delta;
//
//		//経過時間がある一定時間経った場合
//		if (keyPressTime_ > SELECT_TIME)
//		{
//			//インターバルを加算していく
//			interval_ += delta;
//
//			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
//			(interval_ > INTERVAL_TIME) ?
//				interval_ = 0.0f, playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM : interval_;
//			if (playerNum_ == 0)playerNum_ = 4;
//		}
//	}
//	else if (triL.isToggle_)
//	{
//		keyPressTime_ = 0.0f;
//		interval_ = INTERVAL_TIME;
//		press_ = false;
//	}
//
//	//スペースキー押下で決定&入力デバイス選択へ
//	if (selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::DECIDE)
//	{
//		//プレイヤー人数の設定
//		data.Input(SceneManager::PLAY_MODE::USER, playerNum_);
//		//ディスプレイの設定
//		data.Input(DataBank::INFO::DHISPLAY_NUM, playerNum_);
//		data.Input(DataBank::INFO::USER_NUM, playerNum_);
//
//		//CPU人数の設定(CPUは１人から３人)
//		data.Input(SceneManager::PLAY_MODE::CPU, (SceneManager::PLAYER_NUM-playerNum_));
//
//		//ウィンドウ複製の準備
//		SceneManager::GetInstance().RedySubWindow();
//
//		//カメラの設定
//		auto cameras = SceneManager::GetInstance().GetCameras();
//		for (int i = 0; i < playerNum_; i++)
//		{
//			cameras[i]->SetPos(SelectScene::DEFAULT_CAMERA_POS, SelectScene::DEFAULT_TARGET_POS);
//			//cameras[i]->SetFollow(&players_[i]->GetTransform());
//			cameras[i]->ChangeMode(Camera::MODE::FIXED_POINT);
//		}
//
//		selectScene_.ChangeSelect(SelectScene::SELECT::OPERATION);
//	}
//
//	//選択する三角形
//	if (!triR.isToggle_ &&
//		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
//	{
//		triR.isToggle_ = true;
//		triL.isToggle_ = false;
//	}
//
//	if (!triL.isToggle_ &&
//		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
//	{
//		triR.isToggle_ = false;
//		triL.isToggle_ = true;
//	}
//}

void SelectImage::OperationUpdate(void)
{
}

void SelectImage::RoleUpdate(void)
{
}
