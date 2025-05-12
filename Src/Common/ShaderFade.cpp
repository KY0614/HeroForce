#include <DxLib.h>
#include "../Application.h"
#include "../Shader/PixelShader.h"
#include "ShaderFade.h"

ShaderFade::ShaderFade()
{
	imgFade_ = -1;
}

ShaderFade::~ShaderFade()
{
}

void ShaderFade::Init()
{
	Fader::Init();
	imgFade_ = LoadGraph("Data/Image/Fader.png");
}

void ShaderFade::Draw()
{
	switch (state_)
	{
	case STATE::NONE:
		return;

	case STATE::FADE_KEEP:
	case STATE::SET_FADE_OUT:
	case STATE::FADE_OUT:
	case STATE::FADE_IN:

		auto& ds = PixelShader::GetInstance();
		COLOR_F buf = COLOR_F{ alpha_ / 255 };
		ds.DrawExtendGraphToShader(
			{ 0,0 },
			{ Application::SCREEN_SIZE_X,Application::SCREEN_SIZE_Y },
			imgFade_,
			PixelShader::PS_TYPE::FADE,
			buf
		);
		break;
	}
}
