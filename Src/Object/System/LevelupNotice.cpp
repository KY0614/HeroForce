#include <DxLib.h>
#include "../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../Common/Vector2.h"
#include "../Common/ImageFader.h"
#include "LevelupNotice.h"

LevelupNotice::LevelupNotice()
{
	imgFader_ = nullptr;
	img_ = -1;
	cnt_ = 0;
	scl_ = 1.0f;
	state_ = STATE::NONE;
}

LevelupNotice::~LevelupNotice()
{
}

void LevelupNotice::Init()
{
	imgFader_ = new ImageFader();
	imgFader_->Init();

	//画像
	img_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEVEL_UP).handleId_;
	if (img_ == -1)
	{
		return;
	}
}

void LevelupNotice::Update()
{
	float speed = 0.001f;	//拡大スピード;
	float max = 1.2f;		//拡大倍率
	int timeLimit = 1.5 * SceneManager::DEFAULT_FPS;

	switch (state_)
	{
	case STATE::FADE_IN:
	//画像のフェード処理
	imgFader_->SetFade(Fader::STATE::FADE_OUT);
	imgFader_->Update();
	//拡大処理
	
	scl_ += speed;
	//画像サイズが最大になったら
	if (scl_ >= max)
	{
		scl_ = max;
		state_ = STATE::MAINTAIN;
	}
		break;

	case STATE::FIN:
		break;

	case STATE::MAINTAIN:
		//本来はボタンを押してから
		//次の処理へ移るが
		//いったんカウントで
		cnt_ ++;	//カウント更新
		if (cnt_ >= timeLimit)
		{
			state_ = STATE::FIN;
		}

		break;
	}
}

void LevelupNotice::Draw()
{
	//描画設定
	Vector2 pos =
	{ Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 };
	float angle = 0.0f;
	bool trans = true;
	bool reverse = false;

	//画像の描画
	imgFader_->Draw(img_, pos, scl_, angle, trans, reverse);
}
void LevelupNotice::Reset()
{
	cnt_ = 0;
	scl_ = 1.0f;
	state_ = STATE::NONE;
	imgFader_->Init();
}
void LevelupNotice::SetState(const STATE state)
{
	state_ = state;
}
