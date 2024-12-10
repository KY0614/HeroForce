#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "SelectImage.h"

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
	DrawGraph(Application::SCREEN_SIZE_X/2,Application::SCREEN_SIZE_Y/2,playerNumImg_,true);
	DrawGraph(Application::SCREEN_SIZE_X/2 + 100,Application::SCREEN_SIZE_Y/2,rightPointImg_,true);
	DrawGraph(Application::SCREEN_SIZE_X/2 - 100,Application::SCREEN_SIZE_Y/2,leftPointImg_,true);
}

void SelectImage::Load(void)
{
	//‰æ‘œ
	playerNumImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_NUM).handleId_;
	if (playerNumImg_ == -1)
	{
		return;
	}

	rightPointImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RIGHT_POINT).handleId_;
	if (rightPointImg_ == -1)
	{
		return;
	}

	leftPointImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEFT_POINT).handleId_;
	if (leftPointImg_ == -1)
	{
		return;
	}
}
