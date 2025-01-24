//#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Shader/PixelShader.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgLogo_ = -1;
	imgMes_ = -1;
	sky_ = nullptr;
}

void TitleScene::Init(void)
{
	//�X�J�C�h�[��
	sky_ = std::make_unique<SkyDome>();
	sky_->Init();

	//�摜�ǂݍ���
	imgLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_LOGO).handleId_;
	imgMes_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLEASE_KEY).handleId_;

	//�J�����ݒ�
	auto camera = SceneManager::GetInstance().GetCameras();
	camera[0]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
	camera[0]->ChangeMode(Camera::MODE::FIXED_POINT);
}

void TitleScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	SceneManager& mng = SceneManager::GetInstance();

	//�J�E���g�X�V
	step_ += mng.GetDeltaTime();

	//�X�J�C�h�[���X�V
	sky_->Update();

	// �V�[���J��
	if (ins.IsTrgDown(KEY_INPUT_SPACE)|| 
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD3, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD4, InputManager::JOYPAD_BTN::RIGHT))
	{
		mng.ChangeScene(SceneManager::SCENE_ID::SELECT);
	}
}

void TitleScene::Draw(void)
{
	//�X�J�C�h�[���`��
	sky_->Draw();

	//���S�̕`��
	DrawRotaGraph(
		LOGO_POS_X,LOGO_POS_Y,
		0.5f,
		0.0f,
		imgLogo_,
		true,
		false);

	auto& ps = PixelShader::GetInstance();

	//���b�Z�[�W�`��
	COLOR_F buf = COLOR_F();//= COLOR_F{ step_ };
	buf.r = step_;

	

	/*ps.DrawExtendGraphToShader(
		{ MES_POS_X ,MES_POS_Y },
		imgMes_,
		PixelShader::PS_TYPE::YELLOW_BLINK,
		buf);*/
	ps.DrawExtendGraphToShader(
		{ MES_POS_X,10 },
		{ Application::SCREEN_SIZE_X, 200},
		imgMes_,
		PixelShader::PS_TYPE::YELLOW_BLINK,
		buf
	);

	/*DrawRotaGraph(
		MES_POS_X,MES_POS_Y,
		1.0f,
		0.0f,
		imgMes_,
		true,
		false);*/
}

void TitleScene::Release(void)
{
	sky_->Release();
}
