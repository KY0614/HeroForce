#include <vector>
#include <math.h>
#include<algorithm>
#include "../Application.h"
#include "../Manager/Decoration/SoundManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/DataBank.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/Character/PlayableChara/Other/SelectCharacter.h"
#include "../Object/SelectImage.h"
#include "../Object/Character/PlayableChara/Other/WaitEnemy.h"
#include "SelectScene.h"

SelectScene::SelectScene(void)
{
	stage_ = nullptr;
	skyDome_ = nullptr;

	key_ = KEY_CONFIG::NONE;
	select_ = SELECT::NUMBER;

	// ��ԊǗ�
	stateChanges_.emplace(
		SELECT::DISPLAY, std::bind(&SelectScene::ChangeStateDisplay, this));
	stateChanges_.emplace(
		SELECT::NUMBER, std::bind(&SelectScene::ChangeStateNumber, this));
	stateChanges_.emplace(
		SELECT::OPERATION, std::bind(&SelectScene::ChangeStateOperation, this));
	stateChanges_.emplace(
		SELECT::ROLE, std::bind(&SelectScene::ChangeStateRole, this));
	stateChanges_.emplace(
		SELECT::MAX, std::bind(&SelectScene::ChangeStateMax, this));

	for (int i = 0; i < PLAYER_NUM; i++)
	{
		isOk_[i] = false;

		input_[i].cntl_ = SceneManager::CNTL::NONE;

		input_[i].config_ = KEY_CONFIG::NONE;

		characters_[i] = nullptr;
		images_[i] = nullptr;
		enemys_[i] = nullptr;
	}

	imgDisplay_ = -1;
	imgPlayer_ = -1;
	imgOperation_ = -1;
	imgRole_ = -1;
	imgWait_ = -1;
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	auto& snd = SoundManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	//���y�ǂݍ���
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::SELECT,
		res.Load(ResourceManager::SRC::SELECT_BGM).handleId_);

	//���ʒ���
	snd.AdjustVolume(SoundManager::SOUND::SELECT, BGM_VOLUME);
	//���y�Đ�
	snd.Play(SoundManager::SOUND::SELECT);

	//�摜�̓ǂݍ���
	imgPlayer_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMG).handleId_;
 	imgDisplay_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DISPLAY_IMG).handleId_;
	imgOperation_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::OPERATION_IMG).handleId_;
	imgRole_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ROLE_IMG).handleId_;
	imgWait_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::WAIT_IMG).handleId_;

	//�X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();
	
	//�w�i�p�X�e�[�W
	stage_ = std::make_unique<StageManager>();
	stage_->Init();

	//�v���C���[�ƓG�Ɖ摜�̏�����
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		characters_[i] = std::make_unique<SelectCharacter>();
		characters_[i]->Init();
		
		enemys_[i] = std::make_unique<WaitEnemy>();
		enemys_[i]->Init();

		images_[i] = std::make_unique<SelectImage>(*this);
		images_[i]->Init();
	}

	// �J�������[�h�F��_�J����
	auto camera = SceneManager::GetInstance().GetCameras();
	camera[0]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
	camera[0]->ChangeMode(Camera::MODE::FIXED_POINT);

	//�g�p�f�B�X�v���C���I������
	ChangeSelect(SELECT::DISPLAY);

	//�L�[�R���t�B�O�ƃf�o�C�X��������
	key_ = KEY_CONFIG::NONE;
	Change1PDevice(SceneManager::CNTL::NONE);
}

void SelectScene::Update(void)
{
	//�����]
	skyDome_->Update();

	//�X�V�X�e�b�v
	stateUpdate_();
}

void SelectScene::Draw(void)
{
	auto& ins = InputManager::GetInstance();

	skyDome_->Draw();
	stage_->Draw();
	SetUseLightAngleAttenuation(FALSE);

	//�X�V�X�e�b�v
	stateDraw_();

	SetUseLightAngleAttenuation(TRUE);
}

void SelectScene::Release(void)
{
}

void SelectScene::ChangeStateDisplay(void)
{
	stateUpdate_ = std::bind(&SelectScene::DisplayUpdate, this);
	stateDraw_ = std::bind(&SelectScene::DisplayDraw, this);
}

void SelectScene::ChangeStateNumber(void)
{
	stateUpdate_ = std::bind(&SelectScene::NumberUpdate, this);
	stateDraw_ = std::bind(&SelectScene::NumberDraw, this);
}

void SelectScene::ChangeStateOperation(void)
{
	stateUpdate_ = std::bind(&SelectScene::OperationUpdate, this);
	stateDraw_ = std::bind(&SelectScene::OperationDraw, this);
}

void SelectScene::ChangeStateRole(void)
{
	//�f�o�C�X�I���őI�������f�o�C�X���Œ肳����
	DataBank& data = DataBank::GetInstance();
	SceneManager::CNTL maincCntl = data.Output(1).cntrol_;
	Change1PDevice(maincCntl);

	stateUpdate_ = std::bind(&SelectScene::RoleUpdate, this);
	stateDraw_ = std::bind(&SelectScene::RoleDraw, this);
}

void SelectScene::ChangeStateMax(void)
{
	stateUpdate_ = std::bind(&SelectScene::MaxUpdate, this);
	stateDraw_ = std::bind(&SelectScene::MaxDraw, this);
}

void SelectScene::DisplayUpdate(void)
{
	//1P�̑��쏈��
	Process1PInput();
	//�L�[�{�[�h�ƃR���g���[���[�𓯎��ɑ���ł��Ȃ��悤�ɐ���
	Control1PDevice();
	//����ł���̂͂PP�����ɂ������̂Ŕz��̐擪����
	images_[0]->Update();
}

void SelectScene::NumberUpdate(void)
{
	//1P�̑��쏈��
	Process1PInput();
	//�L�[�{�[�h�ƃR���g���[���[�𓯎��ɑ���ł��Ȃ��悤�ɐ���
	Control1PDevice();
	//����ł���̂͂PP�����ɂ������̂Ŕz��̐擪����
	images_[0]->Update();
}

void SelectScene::OperationUpdate(void)
{
	//1P�̑��쏈��
	Process1PInput();
	//�L�[�{�[�h�ƃR���g���[���[�𓯎��ɑ���ł��Ȃ��悤�ɐ���
	Control1PDevice();
	//����ł���̂͂PP�����ɂ������̂Ŕz��̐擪����
	images_[0]->Update();

	auto camera = SceneManager::GetInstance().GetCameras();
	//1P�ȊO�̉�ʂɂ͓G���A�j���[�V���������đҋ@���̉�ʂ����
	for (int pNum = 1; pNum < camera.size(); pNum++)
	{
		for (int index = 0; index < PLAYER_NUM; index++) {
			//�J�����̉�]�ɍ��킹�ďo���ꏊ��90�x����]������
			VECTOR pos = AsoUtility::RotXZPos(DEFAULT_CAMERA_POS, enemys_[pNum - 1]->GetPosAtIndex(index), AsoUtility::Deg2RadF(90.0f));
			enemys_[pNum]->SetPosAtIndex(pos, index);
			enemys_[pNum]->SetRot(Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-90.0f * pNum), 0.0f));
		}
		enemys_[pNum]->Update();
	}
}

void SelectScene::RoleUpdate(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();
	bool checkAllReady = false;
	if (IsAllReady()) checkAllReady = true;

	// ����������ĂȂ��Ƃ���NONE
	for (auto& i : input_)i.config_ = KEY_CONFIG::NONE;

	//1P���L�[�{�[�h��������L�[�{�[�h����������悤��(���̏ꍇ1P��PAD�͑���ł��Ȃ��Ȃ�)
	if (Get1PDevice() == SceneManager::CNTL::KEYBOARD) KeyBoardProcess();
	PadProcess();

	for (auto& c : characters_)
	{
		c->Update();
	}

	//�I�u�W�F�N�g��90�x����]������
	//(�J������90�x����]���Ă�̂ł���ɍ��킹�邽��)
	VERTEX3D ver[4];
	VERTEX3D ready[4];
	VERTEX3D pointL[4];
	VERTEX3D pointR[4];
	for (int m = 1; m < 4; m++)
	{
		for (int i = 0; i < 4; i++)
		{
			//��]������O�̍��W������Ă���
			ver[i] = images_[m - 1]->GetMeshVertexAtIndex(i);
			ready[i] = images_[m - 1]->GetReadyMeshVertexAtIndex(i);
			pointL[i] = images_[m - 1]->GetPointLMeshVertexAtIndex(i);
			pointR[i] = images_[m - 1]->GetPointRMeshVertexAtIndex(i);

			VECTOR prevPos = ver[i].pos;
			VECTOR prevReadyPos = ready[i].pos;
			VECTOR prevPointLPos = pointL[i].pos;
			VECTOR prevPointRPos = pointR[i].pos;

			//��]�O�̍��W����X�O�x��]������
			//�l���I�����b�V��
			VECTOR pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, prevPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotMeshPos(pos, i);

			//Ready���b�V��
			pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, prevReadyPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotReadyMeshPos(pos, i);

			//����󃁃b�V��
			pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, prevPointLPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotPointLMeshPos(pos, i);

			//�E��󃁃b�V��
			pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, prevPointRPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotPointRMeshPos(pos, i);
		}
	}

	for (int i = 0; i < camera.size(); i++)
	{
		//�v���C���[���Ƃ̑���ŃI�u�W�F�N�g��ύX����
		images_[i]->ChangeObject(input_[i], i);
		//�v���C���[�̏�����Ԃ������
		isOk_[i] = images_[i]->GetReady();
		//�v���C���[���Ƃ̑���ŕύX������E�ɉ����ďo���L�����N�^�[��ύX
		characters_[i]->SetRole(images_[i]->GetRole());
	}

	//�L�����N�^�[�̈ʒu�ƌ������J�����̉�]�����Ƃɐݒ�
	for (int i = 1; i < camera.size(); i++)
	{
		//�ʒu
		characters_[i]->SetPos(AsoUtility::RotXZPos(DEFAULT_CAMERA_POS, characters_[i - 1]->GetFrontPos(), AsoUtility::Deg2RadF(90.0f)));
		characters_[i]->SetRot(Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-90.0f * i), 0.0f));
		//����
		characters_[i]->SetChickenPos(AsoUtility::RotXZPos(DEFAULT_CAMERA_POS, characters_[i - 1]->GetChickenPos(), AsoUtility::Deg2RadF(90.0f)));
		characters_[i]->SetRotChicken(Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-90.0f * i), 0.0f));
	}

	//�S������������Ԃ�1P������{�^�������Ő�����ʂ�
	if (checkAllReady && input_[0].config_ == KEY_CONFIG::DECIDE)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::EXP);
	}
}

void SelectScene::MaxUpdate(void)
{
	//�������Ȃ�
}

void SelectScene::DisplayDraw(void)
{
	//��ʕ������Ă��Ȃ��̂Ő擪�����`��
	images_[0]->Draw();
	//�f�B�X�v���C���I��p�摜�`��
	DrawRotaGraph(Application::SCREEN_SIZE_X/2, IMAGE_POS_Y,1.0f, 0.0f, imgDisplay_, true);
}

void SelectScene::NumberDraw(void)
{
	//��ʕ������Ă��Ȃ��̂Ő擪�����`��
	images_[0]->Draw();
	//�l���I��p�摜�`��
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, IMAGE_POS_Y, 1.0f, 0.0f, imgPlayer_, true);
}

void SelectScene::OperationDraw(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();

	//�I������̂�1P�����Ȃ̂Ő擪�����`��
	images_[0]->Draw();

	for (int i = 1; i < camera.size(); i++)
	{
		enemys_[i]->Draw();
	}

	//�E�B���h�E����������Ă�����1P�ɂ̓f�o�C�X�I�����A���ɂ͑ҋ@���̉摜��`��
	if (SceneManager::GetInstance().GetNowWindow() < 1)
	{ 
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, IMAGE_POS_Y, 1.0f, 0.0f, imgOperation_, true);
	}
	else{
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, WAITIMAGE_POS_Y, 1.0f, 0.0f, imgWait_, true);
	}
}

void SelectScene::RoleDraw(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();

	for (int i = 0; i < camera.size(); i++)
	{
		images_[i]->Draw();
	}

	SetUseLightAngleAttenuation(TRUE);
	for (int i = 0; i < camera.size(); i++)
	{
		characters_[i]->Draw();
	}

	int imgScale = 180;
	if (SceneManager::GetInstance().GetNowWindow() > -1)
	{
		DrawRotaGraph(Application::SCREEN_SIZE_X - imgScale, 100, 1.0f, 0.0f, imgRole_, true);
	}
}

void SelectScene::MaxDraw(void)
{//�������Ȃ�
}

void SelectScene::ChangeSelect(const SELECT _state)
{
	//��ԑJ��
	select_ = _state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[select_]();
}

void SelectScene::Process1PInput(void)
{
	auto& ins = InputManager::GetInstance();

	for (auto& i : input_)
	{
		//����������ĂȂ��Ƃ�
		i.config_ = KEY_CONFIG::NONE;
	}
	
	//2P�ȍ~�͑S��PAD����
	for (int i = 1; i < PLAYER_NUM; i++) {
		input_[i].cntl_ = SceneManager::CNTL::PAD;
	}

	//1P�̓��̓f�o�C�X�ɂ���đ��쏈����ύX����
	switch (Get1PDevice())
	{
	case SceneManager::CNTL::KEYBOARD:
		KeyBoardProcess();		//�L�[����
		break;

	case SceneManager::CNTL::PAD:
		PadProcess();		//�p�b�h����
		break;
	default:
		break;
	}
}

void SelectScene::KeyBoardProcess(void)
{
	auto& ins = InputManager::GetInstance();

	//�L�[�̉�������
	if (ins.IsNew(KEY_INPUT_UP)	  || ins.IsNew(KEY_INPUT_W))	input_[0].config_ = KEY_CONFIG::UP;
	if (ins.IsNew(KEY_INPUT_DOWN) || ins.IsNew(KEY_INPUT_S))	input_[0].config_ = KEY_CONFIG::DOWN;
	if (ins.IsNew(KEY_INPUT_LEFT) || ins.IsNew(KEY_INPUT_A))	input_[0].config_ = KEY_CONFIG::LEFT;
	if (ins.IsNew(KEY_INPUT_RIGHT) || ins.IsNew(KEY_INPUT_D))	input_[0].config_ = KEY_CONFIG::RIGHT;

	if (ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsTrgDown(KEY_INPUT_RETURN))input_[0].config_ = KEY_CONFIG::DECIDE;
	if (ins.IsTrgDown(KEY_INPUT_C))input_[0].config_ = KEY_CONFIG::CANCEL;
}

void SelectScene::PadProcess(void)
{
	auto& ins = InputManager::GetInstance();

	// ���X�e�B�b�N�̉���
	int leftStickX_[PLAYER_NUM];
	//�c��
	int leftStickY_[PLAYER_NUM];

	leftStickX_[0] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;
	leftStickX_[1] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD2).AKeyLX;
	leftStickX_[2] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD3).AKeyLX;
	leftStickX_[3] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD4).AKeyLX;

	leftStickY_[0] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;
	leftStickY_[1] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD2).AKeyLY;
	leftStickY_[2] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD3).AKeyLY;
	leftStickY_[3] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD4).AKeyLY;

	int mainCntl = 0;

	if (Get1PDevice() == SceneManager::CNTL::KEYBOARD)
	{
		mainCntl = 1;
	}
	for (int i= mainCntl; i < 4; i++)
	{
		//�X�e�B�b�N�����
		if (leftStickY_[i] < -1)
		{
			input_[i].config_ = KEY_CONFIG::UP;

		}
		//�X�e�B�b�N������
		if (leftStickY_[i] > 1)
		{
			input_[i].config_ = KEY_CONFIG::DOWN;
		}
		//�X�e�B�b�N������
		if (leftStickX_[i] < -1)
		{
			input_[i].config_ = KEY_CONFIG::LEFT;
		}
		//�X�e�B�b�N�E����
		if (leftStickX_[i] > 1)
		{
			input_[i].config_ = KEY_CONFIG::RIGHT;
		}

		//����{�^���i�p�b�hNo��1����)
		if (ins.IsPadBtnTrgDown(static_cast<InputManager::JOYPAD_NO>(i + 1), InputManager::JOYPAD_BTN::RIGHT))
		{
			input_[i].config_ = KEY_CONFIG::DECIDE;
		}
		//�L�����Z���{�^���i�p�b�hNo��1����)
		if (ins.IsPadBtnTrgDown(static_cast<InputManager::JOYPAD_NO>(i + 1), InputManager::JOYPAD_BTN::DOWN))
		{
			input_[i].config_ = KEY_CONFIG::CANCEL;
		}
	}
}

void SelectScene::Control1PDevice(void)
{
	InputManager& ins = InputManager::GetInstance();
	int key = CheckHitKeyAll(DX_CHECKINPUT_KEY);		//���͂𒲂ׂ�(�L�[�{�[�h)
	int padNum = GetJoypadNum();						//�p�b�h�̐����擾����
	int padState = CheckHitKeyAll(DX_CHECKINPUT_PAD);	//���͂𒲂ׂ�(�p�b�h)

	//�L�[�{�[�h����̎��p�b�h������ł��Ȃ��悤��
	if (key != 0	&&
		padState == 0)
	{
		Change1PDevice(SceneManager::CNTL::KEYBOARD);
	}//�p�b�h����̎��L�[������ł��Ȃ��悤��
	else if (key <= 0 &&
		padNum > 0 &&
		padState != 0)
	{
		Change1PDevice(SceneManager::CNTL::PAD);
	}
}

bool SelectScene::IsAllReady(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();
	for (int i = 0; i < camera.size(); i++)
	{
		//��l�ł��������ł��Ă��Ȃ�������false
		if (!isOk_[i])
		{
			return false;
		}
	}
	return true;
}