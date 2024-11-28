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
#include "SelectScene.h"

SelectScene::SelectScene(void)
{

}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	// �J�������[�h�F��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	//�l���I������
	ChangeSelect(SELECT::NUMBER);

	KeyConfigSetting();

	key_ = KEY_CONFIG::NONE;

	ChangeDevice(SceneManager::CNTL::KEYBOARD);

	playerNum_ = 1;

	isPad_ = false;

	role_ = 0;

	//�}�`�p------------------------------------------------

	//�O�p�`�̒��S���W�Ƒ傫��
	triL = { 450,TRI_POS_Y ,TRI_SCALE,TRI_SCALE ,false };
	triR = { 1050,TRI_POS_Y ,TRI_SCALE,TRI_SCALE ,false };

	//�O�p�`�̐F
	triL.color_ = GetColor(255, 255, 64);
	triR.color_ = GetColor(255, 255, 64);

	//�l�p�`�̒��S���W�Ƒ傫��
	rc = { 750,450,RECT_SCALE,RECT_SCALE };

	//�l�p�`�̐F
	rc.color_ = GetColor(255, 0, 0);

	//�O�p�`�̕`����W
	triL.pos.x = rc.pos.x - TRI_SCALE - PRI_SPACE;
	triR.pos.x = rc.pos.x + TRI_SCALE + PRI_SPACE;

	//------------------------------------------------------

	keyPressTime_ = 0.0f;
	interval_ = 0.0f;
}

void SelectScene::Update(void)
{
	//�L�[�̐ݒ�
	KeyConfigSetting();

	//�ǂ��炩�𑀍삵�Ă���Ƃ��ɂ����Е��𑀍�ł��Ȃ��悤�ɐ���
	ControllDevice();

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
}

void SelectScene::Draw(void)
{
	auto& ins = InputManager::GetInstance();
	InputManager::JOYPAD_IN_STATE pad = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	//�f�o�b�O�`��
	DrawDebug();
}

void SelectScene::Release(void)
{
}

void SelectScene::NumberUpdate(void)
{
	InputManager& ins = InputManager::GetInstance();
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

#ifdef DEBUG_RECT
	int PRI_SPACE = 100;
	int RECT_SCALE = 300;
	int TRI_SCALE = 150;

	////�l�p�`�̍��W�Ƒ傫���A�F�����߂�
	//rc = { 750,450,RECT_SCALE,RECT_SCALE };
	//rc.color_ = GetColor(255, 0, 0);

	//�O�p�`�̕`����W
	triL.pos.x = rc.pos.x - TRI_SCALE - PRI_SPACE;
	triR.pos.x = rc.pos.x + TRI_SCALE + PRI_SPACE;

	//�O�p�`�̃{�^����I�𒆂�������΂ɔ�I���������物�F��
	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);

	//�I�𒆂ɓ��������̃L�[��������񉟂��Ɛl�������Z
	if (triR.isToggle_&&
		GetKeyConfig() == KEY_CONFIG::RIGHT_TRG)
	{
		//�l�����P�ǉ�
		playerNum_ += 1;
	}

	//�E�̎O�p�`��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
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
				interval_ = 0.0f ,playerNum_ += 1: interval_;
		}
	}
	else if(triR.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
	}

	//�I�𒆂ɓ��������̃L�[��������񉟂��Ɛl�������Z
	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT_TRG)
	{
		//�l�����P�팸
		playerNum_ -= 1;
	}

	//��
	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
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
				interval_ = 0.0f, playerNum_ -= 1 : interval_;
		}
	}
	else if (triL.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
	}

	//�v���C�l���͈͓̔��ɐ��l�����߂�
	playerNum_ = std::clamp(playerNum_, 1, SceneManager::PLAYER_NUM );	//�P�`�S�l�v���C�Ȃ̂�1�`4�܂�

	//�X�y�[�X�L�[�����Ō���&���̓f�o�C�X�I����
	if (key_ == KEY_CONFIG::DECIDE)
	{
		//�v���C���[�l���̐ݒ�
		data.Input(SceneManager::PLAY_MODE::USER, playerNum_);

		//CPU�l���̐ݒ�(CPU�͂P�l����R�l)
		data.Input(SceneManager::PLAY_MODE::CPU, (SceneManager::PLAYER_NUM) - playerNum_);

		//���������Ƃ��̐F
		rc.color_ = 0xFF0000;
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

#ifdef DEBUG_RECT
	int PRI_SPACE = 100;
	int RECT_SCALE = 300;
	int TRI_SCALE = 150;

	//�l�p�`�̍��W�Ƒ傫���A�F�����߂�
	rc = { 750,450,RECT_SCALE,RECT_SCALE };
	rc.color_ = GetColor(255, 0, 0);

	//�O�p�`�̕`����W
	triL.pos.x = rc.pos.x - TRI_SCALE - PRI_SPACE;
	triR.pos.x = rc.pos.x + TRI_SCALE + PRI_SPACE;

	//�O�p�`�̃{�^�����I�𒆂�������΂ɔ�I���������物�F��
	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);


	//�I�𒆂ɓ��������̃L�[��������񉟂��ƑI���f�o�C�X��ύX
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT_TRG)
	{
		//�L�[�{�[�h��I��
		isPad_ = true;
	}

	//�E�̎O�p�`��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
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
	}

	//�I�𒆂ɓ��������̃L�[��������񉟂��ƑI���f�o�C�X��ύX
	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT_TRG)
	{
		//�L�[�{�[�h��I��
		isPad_ = false;
	}

	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
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
	int PRI_SPACE = 100;
	int RECT_SCALE = 300;
	int TRI_SCALE = 150;

	//�l�p�`�̍��W�Ƒ傫���A�F�����߂�
	rc = { 750,450,RECT_SCALE,RECT_SCALE };
	rc.color_ = GetColor(255, 0, 0);

	//�O�p�`�̕`����W
	triL.pos.x = rc.pos.x - TRI_SCALE - PRI_SPACE;
	triR.pos.x = rc.pos.x + TRI_SCALE + PRI_SPACE;

	//�O�p�`�̃{�^�����I�𒆂�������΂ɔ�I���������物�F��
	triL.color_ = (triL.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);
	triR.color_ = (triR.isToggle_) ? GetColor(128, 168, 128) : GetColor(255, 255, 64);

	//�I�𒆂ɓ��������̃L�[��������񉟂��ƑI���E��ύX
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT_TRG)
	{
		//��E��I��
		role_ += 1;
	}

	//�E�̎O�p�`��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (triR.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::RIGHT)
	{
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
				interval_ = 0.0f, role_ += 1 : interval_;
		}
	}
	else if (triR.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
	}

	//�I�𒆂ɓ��������̃L�[��������񉟂��ƑI���E��ύX
	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT_TRG)
	{
		//��E��I��
		role_ -= 1;
	}

	if (triL.isToggle_ &&
		GetKeyConfig() == KEY_CONFIG::LEFT)
	{
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
				interval_ = 0.0f, role_ -= 1 : interval_;
		}
	}
	else if (triL.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
	}

	//��E���͈͓̔��ɐ��l�����߂�
	role_ = std::clamp(role_, 0, static_cast<int>(SceneManager::ROLE::ARCHER) + 1);	//�S��E�Ȃ̂�0�`3�܂�

	//�X�y�[�X�L�[�����ŃQ�[����ʂ�
	if (GetKeyConfig() == KEY_CONFIG::DECIDE)
	{
		//��E�̐ݒ�
		data.Input(static_cast<SceneManager::ROLE>(role_),playerNum_);

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

void SelectScene::DrawDebug(void)
{
	InputManager& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();

#ifdef DEBUG_RECT
	//�I��p�̎l�p�`�ƑI�����Ă����ނ܂��͐�����\������
	rc.Draw(rc.color_);

	switch (select_)
	{
	case SelectScene::SELECT::NUMBER:

		DrawFormatString(rc.pos.x, rc.pos.y,
			0xFFFFFF, "%d", playerNum_);

		DrawFormatString(Application::SCREEN_SIZE_X / 2 - 200, 0,
			0xFF9999, "�v���C�l���I��");

		DrawFormatString(0, 0,
			0xFFFFFF, "time : %.2f", keyPressTime_);

		break;

	case SelectScene::SELECT::OPERATION:
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

		break;

	case SelectScene::SELECT::ROLE:
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

		break;

	default:
		break;
	}
#endif // DEBUG_RECT

	//���݂̓��̓f�o�C�X
	DrawFormatString(0, 820, 0xFFFFFF, "(key_:0)(pad:1) %d", GetDevice());
	//���͂̎��
	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0xFFFFFF,
		"%d",
		key_);

	//�v���C�l��
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 0, 0xFFFFFF, "number : %d", playerNum_);
	//1P���p�b�h���삩�ǂ���
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 20, 0xFFFFFF, "operation : %d", isPad_);
	//��E
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 40, 0xFFFFFF, "role_ : %d", role_);

	// ���X�e�B�b�N�̉���
	int leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;
	//�c��
	int leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;

	DrawFormatString(Application::SCREEN_SIZE_X / 2, 60, 0xFFFFFF, "stickX : %d", leftStickX_);
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 80, 0xFFFFFF, "stickY : %d", leftStickY_);


	//�O�p�`�`��
	triL.LeftDraw(triL.color_);
	triR.RightDraw(triR.color_);
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
		if (ins.IsNew(KEY_INPUT_UP) ||
			ins.IsNew(KEY_INPUT_W))
		{
			key_ = KEY_CONFIG::UP;
		}
		if (ins.IsNew(KEY_INPUT_DOWN) ||
			ins.IsNew(KEY_INPUT_S))
		{
			key_ = KEY_CONFIG::DOWN;
		}
		if (ins.IsNew(KEY_INPUT_LEFT) ||
			ins.IsNew(KEY_INPUT_A))
		{
			key_ = KEY_CONFIG::LEFT;
		}
		if (ins.IsNew(KEY_INPUT_RIGHT) ||
			ins.IsNew(KEY_INPUT_D))
		{
			key_ = KEY_CONFIG::RIGHT;
		}

		//�L�[�̉�������(�������u�Ԃ���)
		if (ins.IsTrgDown(KEY_INPUT_UP) ||
			ins.IsTrgDown(KEY_INPUT_W))
		{
			key_ = KEY_CONFIG::UP_TRG;
		}
		if (ins.IsTrgDown(KEY_INPUT_DOWN) ||
			ins.IsTrgDown(KEY_INPUT_S))
		{
			key_ = KEY_CONFIG::DOWN_TRG;
		}
		if (ins.IsTrgDown(KEY_INPUT_LEFT) ||
			ins.IsTrgDown(KEY_INPUT_A))
		{
			key_ = KEY_CONFIG::LEFT_TRG;
		}
		if (ins.IsTrgDown(KEY_INPUT_RIGHT) ||
			ins.IsTrgDown(KEY_INPUT_D))
		{
			key_ = KEY_CONFIG::RIGHT_TRG;
		}

		if (ins.IsTrgDown(KEY_INPUT_SPACE))
		{
			key_ = KEY_CONFIG::DECIDE;
		}
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
		if (leftStickX_ < -1)
		{
			key_ = KEY_CONFIG::LEFT;
		}
		if (leftStickX_ > 1)
		{
			key_ = KEY_CONFIG::RIGHT;
		}

		//
		if (leftStickY_ < -1)
		{
			key_ = KEY_CONFIG::UP;

		}
		if (leftStickY_ > 1)
		{
			key_ = KEY_CONFIG::DOWN;
		}
		if (leftStickX_ < -1)
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
	//1P�̑���I����ł�������g�p�f�o�C�X���Œ�(�Ƃ肠����)
	if (selectedCntl_ == SceneManager::CNTL::KEYBOARD)
	{
		device_ = SceneManager::CNTL::KEYBOARD;
	}
	else if(selectedCntl_ == SceneManager::CNTL::PAD)
	{
		device_ = SceneManager::CNTL::PAD;
	}

	return device_;
}

SelectScene::KEY_CONFIG SelectScene::GetKeyConfig(void)
{
	return key_;
}

void SelectScene::ChangeDevice(SceneManager::CNTL device)
{
	device_ = device;
}

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