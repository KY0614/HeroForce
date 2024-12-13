#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/DataBank.h"
#include "../Scene/SelectScene.h"
#include "SelectImage.h"

SelectImage::SelectImage(SelectScene& select) : selectScene_(select)
{
	imgPlayerNum_ = nullptr;
	imgLeftPoint_ = -1;
	imgRightPoint_ = -1;

	playerNum_ = 1;
	isPad_ = false;
	role_ = 0;

	keyPressTime_ = 0.0f;
	interval_ = 0.0f;
	press_ = false;
}

void SelectImage::Destroy(void)
{
}

void SelectImage::Init(void)
{
	Load();

	pointL_ = { LEFT_POS_X,POINT_POS_Y,POINT_SCALE,POINT_SCALE,false,imgLeftPoint_ };	
	
	pointR_ = { RIGHT_POS_X,POINT_POS_Y,POINT_SCALE,POINT_SCALE,false,imgRightPoint_ };
}

void SelectImage::Update(void)
{
	switch (selectScene_.GetSelect())
	{
	case SelectScene::SELECT::NUMBER:
		NumberUpdate();
		break;

	case SelectScene::SELECT::OPERATION:
		OperationUpdate();
		break;

	case SelectScene::SELECT::ROLE:
		RoleUpdate();
		break;

	default:
		break;
	}
}

void SelectImage::Draw(void)
{
	//�����I�ɂ�DrawPolygon3D�ŕ`��

	switch (selectScene_.GetSelect())
	{
	case SelectScene::SELECT::NUMBER:
		NumberDraw();
		break;

	case SelectScene::SELECT::OPERATION:
		OperationDraw();
		break;

	case SelectScene::SELECT::ROLE:
		RoleDraw();
		break;

	default:
		break;
	}
	
	DrawFormatString(Application::SCREEN_SIZE_X - 100,0, 0x000000, "L : %d", pointL_.isToggle_);
	DrawFormatString(Application::SCREEN_SIZE_X - 100,20, 0x000000, "R : %d", pointR_.isToggle_);
}

void SelectImage::Update1(void)
{
	OperationUpdate();
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

void SelectImage::NumberUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	//�E�̖��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//�l�����P�ǉ�(���g��1�`4�Ɏ��߂�)
			playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1;
		}

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����₵�Ă���
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1 : interval_;
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//��
	if (pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//�l�����P�폜(���g��1�`4�Ɏ��߂�)
			playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM;
			if (playerNum_ == 0)playerNum_ = 4;
		}

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����炵�Ă���
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM : interval_;
			if (playerNum_ == 0)playerNum_ = 4;
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//�X�y�[�X�L�[�����Ō���&���̓f�o�C�X�I����
	if (selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		//�v���C���[�l���̐ݒ�
		data.Input(SceneManager::PLAY_MODE::USER, playerNum_);
		//�f�B�X�v���C�̐ݒ�
		data.Input(DataBank::INFO::DHISPLAY_NUM, playerNum_);
		data.Input(DataBank::INFO::USER_NUM, playerNum_);

		//CPU�l���̐ݒ�(CPU�͂P�l����R�l)
		data.Input(SceneManager::PLAY_MODE::CPU, (SceneManager::PLAYER_NUM-playerNum_));

		//�E�B���h�E�����̏���
		SceneManager::GetInstance().RedySubWindow();

		//�J�����̐ݒ�
		auto cameras = SceneManager::GetInstance().GetCameras();
		for (int i = 0; i < cameras.size(); i++)
		{ 
			cameras[i]->SetPos(SelectScene::DEFAULT_CAMERA_POS, SelectScene::DEFAULT_TARGET_POS);
			cameras[i]->ChangeMode(Camera::MODE::FIXED_POINT);
		}

		selectScene_.ChangeSelect(SelectScene::SELECT::OPERATION);
	}

	//�I��������
	if (!pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}
}

void SelectImage::OperationUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	data.Input(SceneManager::CNTL::PAD, 2);
	data.Input(SceneManager::CNTL::PAD, 3);
	data.Input(SceneManager::CNTL::PAD, 4);

	//�E�̖��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//�L�[���������ۂɑI�����Ă�����̂𔽓](2��ނ����Ȃ��̂�)
			isPad_ = !isPad_;
		}
		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����₵�Ă���
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, isPad_ = !isPad_ : interval_;
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//��
	if (pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//�L�[���������ۂɑI�����Ă�����̂𔽓](2��ނ����Ȃ��̂�)
			isPad_ = !isPad_;
		}
		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����炵�Ă���
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, isPad_ = !isPad_ : interval_;
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//�X�y�[�X�L�[�����Ō���&��E�I����
	if (selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		//1P�̑���̐ݒ�
		(isPad_) ? data.Input(SceneManager::CNTL::PAD, 1) : data.Input(SceneManager::CNTL::KEYBOARD, 1);
		//selectedCntl_ = (isPad_) ? SceneManager::CNTL::PAD : SceneManager::CNTL::KEYBOARD;

		selectScene_.ChangeSelect(SelectScene::SELECT::ROLE);
	}

	//�I��������
	if (!pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}
}

void SelectImage::RoleUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	//�E�̎O�p�`��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//��E��I��
			role_ = (role_ + 1) % SceneManager::PLAYER_NUM;
		}

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����₵�Ă���
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, role_ = (role_ + 1) % SceneManager::PLAYER_NUM : interval_;
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	if (pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//��E��I��
			role_ = (role_ - 1 + SceneManager::PLAYER_NUM) % SceneManager::PLAYER_NUM;
		}
		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����炵�Ă���
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, role_ = (role_ - 1 + SceneManager::PLAYER_NUM) % SceneManager::PLAYER_NUM : interval_;
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//�X�y�[�X�L�[�����ŃQ�[����ʂ�
	if (selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		//��E�̐ݒ�
		data.Input(static_cast<SceneManager::ROLE>(role_), playerNum_);

		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	//�I������O�p�`
	if (!pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}
}

void SelectImage::NumberDraw(void)
{
	//�l���I���摜�̕`��
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f, 0.0f,
		imgPlayerNum_[playerNum_ - 1],	//�z���0�`3�Ȃ̂�
		true, false);

	//���̖��̕`��(�E�̖��͈Â߂ɂ���)
	if (pointL_.isToggle_)
	{
		//�E�̖��摜��`�悵�A���Z�u�����h����
		pointR_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
		pointR_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//�u�����h���[�h��߂�

		pointL_.PointDraw();
	}
	//�E�̖��̕`��(���̖��͈Â߂ɂ���)
	if (pointR_.isToggle_)
	{
		//���̖��摜��`�悵�A���Z�u�����h����
		pointL_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
		pointL_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//�u�����h���[�h��߂�

		pointR_.PointDraw();
	}

	//�ǂ���̖����I��ł��Ȃ��Ƃ��̕`��
	if (!pointL_.isToggle_ && !pointR_.isToggle_)
	{
		//���ʂɕ`��
		pointL_.PointDraw();
		pointR_.PointDraw();
		//���̏�̉摜�Ɍ��Z�u�����h����
		SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
		pointL_.PointDraw();
		pointR_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "num : %d", playerNum_);
}

void SelectImage::OperationDraw(void)
{
	//�l���I���摜�̕`��
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f, 0.0f,
		imgPlayerNum_[(int)(isPad_)],	//�z���0�`3�Ȃ̂�
		true, false);

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "ope");
}

void SelectImage::RoleDraw(void)
{
	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "role");
}

void SelectImage::Point::PointDraw(void)
{
	DrawRotaGraph(pos.x, pos.y,
		POINT_SCALE_RATE, 0.0f,
		imgHandle_,
		true, false);
}