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

	//�摜
	img_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEVEL_UP).handleId_;
	if (img_ == -1)
	{
		return;
	}
}

void LevelupNotice::Update()
{
	float speed = 0.001f;	//�g��X�s�[�h;
	float max = 1.2f;		//�g��{��
	int timeLimit = 1.5 * SceneManager::DEFAULT_FPS;

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

	//�摜�̕`��
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
