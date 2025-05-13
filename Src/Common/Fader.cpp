#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "Fader.h"

Fader::STATE Fader::GetState(void) const
{
	return state_;
}

bool Fader::IsEnd(void) const
{
	return isEnd_;
}

void Fader::SetFade(STATE state)
{
	state_ = state;
	if (state_ != STATE::NONE)
	{
		isPreEnd_ = false;
		isEnd_ = false;
	}
}

void Fader::SetAlpha(float alpha)
{
	alphaMax_ = alpha_;
}

void Fader::Init(void)
{
	state_ = STATE::NONE;
	alpha_ = 0;
	alphaMax_ = 0;
	isPreEnd_ = true;
	isEnd_ = true;

	tmpScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		true
	);
}

void Fader::Update(void)
{
	if (isEnd_)
	{
		return;
	}

	switch (state_)
	{
	case STATE::NONE:
	case STATE::FADE_KEEP:
		return;

	case STATE::FADE_OUT:
		alpha_ += SPEED_ALPHA;
		if (alpha_ > ALPHA_MAX)
		{
			// フェード終了
			alpha_ = ALPHA_MAX;
			if (isPreEnd_)
			{
				// 1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}

		break;

	case STATE::FADE_IN:
		alpha_ -= SPEED_SCENE;
		if (alpha_ < 0)
		{
			// フェード終了
			alpha_ = 0;
			if (isPreEnd_)
			{
				// 1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
		break;

	case STATE::SET_FADE_OUT:
		alpha_ += SPEED_ALPHA;
		if (alpha_ > alphaMax_)
		{
			// フェード終了
			alpha_ = LITTLE_ALPHA;
			if (isPreEnd_)
			{
				// 1フレーム後(Draw後)に終了とする
				isEnd_ = true;
			}
			isPreEnd_ = true;
		}
		break;

	default:
		return;
	}

}

void Fader::Draw(void)
{

	switch (state_)
	{
	case STATE::NONE:
		return;

	case STATE::FADE_KEEP:
	case STATE::SET_FADE_OUT:
	case STATE::FADE_OUT:
	case STATE::FADE_IN:
		CircleMask();
		break;
	}
}

void Fader::CircleMask()
{
	constexpr int ALPHA_SPEED = 4;

	// 描画領域をマスク画像領域に切り替える
	// 元々は、背面スクリーンになっている
	SetDrawScreen(tmpScreen_);

	//画面全体を黒に塗る
	DrawBox(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		AsoUtility::BLACK,
		true);

	//白色の円を描画する
	//alpha値を利用して大きさを制御
	DrawCircle(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		(ALPHA_MAX - alpha_) * ALPHA_SPEED,
		AsoUtility::WHITE,
		true);

	//描画領域を元に戻す
	SetDrawScreen(DX_SCREEN_BACK);

	//描画を色の乗算モードにする
	SetDrawBlendMode(DX_BLENDMODE_MUL, 0);

	//元々のゲーム画面にマスク画像を描画する
	DrawGraph(0, 0, tmpScreen_, false);

	//描画モードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}
