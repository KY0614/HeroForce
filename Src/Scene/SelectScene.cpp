#include <vector>
#include <math.h>
#include<algorithm>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Manager/DataBank.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/StageObject.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/Character/SelectPlayer.h"
#include "../Object/SelectImage.h"
#include "SelectScene.h"

SelectScene::SelectScene(void)
{

}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	//�X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();
	
	//�w�i�p�X�e�[�W
	stage_ = new StageManager();
	stage_->Init();

	//�w�i�F�𔒂�
	SetBackgroundColor(255, 255, 255);
	//�w�i�̃X�e�[�W���f�����𔼓�����
	//float alpha = 0.5f;
	//MV1SetOpacityRate(skyDome_->GetTransform().modelId, alpha);
	//for (int i = 0; i < StageManager::MODELS; i++) {
	//	for (auto& s : stage_->GetTtans(static_cast<StageManager::MODEL_TYPE>(i)))
	//	{
	//		MV1SetOpacityRate(s.modelId, alpha);
	//	}
	//}

	//�t�H�O�̐ݒ�
	SetFogEnable(true);
	//��
	SetFogColor(255, 255, 255);
	SetFogStartEnd(-10000.0f, 15000.0f);

	//�v���C���[�ݒ�
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		players_[i] = std::make_unique<SelectPlayer>();
		players_[i]->Init();
	}

	//�摜�ݒ�
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		images_[i] = std::make_unique<SelectImage>(*this);
		images_[i]->Init();
	}

	// �J�������[�h�F��_�J����
	auto camera = SceneManager::GetInstance().GetCameras();
	camera[0]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
	camera[0]->ChangeMode(Camera::MODE::FIXED_POINT);

	//�l���I������
	ChangeSelect(SELECT::NUMBER);

	KeyConfigSetting();

	key_ = KEY_CONFIG::NONE;

	ChangeDevice(SceneManager::CNTL::KEYBOARD);

	//playerNum_ = 1;

	isPad_ = false;

	role_ = 0;

	//�}�`�p------------------------------------------------

	//�O�p�`�̒��S���W�Ƒ傫��
	triL = { TRI_POS_X - 300,TRI_POS_Y ,TRI_SCALE,TRI_SCALE ,false };
	triR = { TRI_POS_X + 300,TRI_POS_Y ,TRI_SCALE,TRI_SCALE ,false };

	//�O�p�`�̐F
	triL.color_ = GetColor(255, 255, 64);
	triR.color_ = GetColor(255, 255, 64);

	//�l�p�`�̒��S���W�Ƒ傫��
	rc = { Application::SCREEN_SIZE_X/2/*750*/,Application::SCREEN_SIZE_Y / 2/*450*/,RECT_SCALE,RECT_SCALE };

	//�l�p�`�̐F
	rc.color_ = GetColor(255, 0, 0);

	//�O�p�`�̕`����W
	triL.pos.x = rc.pos.x - TRI_SCALE - PRI_SPACE;
	triR.pos.x = rc.pos.x + TRI_SCALE + PRI_SPACE;

	//------------------------------------------------------

	//keyPressTime_ = 0.0f;
	//interval_ = 0.0f;

}

void SelectScene::Update(void)
{
	//�L�[�̐ݒ�
	KeyConfigSetting();

	//�ǂ��炩�𑀍삵�Ă���Ƃ��ɂ����Е��𑀍�ł��Ȃ��悤�ɐ���
	ControllDevice();

	//�����]
	skyDome_->Update();

	//�I�𒆂̎�ނ��Ƃ̍X�V����
	switch (select_)
	{
	case SELECT::NUMBER:  
		NumberUpdate();
		break;

	case SELECT::OPERATION:
		OperationUpdate();
		break;

	case SELECT::ROLE:
		RoleUpdate();
		break;

	default:
		break;
	}

	trans_.Update();
	for (auto& i : tests_)
	{
		i.Update();
	}
}

void SelectScene::Draw(void)
{
	auto& ins = InputManager::GetInstance();

	skyDome_->Draw();
	stage_->Draw();

	//�f�o�b�O�`��
	DrawDebug();

	switch (select_)
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

	for (auto& i : images_)
	{
		i->Draw();
	}
}

void SelectScene::Release(void)
{
	SceneManager::GetInstance().ResetCameras();

	for (auto i : tests_)
	{
		MV1DeleteModel(i.modelId);
	}
	MV1DeleteModel(trans_.modelId);
}

void SelectScene::InitModel(void)
{
	//�Q�����z��ɂ��Ă��ꂼ��Ή�������H


}

void SelectScene::NumberUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	for (auto& i : images_)
	{
		i->Update();
	}

#ifdef DEBUG_RECT

	//�O�p�`�̃{�^����I�𒆂�������΂ɔ�I���������物�F��
	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);

	//�E�̎O�p�`��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//�l�����P�ǉ�(���g��1�`4�Ɏ��߂�)
			playerNum_ = (playerNum_ % PLAYER_NUM) + 1;
		}
		
		//�F�𔒂�
		triR.color_ = GetColor(255, 255, 255);

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME )
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����₵�Ă���
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f , playerNum_ = (playerNum_ % PLAYER_NUM) + 1 : interval_;
		}
	}
	else if(triR.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//��
	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//�l�����P�폜(���g��1�`4�Ɏ��߂�)
			playerNum_ = (playerNum_ + 3) % PLAYER_NUM;
			if (playerNum_ == 0)playerNum_ = 4;
		}
		//�F�𔒂�
		triL.color_ = GetColor(255, 255, 255);

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����炵�Ă���
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, playerNum_ = (playerNum_ + 3) % PLAYER_NUM : interval_;
			if (playerNum_ == 0)playerNum_ = 4;
		}
	}
	else if (triL.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//�v���C�l���͈͓̔��ɐ��l�����߂�
	//playerNum_ = std::clamp(playerNum_, 1, PLAYER_NUM );	//�P�`�S�l�v���C�Ȃ̂�1�`4�܂�

	//�X�y�[�X�L�[�����Ō���&���̓f�o�C�X�I����
	if (key_ == KEY_CONFIG::DECIDE)
	{
		//�v���C���[�l���̐ݒ�
		data.Input(SceneManager::PLAY_MODE::USER, playerNum_);
		//�f�B�X�v���C�̐ݒ�
		data.Input(DataBank::INFO::DHISPLAY_NUM, playerNum_);
		data.Input(DataBank::INFO::USER_NUM, playerNum_);	

		//CPU�l���̐ݒ�(CPU�͂P�l����R�l)
		data.Input(SceneManager::PLAY_MODE::CPU, (PLAYER_NUM) - playerNum_);

		//�E�B���h�E�����̏���
		SceneManager::GetInstance().RedySubWindow();

		//�J�����̐ݒ�
		auto cameras = SceneManager::GetInstance().GetCameras();
		for (int i = 0; i < playerNum_; i++)
		{
			cameras[i]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
			cameras[i]->SetFollow(&players_[i]->GetTransform());
			cameras[i]->ChangeMode(Camera::MODE::FIXED_POINT);
		}

		ChangeSelect(SELECT::OPERATION);
	}

	//�I������O�p�`
	if (!triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		triR.isToggle_ = true;
		triL.isToggle_ = false;
	}

	if (!triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		triR.isToggle_ = false;
		triL.isToggle_ = true;
	}


#endif // DEBUG_RECT

}

void SelectScene::OperationUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	data.Input(SceneManager::CNTL::PAD, 2);
	data.Input(SceneManager::CNTL::PAD, 3);
	data.Input(SceneManager::CNTL::PAD, 4);

#ifdef DEBUG_RECT
	//�O�p�`�̃{�^�����I�𒆂�������΂ɔ�I���������物�F��
	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);

	//�E�̎O�p�`��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//�L�[�{�[�h��I��
			isPad_ = true;
		}

		//�F�𔒂�
		triR.color_ = GetColor(255, 255, 255);

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����₵�Ă���
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, isPad_ = true : interval_;
		}
	}
	else if (triR.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//�L�[�{�[�h��I��
			isPad_ = false;
		}

		//�F�𔒂�
		triL.color_ = GetColor(255, 255, 255);

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����炵�Ă���
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, isPad_ = false : interval_;
		}
	}
	else if (triL.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//�X�y�[�X�L�[�����Ō���&��E�I����
	if (GetKeyConfig() == KEY_CONFIG::DECIDE)
	{
		//1P�̑���̐ݒ�
		(isPad_) ? data.Input(SceneManager::CNTL::PAD, 1) : data.Input(SceneManager::CNTL::KEYBOARD, 1);
		selectedCntl_ = (isPad_) ? SceneManager::CNTL::PAD : SceneManager::CNTL::KEYBOARD;
		
		//���������Ƃ��̐F
		rc.color_ = 0xFF0000;

		ChangeSelect(SELECT::ROLE);
	}

	//�I������O�p�`
	if (!triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		triR.isToggle_ = true;
		triL.isToggle_ = false;
	}

	if (!triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		triR.isToggle_ = false;
		triL.isToggle_ = true;
	}

#endif // DEBUG_RECT

}

void SelectScene::RoleUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

#ifdef DEBUG_RECT
	//�O�p�`�̃{�^�����I�𒆂�������΂ɔ�I���������物�F��
	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);

	//�E�̎O�p�`��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//��E��I��
			role_ = (role_ + 1) % PLAYER_NUM;
		}

		//�F�𔒂�
		triR.color_ = GetColor(255, 255, 255);

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����₵�Ă���
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, role_ = (role_ + 1) % PLAYER_NUM : interval_;
		}
	}
	else if (triR.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//��E��I��
			role_ = (role_ - 1 + PLAYER_NUM) % PLAYER_NUM;
		}
		//�F�𔒂�
		triL.color_ = GetColor(255, 255, 255);

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����炵�Ă���
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, role_ = (role_ - 1 + PLAYER_NUM) % PLAYER_NUM : interval_;
		}
	}
	else if (triL.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//��E���͈͓̔��ɐ��l�����߂�
	//role_ = std::clamp(role_, 0, static_cast<int>(SceneManager::ROLE::ARCHER) + 1);	//�S��E�Ȃ̂�0�`3�܂�

	//�X�y�[�X�L�[�����ŃQ�[����ʂ�
	if (GetKeyConfig() == KEY_CONFIG::DECIDE)
	{
		//��E�̐ݒ�
		data.Input(static_cast<SceneManager::ROLE>(role_),playerNum_);

		//���������Ƃ��̐F
		rc.color_ = 0xFF0000;

		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	//�I������O�p�`
	if (!triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
		triR.isToggle_ = true;
		triL.isToggle_ = false;
	}

	if (!triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
		triR.isToggle_ = false;
		triL.isToggle_ = true;
	}

#endif // DEBUG_RECT

}

void SelectScene::NumberDraw(void)
{
#ifdef DEBUG_RECT

	DrawFormatString(rc.pos.x, rc.pos.y,
		0xFFFFFF, "%d", playerNum_);

	DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200, 0,
		0xFF9999, "�v���C�l���I��");

	DrawFormatString(0, 0,
		0xFFFFFF, "time : %.2f", keyPressTime_);
#endif // DEBUG_RECT
}

void SelectScene::OperationDraw(void)
{
#ifdef DEBUG_RECT
	for (int i = 0; i < playerNum_; i++) {
		if (!isPad_)
		{
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "key");
		}
		else
		{
			DrawFormatString(rc.pos.x, rc.pos.y,
				0xFFFFFF, "pad");
		}

		DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200, 0,
			0xFF9999, "1P������@�I��");
	}

#endif // DEBUG_RECT

}

void SelectScene::RoleDraw(void)
{
#ifdef DEBUG_RECT
	if (role_ > static_cast<int>(SceneManager::ROLE::MAGE))
	{
		DrawFormatString(rc.pos.x, rc.pos.y,
			0xFFFFFF, "ARCHER");
	}
	else if (role_ > static_cast<int>(SceneManager::ROLE::AXEMAN))
	{
		DrawFormatString(rc.pos.x, rc.pos.y,
			0xFFFFFF, "MAGE");
	}
	else if (role_ > static_cast<int>(SceneManager::ROLE::KNIGHT))
	{
		DrawFormatString(rc.pos.x, rc.pos.y,
			0xFFFFFF, "AXEMAN");
	}
	else
	{
		DrawFormatString(rc.pos.x, rc.pos.y,
			0xFFFFFF, "KNIGHT");
	}

	DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200, 0,
		0x99FF99, "��E�I��");


	for (auto& p : players_)
	{
		p->SetRole(role_);
		p->Draw();
	}
#endif // DEBUG_RECT

}

void SelectScene::DrawDebug(void)
{
	InputManager& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();

#ifdef DEBUG_RECT
	//�I��p�̎l�p�`�ƑI�����Ă����ނ܂��͐�����\������
	rc.Draw(rc.color_);

	//�O�p�`�`��
	triL.LeftDraw(triL.color_);
	triR.RightDraw(triR.color_);

#endif // DEBUG_RECT

	//���݂̓��̓f�o�C�X
	DrawFormatString(0, 820, 0x000000, "(key_:0)(pad:1) %d", GetDevice());
	//���͂̎��
	DrawFormatString(0,
		800,
		0x000000,
		"%d",
		key_);

	////�v���C�l��
	//DrawFormatString(Application::SCREEN_SIZE_X / 2, 0, 0x000000, "number : %d", playerNum_);
	//1P���p�b�h���삩�ǂ���
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 20, 0x000000, "operation : %d", isPad_);
	//��E
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 40, 0x000000, "role_ : %d", role_);

	DrawFormatString(Application::SCREEN_SIZE_X / 2, 100, 0x000000, "pos : %2.f,%2.f,%2.f", trans_.pos.x,trans_.pos.y,trans_.pos.z);

	// ���X�e�B�b�N�̉���
	int leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;
	//�c��
	int leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;

	DrawFormatString(Application::SCREEN_SIZE_X / 2, 60, 0x000000, "stickX : %d", leftStickX_);
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 80, 0x000000, "stickY : %d", leftStickY_);

}

void SelectScene::ChangeSelect(SELECT select)
{
	select_ = select;
}

void SelectScene::KeyConfigSetting(void)
{
	auto& ins = InputManager::GetInstance();

	// ����������ĂȂ��Ƃ��͈ړ����Ȃ��悤��
	key_ = KEY_CONFIG::NONE;
	
	// ���X�e�B�b�N�̉���
	int leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;

	//�c��
	int leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;

	//0���X�e�B�b�N�������Ă��Ȃ����
	
	//							//�X�e�B�b�N���ő�܂œ|�����Ƃ��̐��l
	int stickXRight = 900;		//0�`1000
	int stickXLeft = -900;		//0�`-1000
	int stickYDown = 900;		//0�`1000
	int stickYUp = -900;		//0�`-1000

	//�X�e�B�b�N���|����Ă��邩�ǂ���
	bool isStickMoved = (leftStickX_ != 0 || leftStickY_ != 0);
	bool isStickPressed = false; // �X�e�B�b�N���|���ꂽ���ǂ����̃t���O
	bool lastStickState = false; // �Ō�̃X�e�B�b�N��ԁi�|����Ă��邩�ǂ����j

	switch (GetDevice())
	{
	case SceneManager::CNTL::KEYBOARD:
		ChangeDevice(SceneManager::CNTL::KEYBOARD);

		//�L�[�̉�������
		if (ins.IsNew(KEY_INPUT_UP)		||	ins.IsNew(KEY_INPUT_W))	key_ = KEY_CONFIG::UP;
		if (ins.IsNew(KEY_INPUT_DOWN)	||	ins.IsNew(KEY_INPUT_S))	key_ = KEY_CONFIG::DOWN;
		if (ins.IsNew(KEY_INPUT_LEFT)	||	ins.IsNew(KEY_INPUT_A))	key_ = KEY_CONFIG::LEFT;
		if (ins.IsNew(KEY_INPUT_RIGHT)	||	ins.IsNew(KEY_INPUT_D))	key_ = KEY_CONFIG::RIGHT;

		//�L�[�̉�������(�������u�Ԃ���)
		if (ins.IsTrgDown(KEY_INPUT_UP) ||	ins.IsTrgDown(KEY_INPUT_W))key_ = KEY_CONFIG::UP_TRG;
		if (ins.IsTrgDown(KEY_INPUT_DOWN) ||ins.IsTrgDown(KEY_INPUT_S))key_ = KEY_CONFIG::DOWN_TRG;
		if (ins.IsTrgDown(KEY_INPUT_LEFT) ||ins.IsTrgDown(KEY_INPUT_A))key_ = KEY_CONFIG::LEFT_TRG;
		if (ins.IsTrgDown(KEY_INPUT_RIGHT)||ins.IsTrgDown(KEY_INPUT_D))key_ = KEY_CONFIG::RIGHT_TRG;

		if (ins.IsTrgDown(KEY_INPUT_SPACE))key_ = KEY_CONFIG::DECIDE;
		break;

	case SceneManager::CNTL::PAD:
		ChangeDevice(SceneManager::CNTL::PAD);
		if (leftStickY_ < -1)
		{
			key_ = KEY_CONFIG::UP;
			
		}
		if (leftStickY_ > 1)
		{
			key_ = KEY_CONFIG::DOWN;
		}
		if (leftStickX_ < stickXLeft)
		{
			key_ = KEY_CONFIG::LEFT;
		}
		if (leftStickX_ > 1)
		{
			key_ = KEY_CONFIG::RIGHT;
		}

		if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
		{
			key_ = KEY_CONFIG::DECIDE;
		}

		break;
	default:
		break;
	}
}

SceneManager::CNTL SelectScene::GetDevice(void)
{
	//�Ԃ�l�p��ret���ŉ^�p���邱��
	//1P�̑���I����ł�������g�p�f�o�C�X���Œ�(�Ƃ肠����)
	SceneManager::CNTL ret;
	if (selectedCntl_ == SceneManager::CNTL::KEYBOARD)	ret = SceneManager::CNTL::KEYBOARD;
	else if(selectedCntl_ == SceneManager::CNTL::PAD)	ret = SceneManager::CNTL::PAD;
	
	return ret;

	//if (selectedCntl_ == SceneManager::CNTL::KEYBOARD)
	//{
	//	ChangeDevice(SceneManager::CNTL::KEYBOARD);
	//	return SceneManager::CNTL::KEYBOARD;
	//}
	//ChangeDevice(SceneManager::CNTL::PAD);
	//return SceneManager::CNTL::PAD;
}

void SelectScene::ChangeDevice(SceneManager::CNTL device)
{
	device_ = device;
}

void SelectScene::ControllDevice(void)
{
	InputManager& ins = InputManager::GetInstance();
	int key_ = CheckHitKeyAll(DX_CHECKINPUT_KEY);		//���͂𒲂ׂ�(�L�[�{�[�h)
	int padNum = GetJoypadNum();
	int padState = CheckHitKeyAll(DX_CHECKINPUT_PAD);	//���͂𒲂ׂ�(�p�b�h)

	//�L�[�{�[�h����̎��p�b�h������ł��Ȃ��悤��
	if (key_ != 0	&&
		padState == 0)
	{
		ChangeDevice(SceneManager::CNTL::KEYBOARD);
	}//�p�b�h����̎��L�[������ł��Ȃ��悤��
	else if (key_ <= 0 &&
		padNum > 0 &&
		padState != 0)
	{
		ChangeDevice(SceneManager::CNTL::PAD);
	}
}

//�f�o�b�O�p�̐}�`�`��------------------------------------------------------------------------

bool SelectScene::IsHitRect(Rect& rc, Vector2 pos, int r)
{
	auto diffX = pos.x - rc.pos.x;	//�I�_����n�_������
	auto diffY = pos.y - rc.pos.y;
	if (fabsf(diffX) > r + rc.w / 2 ||
		fabsf(diffY) > r + rc.h / 2) {
		return false;
	}
	else {
		return true;
	}
}

void SelectScene::Rect::Draw(unsigned int color)
{
	DxLib::DrawBox(
		(int)Left(),
		(int)Top() ,
		(int)Right() ,
		(int)Bottom(), color, true);
}

void SelectScene::Tri::LeftDraw(unsigned int color)
{
	DxLib::DrawTriangle(
		(int)LeftX_L(),
		(int)LeftY_L(),
		(int)TopX_L(),
		(int)TopY_L(),
		(int)RightX_L(),
		(int)RightY_L(),
		 color, true);
}

void SelectScene::Tri::RightDraw(unsigned int color)
{
	DxLib::DrawTriangle(
		(int)LeftX_R(),
		(int)LeftY_R(),
		(int)TopX_R(),
		(int)TopY_R(),
		(int)RightX_R(),
		(int)RightY_R(),
		color, true);
}