#include <DxLib.h>
#include "../Application.h"
#include "ImageFader.h"

void ImageFader::Draw(int img, Vector2 pos, float scl, float angle, bool trans, bool reverse)
{
	switch (state_)
	{
	case STATE::NONE:
		return;

	case STATE::FADE_OUT:
	case STATE::FADE_IN:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
		DrawRotaGraph(
			pos.x, pos.y,
			scl,
			angle,
			img,
			trans,
			reverse);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}
}
