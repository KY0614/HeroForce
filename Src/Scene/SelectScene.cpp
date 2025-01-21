#include <vector>
#include <math.h>
#include<algorithm>
#include "../Application.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/DataBank.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/StageObject.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/Character/PlayableChara/Other/SelectPlayer.h"
#include "../Object/SelectImage.h"
#include "../Object/Character/PlayableChara/PlayerBase.h"
#include "../Object/Character/PlayableChara/USER/PlAxeMan.h"
#include "SelectScene.h"

SelectScene::SelectScene(void)
{
	stage_ = nullptr;

	key_ = KEY_CONFIG::NONE;
	select_ = SELECT::NUMBER;

	// 状態管理
	stateChanges_.emplace(
		SELECT::NUMBER, std::bind(&SelectScene::ChangeStateNumber, this));
	stateChanges_.emplace(
		SELECT::OPERATION, std::bind(&SelectScene::ChangeStateOperation, this));
	stateChanges_.emplace(
		SELECT::ROLE, std::bind(&SelectScene::ChangeStateRole, this));
	stateChanges_.emplace(
		SELECT::MAX, std::bind(&SelectScene::ChangeStateMax, this));

	readyNum = 0;
	okNum = 0;
	isOk_[0] = false;
	isOk_[1] = false;
	isOk_[2] = false;
	isOk_[3] = false;

	input_[0].cntl_ = SceneManager::CNTL::NONE;
	input_[1].cntl_ = SceneManager::CNTL::NONE;
	input_[2].cntl_ = SceneManager::CNTL::NONE;
	input_[3].cntl_ = SceneManager::CNTL::NONE;

	input_[0].config_ = KEY_CONFIG::NONE;
	input_[1].config_ = KEY_CONFIG::NONE;
	input_[2].config_ = KEY_CONFIG::NONE;
	input_[3].config_ = KEY_CONFIG::NONE;
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();
	
	//背景用ステージ
	stage_ = new StageManager();
	stage_->Init();

	//背景色を白に
	SetBackgroundColor(255, 255, 255);
	//背景のステージモデルやらを半透明に
	//float alpha = 0.5f;
	//MV1SetOpacityRate(skyDome_->GetTransform().modelId, alpha);
	//for (int i = 0; i < StageManager::MODELS; i++) {
	//	for (auto& s : stage_->GetTtans(static_cast<StageManager::MODEL_TYPE>(i)))
	//	{
	//		MV1SetOpacityRate(s.modelId, alpha);
	//	}
	//}

	//フォグの設定
	SetFogEnable(false);
	//白
	SetFogColor(255, 255, 255);
	SetFogStartEnd(-300.0f, 15000.0f);

	//プレイヤー設定
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		players_[i] = std::make_shared<SelectPlayer>();
		players_[i]->Init();
	}

	//画像設定
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		images_[i] = std::make_unique<SelectImage>(*this, *players_);
		images_[i]->Init();
	}
	//image_ = std::make_unique<SelectImage>(*this);
	//image_->Init();

	// カメラモード：定点カメラ
	auto camera = SceneManager::GetInstance().GetCameras();
	camera[0]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
	camera[0]->ChangeMode(Camera::MODE::FIXED_POINT);

	//人数選択から
	ChangeSelect(SELECT::NUMBER);

	key_ = KEY_CONFIG::NONE;
	Change1PDevice(SceneManager::CNTL::NONE);

	Set1PDevice(SceneManager::CNTL::NONE);
}

void SelectScene::Update(void)
{
	//キーの設定
//	KeyConfigSetting();

	//どちらかを操作しているときにもう片方を操作できないように制御
//	ControllDevice();

	//空を回転
	skyDome_->Update();

	for (auto& p : players_)
	{
		p->Update();
	}

	//更新ステップ
	stateUpdate_();
}

void SelectScene::Draw(void)
{
	auto& ins = InputManager::GetInstance();

	skyDome_->Draw();
	stage_->Draw();

	//選択中の種類ごとの更新処理
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

	//デバッグ描画
	DrawDebug();
}

void SelectScene::Release(void)
{
	SceneManager::GetInstance().ResetCameras();

	//image_->Destroy();

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
	DataBank& data = DataBank::GetInstance();
	SceneManager::CNTL maincCntl = data.Output(1).cntrol_;
	Set1PDevice(maincCntl);

	stateUpdate_ = std::bind(&SelectScene::RoleUpdate, this);
}

void SelectScene::ChangeStateMax(void)
{
	stateUpdate_ = std::bind(&SelectScene::MaxUpdate, this);
}

void SelectScene::NumberUpdate(void)
{
	KeyConfigSetting();
	ControllDevice();

	images_[0]->Update();
}

void SelectScene::OperationUpdate(void)
{
	KeyConfigSetting();
	ControllDevice();

	images_[0]->Update();
}

void SelectScene::RoleUpdate(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();

	// 何も押されてないとき
	for (auto& i : input_)i.config_ = KEY_CONFIG::NONE;

	//1Pがキーボードだったらキーボード処理もするように(その場合1PのPADは操作できなくなる)
	if (input_[0].cntl_ == SceneManager::CNTL::KEYBOARD) KeyBoardProcess();
	PadProcess();

	VERTEX3D ver[4];
	VERTEX3D pointL[4];
	VERTEX3D pointR[4];
	for (int m = 1; m < 4; m++)
	{
		for (int i = 0; i < 4; i++)
		{
			ver[i] = images_[m - 1]->GetMeshVertex(i);
			pointL[i] = images_[m - 1]->GetPointLMeshVertex(i);
			pointR[i] = images_[m - 1]->GetPointRMeshVertex(i);

			VECTOR prevPos = ver[i].pos;
			VECTOR pointLPos = pointL[i].pos;
			VECTOR pointRPos = pointR[i].pos;

			VECTOR pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, prevPos,AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotMeshPos(pos, i);

			pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, pointLPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotPointLMeshPos(pos, i);

			pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, pointRPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotPointRMeshPos(pos, i);
		}
	}

	for (int i = 0; i < camera.size(); i++)
	{
		images_[i]->ChangeObject(input_[i], i );
		players_[i]->SetRole(images_[i]->GetRole());
	}
	//キャラクターの位置と向きを設定
	for (int i = 1; i < camera.size(); i++)
	{
		players_[i]->SetPos(AsoUtility::RotXZPos(DEFAULT_CAMERA_POS, players_[i - 1]->GetPos(), AsoUtility::Deg2RadF(90.0f)));
		players_[i]->SetRot(Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-90.0f * i), 0.0f));
	}

	for (int i = 0; i < camera.size(); i++)
	{
		if (readyNum > camera.size())
		{
			break;
		}
		if (images_[i]->GetReady()) 
		{
			readyNum++;
		}
	}

	if (readyNum >= camera.size() && input_[0].config_ == KEY_CONFIG::DECIDE)
	{
		readyNum++;
	}

	if (readyNum > camera.size())
	{
		//SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		for (int i = 0; i < camera.size(); i++)
		{
			if (isOk_[i])
			{
				continue;
			}
			if (input_[i].config_ == KEY_CONFIG::DECIDE)
			{
				isOk_[i] = true;
				okNum++;
			}
		}
	}

	if (okNum >= camera.size() && input_[0].config_ == KEY_CONFIG::DECIDE)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	//if (readyNum >= camera.size() && 
	//	input_[0].config_ == KEY_CONFIG::DECIDE)
	//{
	//	readyNum++;
	//}
}

void SelectScene::MaxUpdate(void)
{
	//何もしない
}

void SelectScene::NumberDraw(void)
{
	images_[0]->Draw();
}

void SelectScene::OperationDraw(void)
{
	//for (auto& i : images_)
	//{
	//	i->Draw();
	//}
	images_[0]->Draw();
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

void SelectScene::MaxDraw(void)
{
}

void SelectScene::DrawDebug(void)
{
	InputManager& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();

	for (int i = 0; i < 4; i++)
	{
		//DrawFormatString(0, 20 + (20 * i), 0Xff0000, "player : %0.2f,%0.2f,%0.2f",
		//	players_[0]->GetPosArray(i).x, players_[0]->GetPosArray(i).y, players_[0]->GetPosArray(i).z);

		//DrawFormatString(0, 120 + (20 * i), 0Xff0000, "player : %0.2f,%0.2f,%0.2f",
		//	players_[1]->GetPosArray(i).x, players_[1]->GetPosArray(i).y, players_[1]->GetPosArray(i).z);
		
		DrawFormatString(0, 20 + (20 * i), 0Xff0000, "ver.pos : %0.2f,%0.2f,%0.2f",
			images_[i]->GetVerPos().x, images_[i]->GetVerPos().y, images_[i]->GetVerPos().z);

		//DrawFormatString(0, 120 + (20 * i), 0x00CC00, "input_[%d]: %d", i, input_[i].cntl_);
		//DrawFormatString(500, 40 + (20 * i), 0x00CC00, "pos: %2.f,%2.f,%2.f", images_[i]->GetMeshVertex(i).pos.x, images_[i]->GetMeshVertex(i).pos.y, images_[i]->GetMeshVertex(i).pos.z);
		DrawFormatString(500, 40 + (20 * i), 0x00CC00, "isOk: %d", isOk_[i]);
		//DrawFormatString(Application::SCREEN_SIZE_X - 100, 100 + (i*20), 0xFF3333, "ready : %d", images_[i]->GetReady());
		if (isOk_[i]) {
			DrawCircle(Application::SCREEN_SIZE_X / 2 + (70 * i), Application::SCREEN_SIZE_Y / 2, 50, 0x00FF00, true);
		}
	}
	DrawFormatString(100, 120, 0x00CC00, "input_: %d", input_[0].config_);

}

void SelectScene::ChangeSelect(const SELECT _state)
{
	//状態遷移
	select_ = _state;

	// 各状態遷移の初期処理
	stateChanges_[select_]();
}

void SelectScene::KeyConfigSetting(void)
{
	auto& ins = InputManager::GetInstance();

	for (auto& i : input_)
	{
		// 何も押されてないとき
		i.config_ = KEY_CONFIG::NONE;
	}
	
	//2P以降は全員PAD入力
	for (int i = 1; i < SceneManager::PLAYER_NUM; i++) {
		input_[i].cntl_ = SceneManager::CNTL::PAD;
	}

	switch (Get1PDevice())
	{
	case SceneManager::CNTL::KEYBOARD:

		KeyBoardProcess();
		break;

	case SceneManager::CNTL::PAD:

		PadProcess();

		break;
	default:
		break;
	}
}

void SelectScene::KeyBoardProcess(void)
{
	auto& ins = InputManager::GetInstance();

	//キーの押下判定
	if (ins.IsNew(KEY_INPUT_UP)	  || ins.IsNew(KEY_INPUT_W))	input_[0].config_ = KEY_CONFIG::UP;
	if (ins.IsNew(KEY_INPUT_DOWN) || ins.IsNew(KEY_INPUT_S))	input_[0].config_ = KEY_CONFIG::DOWN;
	if (ins.IsNew(KEY_INPUT_LEFT) || ins.IsNew(KEY_INPUT_A))	input_[0].config_ = KEY_CONFIG::LEFT;
	if (ins.IsNew(KEY_INPUT_RIGHT) || ins.IsNew(KEY_INPUT_D))	input_[0].config_ = KEY_CONFIG::RIGHT;

	//キーの押下判定(押した瞬間だけ)
	if (ins.IsTrgDown(KEY_INPUT_UP)  || ins.IsTrgDown(KEY_INPUT_W))input_[0].config_ = KEY_CONFIG::UP_TRG;
	if (ins.IsTrgDown(KEY_INPUT_DOWN) || ins.IsTrgDown(KEY_INPUT_S))input_[0].config_ = KEY_CONFIG::DOWN_TRG;
	if (ins.IsTrgDown(KEY_INPUT_LEFT) || ins.IsTrgDown(KEY_INPUT_A))input_[0].config_ = KEY_CONFIG::LEFT_TRG;
	if (ins.IsTrgDown(KEY_INPUT_RIGHT) || ins.IsTrgDown(KEY_INPUT_D))input_[0].config_ = KEY_CONFIG::RIGHT_TRG;

	if (ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsTrgDown(KEY_INPUT_RETURN))input_[0].config_ = KEY_CONFIG::DECIDE;
}

void SelectScene::PadProcess(void)
{
	auto& ins = InputManager::GetInstance();

	// 左スティックの横軸
	int leftStickX_[SceneManager::PLAYER_NUM];
	//縦軸
	int leftStickY_[SceneManager::PLAYER_NUM];

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
		if (leftStickY_[i] < -1)
		{
			input_[i].config_ = KEY_CONFIG::UP;

		}
		if (leftStickY_[i] > 1)
		{
			input_[i].config_ = KEY_CONFIG::DOWN;
		}

		if (leftStickX_[i] < -1)
		{
			input_[i].config_ = KEY_CONFIG::LEFT;
		}
		if (leftStickX_[i] > 1)
		{
			input_[i].config_ = KEY_CONFIG::RIGHT;
		}
		if (ins.IsPadBtnTrgDown(static_cast<InputManager::JOYPAD_NO>(i + 1), InputManager::JOYPAD_BTN::RIGHT))
		{
			input_[i].config_ = KEY_CONFIG::DECIDE;
		}
	}
}

void SelectScene::Change1PDevice(SceneManager::CNTL cntl)
{
	input_[0].cntl_ = cntl;
}

void SelectScene::ControllDevice(void)
{
	InputManager& ins = InputManager::GetInstance();
	int key = CheckHitKeyAll(DX_CHECKINPUT_KEY);		//入力を調べる(キーボード)
	int padNum = GetJoypadNum();
	int padState = CheckHitKeyAll(DX_CHECKINPUT_PAD);	//入力を調べる(パッド)

	DataBank& data = DataBank::GetInstance();
	SceneManager::CNTL maincCntl = data.Output(1).cntrol_;

	//キーボード操作の時パッド操作をできないように
	if (key != 0	&&
		padState == 0)
	{
		Change1PDevice(SceneManager::CNTL::KEYBOARD);
	}//パッド操作の時キー操作をできないように
	else if (key <= 0 &&
		padNum > 0 &&
		padState != 0)
	{
		Change1PDevice(SceneManager::CNTL::PAD);
	}
}

SelectScene::KEY_CONFIG SelectScene::GetConfig(void)
{
	return input_[0].config_;
}
