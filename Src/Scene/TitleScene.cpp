//#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/DataBank.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgLogo_ = -1;
	imgMes_ = -1;
	sky_ = nullptr;
}

void TitleScene::Init(void)
{
	//スカイドーム
	sky_ = std::make_unique<SkyDome>();
	sky_->Init();

	//画像読み込み
	imgLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_LOGO).handleId_;
	imgMes_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLEASE_KEY).handleId_;

	//カメラ設定
	auto camera = SceneManager::GetInstance().GetCameras();
	camera[0]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
	camera[0]->ChangeMode(Camera::MODE::FIXED_POINT);
}

void TitleScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	SceneManager& mng = SceneManager::GetInstance();

	//スカイドーム更新
	sky_->Update();

	// シーン遷移
	if (ins.IsTrgDown(KEY_INPUT_SPACE)|| ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		mng.ChangeScene(SceneManager::SCENE_ID::SELECT);
	}
}

void TitleScene::Draw(void)
{
	//スカイドーム描画
	sky_->Draw();

	//ロゴの描画
	DrawRotaGraph(
		LOGO_POS_X,LOGO_POS_Y,
		1.0f,
		0.0f,
		imgLogo_,
		true,
		false);

	//メッセージ描画
	DrawRotaGraph(
		MES_POS_X,MES_POS_Y,
		1.0f,
		0.0f,
		imgMes_,
		true,
		false);
}

void TitleScene::Release(void)
{
	sky_->Release();
}

void TitleScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// タイトルロゴ
	DrawRotaGraph(
		cx, cy - 200,
		1.0f, 0.0f, imgLogo_, true);

	DrawString(cx, cy - 200, "HeroForce", 0xff0000, true);

	// Pushメッセージ
	std::string msg = "Push 「Space」 or 「Bボタン」";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 400, 0x87cefa, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

}
