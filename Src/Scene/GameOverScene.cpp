#include<DxLib.h>
#include "GameOverScene.h"
#include"../Application.h"

void GameOverScene::Init(void)
{
}

void GameOverScene::Update(void)
{
}

void GameOverScene::Draw(void)
{
	DrawString(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, "GAMEOVER", 0xff0000, true);
}

void GameOverScene::Release(void)
{
}
