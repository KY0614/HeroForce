#include<DxLib.h>
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"

#include "GameOverScene.h"

void GameOverScene::Init(void)
{
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

	DrawString(cx-200, cy - 200, "タイトルに戻ります\nSpaceまたはBボタンを押してください", 0xff0000, true);
}

void GameOverScene::Release(void)
{
}
