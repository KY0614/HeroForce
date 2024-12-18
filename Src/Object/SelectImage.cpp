#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/DataBank.h"
#include "../Scene/SelectScene.h"
#include "../Object/Character/SelectPlayer.h"
#include "SelectImage.h"

SelectImage::SelectImage(SelectScene& select, std::shared_ptr<SelectPlayer> player) : selectScene_(select), player_(player)
{
	imgPlayerNum_ = nullptr;
	imgLeftPoint_ = -1;
	imgRightPoint_ = -1;

	//���W(�l�p�`����邽�߂�2�̎O�p�`���g��)
	vertices_[0].pos = VGet(VERTEX_LEFT_X, VERTEX_UNDER_Y, VERTEX_Z + 12.0f);	// ����
	vertices_[1].pos = VGet(VERTEX_RIGHT_X, VERTEX_UNDER_Y, VERTEX_Z + 12.0f);	// �E��
	vertices_[2].pos = VGet(VERTEX_LEFT_X, VERTEX_TOP_Y, VERTEX_Z);				// ����
	vertices_[3].pos = VGet(VERTEX_RIGHT_X, VERTEX_TOP_Y, VERTEX_Z);			// �E��

	//UV���W�i�e�N�X�`�����W�j
	vertices_[0].u = 0.0f / 4.0f;	vertices_[0].v = 1.0f;	// ����
	vertices_[1].u = 1.0f / 4.0f;	vertices_[1].v = 1.0f;	// �E��
	vertices_[2].u = 0.0f / 4.0f;	vertices_[2].v = 0.0f;	// ����
	vertices_[3].u = 1.0f / 4.0f;	vertices_[3].v = 0.0f;	// �E��


	// �@���̐ݒ�i����͏ȗ��A�K���Ȓl��ݒ�j
	for (int i = 0; i < VERTEX_NUM; i++) {
		vertices_[i].norm = VGet(0.0f, 0.0f, 1.0f);  // �@����-Z������
	}

	// �F�̐ݒ�i�f�B�t���[�Y�J���[�j
	for (int i = 0; i < VERTEX_NUM; i++) {
		vertices_[i].dif = GetColorU8(255, 255, 255, 255);  // ���F
	}

	// �F�̐ݒ�i�f�B�t���[�Y�J���[�j
	for (int i = 0; i < VERTEX_NUM; i++) {
		vertices_[i].spc = GetColorU8(0, 0, 0, 0);  // ���F
	}

	leftTop_ = AsoUtility::VECTOR_ZERO;
	leftBottom_ = AsoUtility::VECTOR_ZERO;
	rightTop_ = AsoUtility::VECTOR_ZERO;
	rightBottom_ = AsoUtility::VECTOR_ZERO;

	playerNum_ = 1;
	isPad_ = false;
	role_ = 0;

	keyPressTime_ = 0.0f;
	interval_ = 0.0f;
	press_ = false;
	angle_ = 0.0f;
}

void SelectImage::Destroy(void)
{
}

void SelectImage::Init(void)
{
	Load();

	InitVertex();

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
	//��
	//����
	DrawSphere3D(vertices_[0].pos, 10.0f, 10, 0xFF0000, 0xFF0000, false);
	//�E��
	DrawSphere3D(vertices_[1].pos, 10.0f, 10, 0x00FF00, 0x00FF00, false);
	//����
	DrawSphere3D(vertices_[2].pos, 10.0f, 10, 0x0000FF, 0x0000FF, false);
	//�E��
	DrawSphere3D(vertices_[3].pos, 10.0f, 10, 0x000000, 0x000000, false);
	
	for (int i = 0; i < 4; i++)
	{
		DrawFormatString(0, 40 + (20 * i), 0xFF0000, "pos %0.2f,%0.2f,%0.2f", vertices_[i].pos.x, vertices_[i].pos.y, vertices_[i].pos.z);
	}
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
		data.Input(SceneManager::PLAY_MODE::CPU, (SceneManager::PLAYER_NUM - playerNum_));

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

	//UV���W�i�e�N�X�`�����W�j
	vertices_[0].u = ((float)(playerNum_)-1.0f) / 4.0f;		vertices_[0].v = 1.0f;	// ����
	vertices_[1].u = (float)(playerNum_) / 4.0f;			vertices_[1].v = 1.0f;	// �E��
	vertices_[2].u = ((float)(playerNum_)-1.0f) / 4.0f;		vertices_[2].v = 0.0f;	// ����
	vertices_[3].u = (float)(playerNum_) / 4.0f;			vertices_[3].v = 0.0f;	// �E��
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

	//UV���W�i�e�N�X�`�����W�j
	vertices_[0].u = (float)(role_) / 4.0f;				vertices_[0].v = 1.0f;	// ����
	vertices_[1].u = ((float)(role_) + 1.0f) / 4.0f;	vertices_[1].v = 1.0f;	// �E��
	vertices_[2].u = (float)(role_) / 4.0f;				vertices_[2].v = 0.0f;	// ����
	vertices_[3].u = ((float)(role_) +1.0f) / 4.0f;		vertices_[3].v = 0.0f;	// �E��

	player_->SetRole(role_);
}

void SelectImage::NumberDraw(void)
{
	// 2�̎O�p�`��`��i�e�N�X�`���t���j
	// 1�ڂ̎O�p�`
	VERTEX3D triangle1[3] = { vertices_[2], vertices_[1], vertices_[0] };
	// 2�ڂ̎O�p�`
	VERTEX3D triangle2[3] = { vertices_[1], vertices_[2], vertices_[3] };

	// �O�p�`��`��
	DrawPolygon3D(triangle1, 2, *imgPlayerNum_, true);  // 1�ڂ̎O�p�`
	DrawPolygon3D(triangle2, 2, *imgPlayerNum_, true);  // 2�ڂ̎O�p�`

	////�l���I���摜�̕`��
	//DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
	//	Application::SCREEN_SIZE_Y / 2,
	//	1.0f, 0.0f,
	//	imgPlayerNum_[playerNum_ - 1],	//�z���0�`3�Ȃ̂�
	//	true, false);

	PointsDraw();

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "num : %d", playerNum_);

}

void SelectImage::OperationDraw(void)
{
	//���u��
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f, 0.0f,
		imgPlayerNum_[(int)(isPad_)],
		true, false);

	PointsDraw();

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "ope : %d",isPad_);
}

void SelectImage::RoleDraw(void)
{
	// 2�̎O�p�`��`��i�e�N�X�`���t���j
	// 1�ڂ̎O�p�`
	VERTEX3D triangle1[3] = { vertices_[2], vertices_[1], vertices_[0] };
	// 2�ڂ̎O�p�`
	VERTEX3D triangle2[3] = { vertices_[1], vertices_[2], vertices_[3] };

	// �O�p�`��`��
	DrawPolygon3D(triangle1, 2, *imgPlayerNum_, true);  // 1�ڂ̎O�p�`
	DrawPolygon3D(triangle2, 2, *imgPlayerNum_, true);  // 2�ڂ̎O�p�`

	////���u��
	//DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
	//	Application::SCREEN_SIZE_Y / 2,
	//	1.0f, 0.0f,
	//	imgPlayerNum_[(int)(role_)],
	//	true, false);

	PointsDraw();

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "role : %d",role_);
}

void SelectImage::PointsDraw(void)
{
	//���̖��̕`��(�E�̖��͈Â߂ɂ���)
	if (pointL_.isToggle_)
	{
		//�E�̖��摜��`�悵�A�����摜�����Z�u�����h����
		pointR_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
		pointR_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//�u�����h���[�h��߂�

		//���ʂɕ`��
		pointL_.PointDraw();
	}
	//�E�̖��̕`��(���̖��͈Â߂ɂ���)
	if (pointR_.isToggle_)
	{
		//���̖��摜��`�悵�A�����摜�����Z�u�����h����
		pointL_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
		pointL_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//�u�����h���[�h��߂�

		//���ʂɕ`��
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
}

void SelectImage::InitVertex(void)
{
	leftTop_ = { VERTEX_LEFT_X, VERTEX_TOP_Y, VERTEX_Z };
	leftBottom_ = { VERTEX_LEFT_X, VERTEX_UNDER_Y, VERTEX_Z /*+ 12.0f*/ };
	rightTop_ = { VERTEX_RIGHT_X, VERTEX_TOP_Y, VERTEX_Z };
	rightBottom_ = { VERTEX_RIGHT_X, VERTEX_UNDER_Y, VERTEX_Z /*+ 12.0f */};

	//���W(�l�p�`����邽�߂�2�̎O�p�`���g��)
	vertices_[0].pos = leftBottom_;		// ����
	vertices_[1].pos = rightBottom_;	// �E��
	vertices_[2].pos = leftTop_;		// ����
	vertices_[3].pos = rightTop_;		// �E��
}

VECTOR SelectImage::RotateVertex(VECTOR pos, VECTOR center, float angle)
{
	//���b�V���̒��S�_���v�Z(���͂Ƃ肠�������b�V���̐^�񒆂̍��W)
	VECTOR Center;
	Center.x = (VERTEX_RIGHT_X + VERTEX_LEFT_X) * 0.5f;
	Center.y = (VERTEX_TOP_Y + VERTEX_UNDER_Y) * 0.5f;
	Center.z = VERTEX_Z;

	//���a
	const float DIS = 50.0f;

	// ���Ԑi�s
	angle_ += SceneManager::GetInstance().GetDeltaTime();

	// ��]�s���K�p���Ē��_����]

	for (int i = 1; i < 4; i += 2)
	{
		//XZ���ʏ�ŉ�]������
		vertices_[i].pos.x = Center.x + (DIS * (sinf(angle_)));
		vertices_[i].pos.z = Center.z + (DIS * (cosf(angle_)));
	}
	for (int i = 0; i < 4; i += 2)
	{
		vertices_[i].pos.x = Center.x + (-DIS * (sinf(angle_)));
		vertices_[i].pos.z = Center.z + (-DIS * (cosf(angle_)));
	}
	return AsoUtility::VECTOR_ZERO;
}

void SelectImage::Point::PointDraw(void)
{
	DrawRotaGraph(pos.x, pos.y,
		POINT_SCALE_RATE, 0.0f,
		imgHandle_,
		true, false);
}