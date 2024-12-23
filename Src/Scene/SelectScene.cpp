#include <vector>
#include <math.h>
#include<algorithm>
#include "../Application.h"
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
	key_ = KEY_CONFIG::NONE;
	device_ = SceneManager::CNTL::KEYBOARD;
	select_ = SELECT::NUMBER;
	selectedCntl_ = SceneManager::CNTL::NONE;

	// ��ԊǗ�
	stateChanges_.emplace(
		SELECT::NUMBER, std::bind(&SelectScene::ChangeStateNumber, this));
	stateChanges_.emplace(
		SELECT::OPERATION, std::bind(&SelectScene::ChangeStateOperation, this));
	stateChanges_.emplace(
		SELECT::ROLE, std::bind(&SelectScene::ChangeStateRole, this));
	stateChanges_.emplace(
		SELECT::ROLE, std::bind(&SelectScene::ChangeStateMax, this));
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
	float alpha = 0.5f;
	MV1SetOpacityRate(skyDome_->GetTransform().modelId, alpha);
	for (int i = 0; i < StageManager::MODELS; i++) {
		for (auto& s : stage_->GetTtans(static_cast<StageManager::MODEL_TYPE>(i)))
		{
			MV1SetOpacityRate(s.modelId, alpha);
		}
	}

	//�t�H�O�̐ݒ�
	SetFogEnable(false);
	//��
	SetFogColor(255, 255, 255);
	SetFogStartEnd(-300.0f, 15000.0f);

	//�v���C���[�ݒ�
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		players_[i] = std::make_shared<SelectPlayer>();
		players_[i]->Init();
	}

	//�摜�ݒ�
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		images_[i] = std::make_unique<SelectImage>(*this, *players_);
		images_[i]->Init();
	}
	//image_ = std::make_unique<SelectImage>(*this);
	//image_->Init();

	// �J�������[�h�F��_�J����
	auto camera = SceneManager::GetInstance().GetCameras();
	camera[0]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
	camera[0]->ChangeMode(Camera::MODE::FIXED_POINT);

	//�l���I������
	ChangeSelect(SELECT::NUMBER);

	KeyConfigSetting();

	key_ = KEY_CONFIG::NONE;

	//ChangeDevice(SceneManager::CNTL::KEYBOARD);

}

void SelectScene::Update(void)
{
	//�L�[�̐ݒ�
	KeyConfigSetting();

	//�ǂ��炩�𑀍삵�Ă���Ƃ��ɂ����Е��𑀍�ł��Ȃ��悤�ɐ���
	ControllDevice();

	//�����]
	skyDome_->Update();

	//for (auto& i : images_)
	//{
	//	i->Update();
	//}
	//image_->Update();

	//�I�𒆂̎�ނ��Ƃ̍X�V����
	//switch (select_)
	//{
	//case SELECT::NUMBER:  
	//	NumberUpdate();
	//	break;

	//case SELECT::OPERATION:
	//	OperationUpdate();
	//	break;

	//case SELECT::ROLE:
	//	RoleUpdate();
	//	break;

	//default:
	//	break;
	//}

	trans_.Update();
	for (auto& i : tests_)
	{
		i.Update();
	}

	//�X�V�X�e�b�v
	stateUpdate_();
}

void SelectScene::Draw(void)
{
	auto& ins = InputManager::GetInstance();

	skyDome_->Draw();
	stage_->Draw();

	//�f�o�b�O�`��
	//DrawDebug();

	//�I�𒆂̎�ނ��Ƃ̍X�V����
	switch (select_)
	{
	case SELECT::NUMBER:
		NumberDraw();
		break;

	case SELECT::OPERATION:
		OperationDraw();
		break;

	case SELECT::ROLE:
		RoleDraw();
		break;

	default:
		break;
	}
}

void SelectScene::Release(void)
{
	SceneManager::GetInstance().ResetCameras();

	//image_->Destroy();

	for (auto i : tests_)
	{
		MV1DeleteModel(i.modelId);
	}
	MV1DeleteModel(trans_.modelId);
}

void SelectScene::ChangeStateNumber(void)
{
	stateUpdate_ = std::bind(&SelectScene::NumberUpdate, this);
}

void SelectScene::ChangeStateOperation(void)
{
	stateUpdate_ = std::bind(&SelectScene::OperationUpdate, this);
}

void SelectScene::ChangeStateRole(void)
{
	stateUpdate_ = std::bind(&SelectScene::RoleUpdate, this);
}

void SelectScene::ChangeStateMax(void)
{
	stateUpdate_ = std::bind(&SelectScene::MaxUpdate, this);
}

void SelectScene::NumberUpdate(void)
{
	images_[0]->NumberUpdate();

}

void SelectScene::OperationUpdate(void)
{
	for (auto& i : images_)
	{
		i->OperationUpdate();
	}
}

void SelectScene::RoleUpdate(void)
{
	for (int i = 0; i < 4;i++)
	{
		images_[i]->Update();
		images_[i]->ChangeObject(devices_[i], images_[i].get(), i);
	}
}

void SelectScene::MaxUpdate(void)
{
}

void SelectScene::NumberDraw(void)
{
	images_[0]->Draw();
}

void SelectScene::OperationDraw(void)
{
	for (auto& i : images_)
	{
		i->Draw();
	}
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
	//for (auto& i : images_)
	//{
	//	i->Draw();
	//}

	players_[0]->Draw();
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
	//DrawFormatString(0, 820, 0x000000, "(key_:0)(pad:1) %d", GetDevice());
	//���͂̎��
	DrawFormatString(0,
		800,
		0x000000,
		"%d",
		key_);

	////�v���C�l��
	//DrawFormatString(Application::SCREEN_SIZE_X / 2, 0, 0x000000, "number : %d", playerNum_);
	////1P���p�b�h���삩�ǂ���
	//DrawFormatString(Application::SCREEN_SIZE_X / 2, 20, 0x000000, "operation : %d", isPad_);
	//��E
	//DrawFormatString(Application::SCREEN_SIZE_X / 2, 40, 0x000000, "role_ : %d", role_);

	DrawFormatString(Application::SCREEN_SIZE_X / 2, 100, 0x000000, "pos : %2.f,%2.f,%2.f", trans_.pos.x,trans_.pos.y,trans_.pos.z);

	// ���X�e�B�b�N�̉���
	int leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;
	//�c��
	int leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;

	DrawFormatString(Application::SCREEN_SIZE_X / 2, 60, 0x000000, "stickX : %d", leftStickX_);
	DrawFormatString(Application::SCREEN_SIZE_X / 2, 80, 0x000000, "stickY : %d", leftStickY_);

}

void SelectScene::ChangeSelect(const SELECT _state)
{
	//��ԑJ��
	select_ = _state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[select_]();
}

void SelectScene::KeyConfigSetting(void)
{
	auto& ins = InputManager::GetInstance();

	// ����������ĂȂ��Ƃ�
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
	if (device_ == SceneManager::CNTL::KEYBOARD)	ret = SceneManager::CNTL::KEYBOARD;
	else if(device_ == SceneManager::CNTL::PAD)	ret = SceneManager::CNTL::PAD;
	
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
