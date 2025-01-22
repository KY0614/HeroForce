#include<DxLib.h>
#include"../Application.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/ResourceManager.h"

#include "GameOverScene.h"

void GameOverScene::Init(void)
{
	toTitleImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TO_TITLE).handleId_;
}

void GameOverScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_SPACE)||ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1,InputManager::JOYPAD_BTN::RIGHT))
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
}

void GameOverScene::Draw(void)
{
	DrawString(0, 0, "GameOver", 0xff00ff, true);

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	//タイトル
	DrawRotaGraph(
		cx, cy + 200,
		0.5f, 0.0f, toTitleImg_, true);

	DrawString(cx-200, cy + 100, "タイトルに戻ります\nSpaceまたはBボタンを押してください", 0xff0000, true);
}

void GameOverScene::Release(void)
{
}
