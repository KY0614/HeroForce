#include <DxLib.h>
#include "../Application.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/GameSystem/TextManager.h"
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

	//テキスト
	auto& text_m = TextManager::GetInstance();
	using FONT_T = TextManager::FONT_TYPE;	//省略用
	text_ = text_m.TextLoad(TextManager::TEXTS::LEVEL_UP);

	//フォント生成
	font_ = CreateFontToHandle(
		text_m.GetFontName(FONT_T::LOGO).c_str(),
		FONT_SIZE,
		FONT_THICK);

	//画像
	img_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEVEL_UP).handleId_;
	if (img_ == -1)
	{
		return;
	}
}

void LevelupNotice::Update()
{
	float speed = EXPANSION_RATE;	//拡大スピード;
	float max = EXPANSION_MAX;		//拡大倍率
	int timeLimit = MAINTAIN_SECONDS * Application::DEFAULT_FPS;

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
	int color = 0xffffff;
	int c = TextManager::CENTER_TEXT;
	int cH = TextManager::CENTER_TEXT_H;

	//画像の描画
	imgFader_->Draw(img_, pos, scl_, angle, trans, reverse);

	//テキスト描画
	pos.x -= text_.length() * FONT_SIZE / cH;
	pos.y -= FONT_SIZE / c;
	DrawFormatStringToHandle(pos.x, pos.y,color, font_, text_.c_str());
}

void LevelupNotice::Release()
{
	DeleteFontToHandle(font_);
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
