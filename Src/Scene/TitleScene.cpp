//#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/DataBank.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/SkyDome.h"

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

	SceneManager::GetInstance().GetCameras()[0]->SetPos(VECTOR{ 0,100,-800 }, VECTOR{ 0,0,0 });

	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	//背景用ステージ
	stage_ = std::make_unique<StageManager>();
	stage_->Init();

	imgTitleLogo_= ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE).handleId_;
}

void TitleScene::Update(void)
{
	//空を回転
	skyDome_->Update();

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	SceneManager& mng = SceneManager::GetInstance();
	DataBank& data = DataBank::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_SPACE)|| ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		//data.Input(DataBank::INFO::USER_NUM, windowNum_);	//ゲームシーン移行時にやるもの
		mng.ChangeScene(SceneManager::SCENE_ID::SELECT);
	}
}

void TitleScene::Draw(void)
{
	skyDome_->Draw();
	stage_->Draw();
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
		cx, cy - 100,
		0.5f, 0.0f, imgTitleLogo_, true);

	//DrawString(cx, cy - 200, "HeroForce", 0xff0000, true);

	// Pushメッセージ
	std::string msg = "Push 「Space」 or 「Bボタン」";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 400, 0xff0000, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

}
