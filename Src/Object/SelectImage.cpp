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

	state_ = SelectScene::SELECT::NUMBER;

	for (int n = 0; n < SceneManager::PLAYER_NUM; n++) {
		//���W(�l�p�`����邽�߂�2�̎O�p�`���g��)
		mesh_[n].vertex_[0].pos = VGet(VERTEX_LEFT_X, VERTEX_UNDER_Y, VERTEX_Z + 12.0f);	// ����
		mesh_[n].vertex_[1].pos = VGet(VERTEX_RIGHT_X, VERTEX_UNDER_Y, VERTEX_Z + 12.0f);	// �E��
		mesh_[n].vertex_[2].pos = VGet(VERTEX_LEFT_X, VERTEX_TOP_Y, VERTEX_Z);				// ����
		mesh_[n].vertex_[3].pos = VGet(VERTEX_RIGHT_X, VERTEX_TOP_Y, VERTEX_Z);			// �E��

		//UV���W�i�e�N�X�`�����W�j
		mesh_[n].vertex_[0].u = 0.0f / 4.0f;	mesh_[n].vertex_[0].v = 1.0f;	// ����
		mesh_[n].vertex_[1].u = 1.0f / 4.0f;	mesh_[n].vertex_[1].v = 1.0f;	// �E��
		mesh_[n].vertex_[2].u = 0.0f / 4.0f;	mesh_[n].vertex_[2].v = 0.0f;	// ����
		mesh_[n].vertex_[3].u = 1.0f / 4.0f;	mesh_[n].vertex_[3].v = 0.0f;	// �E��

	}

	//���W(�l�p�`����邽�߂�2�̎O�p�`���g��)
	roleMesh_.vertex_[0].pos = VGet(VERTEX_LEFT_X, VERTEX_UNDER_Y, VERTEX_Z + 12.0f);	// ����
	roleMesh_.vertex_[1].pos = VGet(VERTEX_RIGHT_X, VERTEX_UNDER_Y, VERTEX_Z + 12.0f);	// �E��
	roleMesh_.vertex_[2].pos = VGet(VERTEX_LEFT_X, VERTEX_TOP_Y, VERTEX_Z);				// ����
	roleMesh_.vertex_[3].pos = VGet(VERTEX_RIGHT_X, VERTEX_TOP_Y, VERTEX_Z);			// �E��

	roleMesh_.testVertex_[0].pos = VGet(-90.0f, 50.0f, VERTEX_Z + 12.0f);	// ����
	roleMesh_.testVertex_[1].pos = VGet(0.0f, 50.0f, VERTEX_Z + 12.0f);		// �E��
	roleMesh_.testVertex_[2].pos = VGet(-90.0f, 190.0f, VERTEX_Z);			// ����
	roleMesh_.testVertex_[3].pos = VGet(0.0f, 190.0f, VERTEX_Z);			// �E��

	// �@���̐ݒ�i����͏ȗ��A�K���Ȓl��ݒ�j
	for (int n = 0; n < SceneManager::PLAYER_NUM; n++) {
		for (int i = 0; i < VERTEX_NUM; i++) {
			mesh_[n].vertex_[i].norm = VGet(0.0f, 0.0f, 1.0f);  // �@����-Z������
			roleMesh_.vertex_[i].norm = VGet(0.0f, 0.0f, 1.0f);
			roleMesh_.testVertex_[i].norm = VGet(0.0f, 0.0f, 1.0f);
		}
	}

	// �F�̐ݒ�i�f�B�t���[�Y�J���[�j
	for (int n = 0; n < SceneManager::PLAYER_NUM; n++) {
		for (int i = 0; i < VERTEX_NUM; i++) {
			mesh_[n].vertex_[i].dif = GetColorU8(255, 255, 255, 255);  // ���F
			roleMesh_.vertex_[i].dif = GetColorU8(255, 255, 255, 255);
			roleMesh_.testVertex_[i].dif = GetColorU8(255, 255, 255, 255);
		}
	}

	// �F�̐ݒ�i�f�B�t���[�Y�J���[�j
	for (int n = 0; n < SceneManager::PLAYER_NUM; n++) {
		for (int i = 0; i < VERTEX_NUM; i++) {
			mesh_[n].vertex_[i].spc = GetColorU8(0, 0, 0, 0);  // ���F
			roleMesh_.vertex_[i].spc = GetColorU8(0, 0, 0, 0);  // ���F
			roleMesh_.testVertex_[i].spc = GetColorU8(0, 0, 0, 0);  // ���F
		}
	}

	leftTop_ = AsoUtility::VECTOR_ZERO;
	leftBottom_ = AsoUtility::VECTOR_ZERO;
	rightTop_ = AsoUtility::VECTOR_ZERO;
	rightBottom_ = AsoUtility::VECTOR_ZERO;

	playerNum_ = 1;
	isPad_ = false;
	role_ = 0;
	isReady_ = false;

	keyPressTime_ = 0.0f;
	interval_ = 0.0f;
	press_ = false;
	angle_ = 0.0f;

	for (auto& t : target_)
	{
		t = AsoUtility::VECTOR_ZERO;
	}

	// ��ԊǗ�
	stateChanges_.emplace(
		SelectScene::SELECT::NUMBER, std::bind(&SelectImage::ChangeStateNumber, this));
	stateChanges_.emplace(
		SelectScene::SELECT::OPERATION, std::bind(&SelectImage::ChangeStateOperation, this));
	stateChanges_.emplace(
		SelectScene::SELECT::ROLE, std::bind(&SelectImage::ChangeStateRole, this));

}

void SelectImage::Destroy(void)
{
}

void SelectImage::Init(void)
{
	Load();

	InitVertex();

	//���摜�̏����l
	pointL_ = { LEFT_POS_X,POINT_POS_Y,POINT_SCALE,POINT_SCALE,false,imgLeftPoint_ };	
	
	pointR_ = { RIGHT_POS_X,POINT_POS_Y,POINT_SCALE,POINT_SCALE,false,imgRightPoint_ };

	target_[0] = SelectScene::DEFAULT_TARGET_POS;

	//�l���I������
	ChangeSelect(SelectScene::SELECT::NUMBER);
}

void SelectImage::Update(void)
{
	//�X�V�X�e�b�v
	stateUpdate_();
}

void SelectImage::Draw(void)
{
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
	DrawFormatString(Application::SCREEN_SIZE_X - 100,40, 0x000000, "num : %d", playerNum_);
	DrawFormatString(Application::SCREEN_SIZE_X - 100,80, 0x000000, "pad : %d", isPad_);

	//auto camera = SceneManager::GetInstance().GetCameras();
	//DrawFormatString(Application::SCREEN_SIZE_X - 100, 120, 0x000000, "camera : %d", camera.size());
	
	//��
	//����
	DrawSphere3D(roleMesh_.vertex_[0].pos, 10.0f, 10, 0xFF0000, 0xFF0000, false);
	//�E��
	DrawSphere3D(roleMesh_.vertex_[1].pos, 10.0f, 10, 0x00FF00, 0x00FF00, false);
	//����
	DrawSphere3D(roleMesh_.vertex_[2].pos, 10.0f, 10, 0x0000FF, 0x0000FF, false);
	//�E��
	DrawSphere3D(roleMesh_.vertex_[3].pos, 10.0f, 10, 0x000000, 0x000000, false);
	
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
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
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
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
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
	if (selectScene_.GetConfig() == SelectScene::KEY_CONFIG::DECIDE)
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
		for (int i = 1; i < cameras.size(); i++)
		{
			//�ŏ��̍��W��ێ�
			VECTOR prevPos = target_[i - 1];
			//XZ���ʂō��W����]������
			target_[i] = AsoUtility::RotXZPos(SelectScene::DEFAULT_CAMERA_POS, prevPos, AsoUtility::Deg2RadF(90.0f));

			cameras[i]->SetPos(SelectScene::DEFAULT_CAMERA_POS, target_[i]);
			cameras[i]->ChangeMode(Camera::MODE::FIXED_POINT);
		}

		//������ǉ������4�l�v���C�����Ȃ�����ʕ������Q�Ɍ���
		//if (playerNum_ > 1)
		//{
		//	//�v���C���[2�ȏ�̏ꍇ�A2P�ȏ�̃R���g���[���[��PAD����ɐݒ�
		//	for (int num = 2; num <= playerNum_; num++)
		//	{
		//		data.Input(SceneManager::CNTL::PAD, num);
		//	}
		//}

		//selectScene_.ChangeSelect(SelectScene::SELECT::OPERATION);
		ChangeSelect(SelectScene::SELECT::OPERATION);
	}

	//�I��������
	if (!pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	for (int n = 0; n < SceneManager::PLAYER_NUM; n++) {
		//UV���W�i�e�N�X�`�����W�j
		mesh_[n].vertex_[0].u = ((float)(playerNum_)-1.0f) / 4.0f;		mesh_[n].vertex_[0].v = 1.0f;	// ����
		mesh_[n].vertex_[1].u = (float)(playerNum_) / 4.0f;				mesh_[n].vertex_[1].v = 1.0f;	// �E��
		mesh_[n].vertex_[2].u = ((float)(playerNum_)-1.0f) / 4.0f;		mesh_[n].vertex_[2].v = 0.0f;	// ����
		mesh_[n].vertex_[3].u = (float)(playerNum_) / 4.0f;				mesh_[n].vertex_[3].v = 0.0f;	// �E��
	}
}

void SelectImage::OperationUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	//�E�̖��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
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
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
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
	if (selectScene_.GetConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		//1P�̑���̐ݒ�
		(isPad_) ? data.Input(SceneManager::CNTL::PAD, 1) : data.Input(SceneManager::CNTL::KEYBOARD, 1);
		(isPad_) ? selectScene_.Set1PDevice(SceneManager::CNTL::PAD) : selectScene_.Set1PDevice(SceneManager::CNTL::KEYBOARD);
		//selectedCntl_ = (isPad_) ? SceneManager::CNTL::PAD : SceneManager::CNTL::KEYBOARD;

		selectScene_.ChangeSelect(SelectScene::SELECT::ROLE);
		ChangeSelect(SelectScene::SELECT::ROLE);
	}

	//�I��������
	if (!pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	for (int n = 0; n < SceneManager::PLAYER_NUM; n++) {
		//UV���W�i�e�N�X�`�����W�j
		mesh_[n].vertex_[0].u = (float)(isPad_) / 4.0f;				mesh_[n].vertex_[0].v = 1.0f;	// ����
		mesh_[n].vertex_[1].u = ((float)(isPad_)+1.0f) / 4.0f;		mesh_[n].vertex_[1].v = 1.0f;	// �E��
		mesh_[n].vertex_[2].u = (float)(isPad_) / 4.0f;				mesh_[n].vertex_[2].v = 0.0f;	// ����
		mesh_[n].vertex_[3].u = ((float)(isPad_)+1.0f) / 4.0f;		mesh_[n].vertex_[3].v = 0.0f;	// �E��
	}
}

void SelectImage::RoleUpdate(void)
{
	
}

void SelectImage::NumberDraw(void)
{
	//// 2�̎O�p�`��`��i�e�N�X�`���t���j
	//// 1�ڂ̎O�p�`
	//VERTEX3D triangle1[3] = { vertices_[2], vertices_[1], vertices_[0] };
	//// 2�ڂ̎O�p�`
	//VERTEX3D triangle2[3] = { vertices_[1], vertices_[2], vertices_[3] };

	//// �O�p�`��`��
	//DrawPolygon3D(triangle1, 2, *imgPlayerNum_, true);  // 1�ڂ̎O�p�`
	//DrawPolygon3D(triangle2, 2, *imgPlayerNum_, true);  // 2�ڂ̎O�p�`

	mesh_[0].DrawTwoMesh(*imgPlayerNum_);

	PointsDraw();

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "num : %d", playerNum_);

}

void SelectImage::OperationDraw(void)
{
	mesh_[0].DrawTwoMesh(*imgPlayerNum_);

	PointsDraw();

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "ope : %d",isPad_);
}

void SelectImage::RoleDraw(void)
{
	// 2�̎O�p�`��`��i�e�N�X�`���t���j
	// 1�ڂ̎O�p�`
	//VERTEX3D triangle1[3] = { vertices_[2], vertices_[1], vertices_[0] };
	//// 2�ڂ̎O�p�`
	//VERTEX3D triangle2[3] = { vertices_[1], vertices_[2], vertices_[3] };

	//// �O�p�`��`��
	//DrawPolygon3D(triangle1, 2, *imgPlayerNum_, true);  // 1�ڂ̎O�p�`
	//DrawPolygon3D(triangle2, 2, *imgPlayerNum_, true);  // 2�ڂ̎O�p�`

	auto camera = SceneManager::GetInstance().GetCameras();

	//for (int i = 1; i < camera.size(); i++)
	//{
	//	for (int n = 0; n < 4; n++) {
	//		//�ŏ��̍��W��ێ�
	//		VECTOR prevPos = mesh_[i - 1].vertex_[n].pos;
	//		//XZ���ʂō��W����]������
	//		mesh_[i].vertex_[n].pos = AsoUtility::RotXZPos(SelectScene::DEFAULT_CAMERA_POS, prevPos, AsoUtility::Deg2RadF(90.0f));
	//	}
	//	mesh_[i].DrawTwoMesh(*imgPlayerNum_);
	//}

	//mesh_[0].DrawTwoMesh(*imgPlayerNum_);

	if (GetReady() != true)
	{
		roleMesh_.DrawTwoMeshTest(*imgPlayerNum_);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
		roleMesh_.DrawTwoMeshTest(*imgPlayerNum_);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//�u�����h���[�h��߂�
	}

	//PointsDraw();
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

void SelectImage::ChangeObject(SelectScene::Device& input, int i)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	//�E�̎O�p�`��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (pointR_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::RIGHT)
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
		input.config_ == SelectScene::KEY_CONFIG::LEFT)
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
	if (input.config_ == SelectScene::KEY_CONFIG::DECIDE)
	{
		//��E�̐ݒ�
		data.Input(static_cast<SceneManager::ROLE>(role_), i + 1);

		//SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);

		isReady_ = true;
	}

	//�I��������
	if (!pointR_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	//for (int n = 0; n < SceneManager::PLAYER_NUM; n++)
	//{
	//	//UV���W�i�e�N�X�`�����W�j
	//	mesh_[n].vertex_[0].u = (float)(role_) / 4.0f;			mesh_[n].vertex_[0].v = 1.0f;	// ����
	//	mesh_[n].vertex_[1].u = ((float)(role_)+1.0f) / 4.0f;	mesh_[n].vertex_[1].v = 1.0f;	// �E��
	//	mesh_[n].vertex_[2].u = (float)(role_) / 4.0f;			mesh_[n].vertex_[2].v = 0.0f;	// ����
	//	mesh_[n].vertex_[3].u = ((float)(role_)+1.0f) / 4.0f;	mesh_[n].vertex_[3].v = 0.0f;	// �E��
	//}

	//UV���W�i�e�N�X�`�����W�j
	//roleMesh_.vertex_[0].u = (float)(role_) / 4.0f;				roleMesh_.vertex_[0].v = 1.0f;	// ����
	//roleMesh_.vertex_[1].u = ((float)(role_)+1.0f) / 4.0f;		roleMesh_.vertex_[1].v = 1.0f;	// �E��
	//roleMesh_.vertex_[2].u = (float)(role_) / 4.0f;				roleMesh_.vertex_[2].v = 0.0f;	// ����
	//roleMesh_.vertex_[3].u = ((float)(role_)+1.0f) / 4.0f;		roleMesh_.vertex_[3].v = 0.0f;	// �E��

																								//UV���W�i�e�N�X�`�����W�j
	roleMesh_.testVertex_[0].u = (float)(role_) / 4.0f;			roleMesh_.testVertex_[0].v = 1.0f;	// ����
	roleMesh_.testVertex_[1].u = ((float)(role_)+1.0f) / 4.0f;	roleMesh_.testVertex_[1].v = 1.0f;	// �E��
	roleMesh_.testVertex_[2].u = (float)(role_) / 4.0f;			roleMesh_.testVertex_[2].v = 0.0f;	// ����
	roleMesh_.testVertex_[3].u = ((float)(role_)+1.0f) / 4.0f;	roleMesh_.testVertex_[3].v = 0.0f;	// �E��

	//player_->SetRole(role_);
}

void SelectImage::ChangeSelect(const SelectScene::SELECT _state)
{
	//��ԑJ��
	state_ = _state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();
}

VERTEX3D SelectImage::GetMeshVertex(int i)
{
	if (selectScene_.GetSelect() == SelectScene::SELECT::ROLE)
	{
		return roleMesh_.testVertex_[i];
	}
	return roleMesh_.vertex_[i];
}

void SelectImage::SetMeshPos(VECTOR pos, int i)
{
	roleMesh_.vertex_[i].pos = pos;
	roleMesh_.testVertex_[i].pos = pos;
}

void SelectImage::InitVertex(void)
{
	leftTop_ = { VERTEX_LEFT_X, VERTEX_TOP_Y, VERTEX_Z };
	leftBottom_ = { VERTEX_LEFT_X, VERTEX_UNDER_Y, VERTEX_Z /*+ 12.0f*/ };
	rightTop_ = { VERTEX_RIGHT_X, VERTEX_TOP_Y, VERTEX_Z };
	rightBottom_ = { VERTEX_RIGHT_X, VERTEX_UNDER_Y, VERTEX_Z /*+ 12.0f */};

	////���W(�l�p�`����邽�߂�2�̎O�p�`���g��)
	//vertices_[0].pos = leftBottom_;		// ����
	//vertices_[1].pos = rightBottom_;	// �E��
	//vertices_[2].pos = leftTop_;		// ����
	//vertices_[3].pos = rightTop_;		// �E��

	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		//���W(�l�p�`����邽�߂�2�̎O�p�`���g��)
		mesh_[i].vertex_[0].pos = leftBottom_;		// ����
		mesh_[i].vertex_[1].pos = rightBottom_;		// �E��
		mesh_[i].vertex_[2].pos = leftTop_;			// ����
		mesh_[i].vertex_[3].pos = rightTop_;		// �E��
	}
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

	//for (int i = 1; i < 4; i += 2)
	//{
		//XZ���ʏ�ŉ�]������
	//	vertices_[i].pos.x = Center.x + (DIS * (sinf(angle_)));
	//	vertices_[i].pos.z = Center.z + (DIS * (cosf(angle_)));
	//}
	//for (int i = 0; i < 4; i += 2)
	//{
	//	vertices_[i].pos.x = Center.x + (-DIS * (sinf(angle_)));
	//	vertices_[i].pos.z = Center.z + (-DIS * (cosf(angle_)));
	//}
	return AsoUtility::VECTOR_ZERO;
}

void SelectImage::ChangeStateNumber(void)
{
	stateUpdate_ = std::bind(&SelectImage::NumberUpdate, this);
}

void SelectImage::ChangeStateOperation(void)
{
	stateUpdate_ = std::bind(&SelectImage::OperationUpdate, this);
}

void SelectImage::ChangeStateRole(void)
{
	stateUpdate_ = std::bind(&SelectImage::RoleUpdate, this);
}

void SelectImage::Point::PointDraw(void)
{
	DrawRotaGraph(pos.x, pos.y,
		POINT_SCALE_RATE, 0.0f,
		imgHandle_,
		true, false);
}

void SelectImage::Mesh::DrawTwoMesh(int handle)
{
	// 2�̎O�p�`��`��i�e�N�X�`���t���j
	// 1�ڂ̎O�p�`
	VERTEX3D triangle1[3] = { vertex_[2], vertex_[1], vertex_[0] };
	// 2�ڂ̎O�p�`
	VERTEX3D triangle2[3] = { vertex_[1], vertex_[2], vertex_[3] };

	// �O�p�`��`��
	DrawPolygon3D(triangle1, 2, handle, true);  // 1�ڂ̎O�p�`
	DrawPolygon3D(triangle2, 2, handle, true);  // 2�ڂ̎O�p�`
}

void SelectImage::Mesh::DrawTwoMeshTest(int handle)
{
	// 2�̎O�p�`��`��i�e�N�X�`���t���j
	// 1�ڂ̎O�p�`
	VERTEX3D triangle1[3] = { testVertex_[2], testVertex_[1], testVertex_[0] };
	// 2�ڂ̎O�p�`
	VERTEX3D triangle2[3] = { testVertex_[1], testVertex_[2], testVertex_[3] };

	// �O�p�`��`��
	DrawPolygon3D(triangle1, 2, handle, true);  // 1�ڂ̎O�p�`
	DrawPolygon3D(triangle2, 2, handle, true);  // 2�ڂ̎O�p�`
}
