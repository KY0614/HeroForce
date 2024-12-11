#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/DataBank.h"
#include "../Scene/SelectScene.h"
#include "SelectImage.h"

SelectImage::SelectImage(SelectScene& select) : selectScene_(select)
{
	imgLeftPoint_ = -1;
	imgRightPoint_ = -1;

	playerNum_ = 1;
}

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
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f, 0.0f,
		imgPlayerNum_[playerNum_ + 1],	//�z���0�`3�Ȃ̂�
		true, false);

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "%d", playerNum_);

	DrawRotaGraph(Application::SCREEN_SIZE_X / 2 + (POINT_SCALE * 5), Application::SCREEN_SIZE_Y / 2,
		POINT_SCALE_RATE, 0.0f,
		imgRightPoint_,
		true, false);

	DrawRotaGraph(Application::SCREEN_SIZE_X / 2 - (POINT_SCALE * 5), Application::SCREEN_SIZE_Y / 2,
		POINT_SCALE_RATE, 0.0f,
		imgLeftPoint_,
		true, false);
}

void SelectImage::Load(void)
{
	//�摜
	imgPlayerNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_NUM).handleIds_;

	imgRightPoint_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RIGHT_POINT).handleId_;
	if (imgRightPoint_ == -1)
	{
		return;
	}

	imgLeftPoint_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEFT_POINT).handleId_;
	if (imgLeftPoint_ == -1)
	{
		return;
	}
}

//void SelectImage::NumberUpdate(void)
//{
//	InputManager& ins = InputManager::GetInstance();
//	DataBank& data = DataBank::GetInstance();
//	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();
//
//	//�E�̖��ON�̎��ɃL�[�̉E�ɒl������͂����������
//	if (/*triR.isToggle_ &&*/
//		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
//	{
//		if (!press_)
//		{
//			press_ = true;
//
//			//�l�����P�ǉ�(���g��1�`4�Ɏ��߂�)
//			playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1;
//		}
//
//		//�F�𔒂�
//		triR.color_ = GetColor(255, 255, 255);
//
//		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
//		keyPressTime_ += delta;
//
//		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
//		if (keyPressTime_ > SELECT_TIME)
//		{
//			//�C���^�[�o�������Z���Ă���
//			interval_ += delta;
//
//			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����₵�Ă���
//			(interval_ > INTERVAL_TIME) ?
//				interval_ = 0.0f, playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1 : interval_;
//		}
//	}
//	else if (triR.isToggle_)
//	{
//		keyPressTime_ = 0.0f;
//		interval_ = INTERVAL_TIME;
//		press_ = false;
//	}
//
//	//��
//	if (/*triL.isToggle_ &&*/
//		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
//	{
//		if (!press_)
//		{
//			press_ = true;
//
//			//�l�����P�폜(���g��1�`4�Ɏ��߂�)
//			playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM;
//			if (playerNum_ == 0)playerNum_ = 4;
//		}
//		//�F�𔒂�
//		triL.color_ = GetColor(255, 255, 255);
//
//		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
//		keyPressTime_ += delta;
//
//		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
//		if (keyPressTime_ > SELECT_TIME)
//		{
//			//�C���^�[�o�������Z���Ă���
//			interval_ += delta;
//
//			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����炵�Ă���
//			(interval_ > INTERVAL_TIME) ?
//				interval_ = 0.0f, playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM : interval_;
//			if (playerNum_ == 0)playerNum_ = 4;
//		}
//	}
//	else if (triL.isToggle_)
//	{
//		keyPressTime_ = 0.0f;
//		interval_ = INTERVAL_TIME;
//		press_ = false;
//	}
//
//	//�X�y�[�X�L�[�����Ō���&���̓f�o�C�X�I����
//	if (selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::DECIDE)
//	{
//		//�v���C���[�l���̐ݒ�
//		data.Input(SceneManager::PLAY_MODE::USER, playerNum_);
//		//�f�B�X�v���C�̐ݒ�
//		data.Input(DataBank::INFO::DHISPLAY_NUM, playerNum_);
//		data.Input(DataBank::INFO::USER_NUM, playerNum_);
//
//		//CPU�l���̐ݒ�(CPU�͂P�l����R�l)
//		data.Input(SceneManager::PLAY_MODE::CPU, (SceneManager::PLAYER_NUM-playerNum_));
//
//		//�E�B���h�E�����̏���
//		SceneManager::GetInstance().RedySubWindow();
//
//		//�J�����̐ݒ�
//		auto cameras = SceneManager::GetInstance().GetCameras();
//		for (int i = 0; i < playerNum_; i++)
//		{
//			cameras[i]->SetPos(SelectScene::DEFAULT_CAMERA_POS, SelectScene::DEFAULT_TARGET_POS);
//			//cameras[i]->SetFollow(&players_[i]->GetTransform());
//			cameras[i]->ChangeMode(Camera::MODE::FIXED_POINT);
//		}
//
//		selectScene_.ChangeSelect(SelectScene::SELECT::OPERATION);
//	}
//
//	//�I������O�p�`
//	if (!triR.isToggle_ &&
//		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
//	{
//		triR.isToggle_ = true;
//		triL.isToggle_ = false;
//	}
//
//	if (!triL.isToggle_ &&
//		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
//	{
//		triR.isToggle_ = false;
//		triL.isToggle_ = true;
//	}
//}

void SelectImage::OperationUpdate(void)
{
}

void SelectImage::RoleUpdate(void)
{
}
