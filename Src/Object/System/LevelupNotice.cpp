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

	//�e�L�X�g
	auto& text_m = TextManager::GetInstance();
	using FONT_T = TextManager::FONT_TYPE;	//�ȗ��p
	text_ = text_m.TextLoad(TextManager::TEXTS::LEVEL_UP);

	//�t�H���g����
	font_ = CreateFontToHandle(
		text_m.GetFontName(FONT_T::LOGO).c_str(),
		FONT_SIZE,
		FONT_THICK);

	//�摜
	img_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEVEL_UP).handleId_;
	if (img_ == -1)
	{
		return;
	}
}

void LevelupNotice::Update()
{
	float speed = EXPANSION_RATE;	//�g��X�s�[�h;
	float max = EXPANSION_MAX;		//�g��{��
	int timeLimit = MAINTAIN_SECONDS * Application::DEFAULT_FPS;

	switch (state_)
	{
	case STATE::FADE_IN:
	//�摜�̃t�F�[�h����
	imgFader_->SetFade(Fader::STATE::FADE_OUT);
	imgFader_->Update();
	//�g�又��
	
	scl_ += speed;
	//�摜�T�C�Y���ő�ɂȂ�����
	if (scl_ >= max)
	{
		scl_ = max;
		state_ = STATE::MAINTAIN;
	}
		break;

	case STATE::FIN:
		break;

	case STATE::MAINTAIN:
		//�{���̓{�^���������Ă���
		//���̏����ֈڂ邪
		//��������J�E���g��
		cnt_ ++;	//�J�E���g�X�V
		if (cnt_ >= timeLimit)
		{
			state_ = STATE::FIN;
		}
		break;
	}
}

void LevelupNotice::Draw()
{
	//�`��ݒ�
	Vector2 pos =
	{ Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 };
	float angle = 0.0f;
	bool trans = true;
	bool reverse = false;
	int color = 0xffffff;
	int c = TextManager::CENTER_TEXT;
	int cH = TextManager::CENTER_TEXT_H;

	//�摜�̕`��
	imgFader_->Draw(img_, pos, scl_, angle, trans, reverse);

	//�e�L�X�g�`��
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
