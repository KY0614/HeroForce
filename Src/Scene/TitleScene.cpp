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
	imgTitleLogo_ = -1;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	//windowNum_ = 1;
	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCameras()[0]->ChangeMode(Camera::MODE::FIXED_POINT);
}

void TitleScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	SceneManager& mng = SceneManager::GetInstance();
	DataBank& data = DataBank::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		//data.Input(DataBank::INFO::USER_NUM, windowNum_);	//ゲームシーン移行時にやるもの
		mng.ChangeScene(SceneManager::SCENE_ID::SELECT);
	}

	/*if (ins.IsTrgDown(KEY_INPUT_RIGHT))
	{
		windowNum_++;
		if (windowNum_ > SceneManager::PLAYER_NUM)windowNum_ = SceneManager::PLAYER_NUM;
	}

	if (ins.IsTrgDown(KEY_INPUT_LEFT))
	{
		windowNum_--;
		if (windowNum_ < 1)windowNum_ = 1;
	}*/
}

void TitleScene::Draw(void)
{

	// ロゴ描画
	DrawLogo();
	//DrawFormatString(400, 400, 0xffffff, "%d", windowNum_);
}

void TitleScene::Release(void)
{
}

void TitleScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// タイトルロゴ
	DrawRotaGraph(
		cx, cy - 200,
		1.0f, 0.0f, imgTitleLogo_, true);

	// Pushメッセージ
	std::string msg = "Push Space";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 200, 0x87cefa, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

}
