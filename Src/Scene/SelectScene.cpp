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

	key_ = KEY_CONFIG::NONE;
	ChangeDevice(SceneManager::CNTL::NONE);

	Set1PDevice(SceneManager::CNTL::NONE);
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

	//for (auto& p : players_)
	//{
	//	p->Update();
	//}

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

	//�f�o�b�O�`��
	DrawDebug();
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
	images_[0]->Update();
}

void SelectScene::OperationUpdate(void)
{
	images_[0]->Update();
}

void SelectScene::RoleUpdate(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();
	for (int i = 0; i < camera.size(); i++)
	{
		images_[i]->Update();
		images_[i]->ChangeObject(input_[i], i);
		players_[i]->SetRole(images_[i]->GetRole());
	}
	//�L�����N�^�[�̈ʒu�ƌ�����ݒ�
	for (int i = 1; i < camera.size(); i++)
	{
		players_[i]->Update();
		players_[i]->SetPos(AsoUtility::RotXZPos(DEFAULT_CAMERA_POS, players_[i - 1]->GetPos(), AsoUtility::Deg2RadF(90.0f)));
		players_[i]->SetRot(Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-90.0f * i), 0.0f));
	}
}

void SelectScene::MaxUpdate(void)
{
	//�������Ȃ�
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
	auto camera = SceneManager::GetInstance().GetCameras();

	for (int i = 0; i < camera.size(); i++)
	{
		images_[i]->Draw();
	}

	for (int i = 0; i < camera.size(); i++)
	{
		players_[i]->Draw();
	}

}

void SelectScene::DrawDebug(void)
{
	InputManager& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();


	for (int i = 0; i < 4; i++)
	{
		DrawFormatString(0, 120 + (20 * i), 0x00CC00, "input_[%d]: %d", i, input_[i].cntl_);
		DrawFormatString(500, 40 + (20 * i), 0x00CC00, "pos: %2.f,%2.f,%2.f", players_[i]->GetPos().x, players_[i]->GetPos().y, players_[i]->GetPos().z);
	}
	DrawFormatString(100, 120, 0x00CC00, "input_: %d", input_[0].config_);
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
	auto camera = SceneManager::GetInstance().GetCameras();

	// ����������ĂȂ��Ƃ�
	input_[0].config_ = KEY_CONFIG::NONE;
	
	for (int i = 1; i < SceneManager::PLAYER_NUM; i++) {
		input_[i].cntl_ = SceneManager::CNTL::PAD;
	}

	switch (Get1PDevice())
	{
	case SceneManager::CNTL::KEYBOARD:
		ChangeDevice(SceneManager::CNTL::KEYBOARD);

		//�L�[�̉�������
		if (ins.IsNew(KEY_INPUT_UP)		|| ins.IsNew(KEY_INPUT_W))	input_[0].config_ = KEY_CONFIG::UP;
		if (ins.IsNew(KEY_INPUT_DOWN)	||	ins.IsNew(KEY_INPUT_S))	input_[0].config_ = KEY_CONFIG::DOWN;
		if (ins.IsNew(KEY_INPUT_LEFT)	||	ins.IsNew(KEY_INPUT_A))	input_[0].config_ = KEY_CONFIG::LEFT;
		if (ins.IsNew(KEY_INPUT_RIGHT)	||	ins.IsNew(KEY_INPUT_D))	input_[0].config_ = KEY_CONFIG::RIGHT;

		//�L�[�̉�������(�������u�Ԃ���)
		if (ins.IsTrgDown(KEY_INPUT_UP)   ||ins.IsTrgDown(KEY_INPUT_W))input_[0].config_ = KEY_CONFIG::UP_TRG;
		if (ins.IsTrgDown(KEY_INPUT_DOWN) ||ins.IsTrgDown(KEY_INPUT_S))input_[0].config_ = KEY_CONFIG::DOWN_TRG;
		if (ins.IsTrgDown(KEY_INPUT_LEFT) ||ins.IsTrgDown(KEY_INPUT_A))input_[0].config_ = KEY_CONFIG::LEFT_TRG;
		if (ins.IsTrgDown(KEY_INPUT_RIGHT)||ins.IsTrgDown(KEY_INPUT_D))input_[0].config_ = KEY_CONFIG::RIGHT_TRG;

		if (ins.IsTrgDown(KEY_INPUT_SPACE)||ins.IsTrgDown(KEY_INPUT_RETURN))input_[0].config_ = KEY_CONFIG::DECIDE;
		break;

	case SceneManager::CNTL::PAD:
		ChangeDevice(SceneManager::CNTL::PAD);
		
		PadProcess();

		break;
	default:
		break;
	}
}

void SelectScene::PadProcess(void)
{
	auto& ins = InputManager::GetInstance();

	// ���X�e�B�b�N�̉���
	int leftStickX_[SceneManager::PLAYER_NUM];
	leftStickX_[0] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;
	leftStickX_[1] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD2).AKeyLX;
	leftStickX_[2] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD3).AKeyLX;
	leftStickX_[3] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD4).AKeyLX;

	//�c��
	int leftStickY_[SceneManager::PLAYER_NUM];
	leftStickX_[0] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;
	leftStickX_[1] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD2).AKeyLY;
	leftStickX_[2] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD3).AKeyLY;
	leftStickX_[3] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD4).AKeyLY;

	for (auto& leftStickY : leftStickY_)
	{
		if (leftStickY < -1)
		{
			key_ = KEY_CONFIG::UP;

		}
		if (leftStickY > 1)
		{
			key_ = KEY_CONFIG::DOWN;
		}
	}

	for (auto& leftStickX : leftStickX_)
	{
		if (leftStickX < -900)
		{
			key_ = KEY_CONFIG::LEFT;
		}
		if (leftStickX > 1)
		{
			key_ = KEY_CONFIG::RIGHT;
		}
	}

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		key_ = KEY_CONFIG::DECIDE;
	}
}

void SelectScene::ChangeDevice(SceneManager::CNTL cntl)
{
	input_[0].cntl_ = cntl;
}

void SelectScene::ControllDevice(void)
{
	InputManager& ins = InputManager::GetInstance();
	int key = CheckHitKeyAll(DX_CHECKINPUT_KEY);		//���͂𒲂ׂ�(�L�[�{�[�h)
	int padNum = GetJoypadNum();
	int padState = CheckHitKeyAll(DX_CHECKINPUT_PAD);	//���͂𒲂ׂ�(�p�b�h)

	//�L�[�{�[�h����̎��p�b�h������ł��Ȃ��悤��
	if (key != 0	&&
		padState == 0)
	{
		ChangeDevice(SceneManager::CNTL::KEYBOARD);
	}//�p�b�h����̎��L�[������ł��Ȃ��悤��
	else if (key <= 0 &&
		padNum > 0 &&
		padState != 0)
	{
		ChangeDevice(SceneManager::CNTL::PAD);
	}
}

SceneManager::CNTL SelectScene::GetDevice(void)
{
	for (auto& input : input_)
	{
		return input.cntl_;
	}
}

SelectScene::KEY_CONFIG SelectScene::GetConfig(void)
{
	return input_[0].config_;
}
