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

	// 状態管理
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

	//音楽読み込み
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::SELECT,
		res.Load(ResourceManager::SRC::SELECT_BGM).handleId_);

	//音量調節
	snd.AdjustVolume(SoundManager::SOUND::SELECT, BGM_VOLUME);
	//音楽再生
	snd.Play(SoundManager::SOUND::SELECT);

	//画像の読み込み
	imgPlayer_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMG).handleId_;
 	imgDisplay_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DISPLAY_IMG).handleId_;
	imgOperation_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::OPERATION_IMG).handleId_;
	imgRole_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ROLE_IMG).handleId_;
	imgWait_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::WAIT_IMG).handleId_;

	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();
	
	//背景用ステージ
	stage_ = std::make_unique<StageManager>();
	stage_->Init();

	//プレイヤーと敵と画像の初期化
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		characters_[i] = std::make_unique<SelectCharacter>();
		characters_[i]->Init();
		
		enemys_[i] = std::make_unique<WaitEnemy>();
		enemys_[i]->Init();

		images_[i] = std::make_unique<SelectImage>(*this);
		images_[i]->Init();
	}

	// カメラモード：定点カメラ
	auto camera = SceneManager::GetInstance().GetCameras();
	camera[0]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
	camera[0]->ChangeMode(Camera::MODE::FIXED_POINT);

	//使用ディスプレイ数選択から
	ChangeSelect(SELECT::DISPLAY);

	//キーコンフィグとデバイスを初期化
	key_ = KEY_CONFIG::NONE;
	Change1PDevice(SceneManager::CNTL::NONE);
}

void SelectScene::Update(void)
{
	//空を回転
	skyDome_->Update();

	//更新ステップ
	stateUpdate_();
}

void SelectScene::Draw(void)
{
	auto& ins = InputManager::GetInstance();

	skyDome_->Draw();
	stage_->Draw();
	SetUseLightAngleAttenuation(FALSE);

	//更新ステップ
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
	//デバイス選択で選択したデバイスを固定させる
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
	//1Pの操作処理
	Process1PInput();
	//キーボードとコントローラーを同時に操作できないように制限
	Control1PDevice();
	//操作できるのは１Pだけにしたいので配列の先頭だけ
	images_[0]->Update();
}

void SelectScene::NumberUpdate(void)
{
	//1Pの操作処理
	Process1PInput();
	//キーボードとコントローラーを同時に操作できないように制限
	Control1PDevice();
	//操作できるのは１Pだけにしたいので配列の先頭だけ
	images_[0]->Update();
}

void SelectScene::OperationUpdate(void)
{
	//1Pの操作処理
	Process1PInput();
	//キーボードとコントローラーを同時に操作できないように制限
	Control1PDevice();
	//操作できるのは１Pだけにしたいので配列の先頭だけ
	images_[0]->Update();

	auto camera = SceneManager::GetInstance().GetCameras();
	//1P以外の画面には敵をアニメーションさせて待機中の画面を作る
	for (int pNum = 1; pNum < camera.size(); pNum++)
	{
		for (int index = 0; index < PLAYER_NUM; index++) {
			//カメラの回転に合わせて出現場所を90度ずつ回転させる
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

	// 何も押されてないときはNONE
	for (auto& i : input_)i.config_ = KEY_CONFIG::NONE;

	//1Pがキーボードだったらキーボード処理もするように(その場合1PのPADは操作できなくなる)
	if (Get1PDevice() == SceneManager::CNTL::KEYBOARD) KeyBoardProcess();
	PadProcess();

	for (auto& c : characters_)
	{
		c->Update();
	}

	//オブジェクトを90度ずつ回転させる
	//(カメラが90度ずつ回転してるのでそれに合わせるため)
	VERTEX3D ver[4];
	VERTEX3D ready[4];
	VERTEX3D pointL[4];
	VERTEX3D pointR[4];
	for (int m = 1; m < 4; m++)
	{
		for (int i = 0; i < 4; i++)
		{
			//回転させる前の座標を取っておく
			ver[i] = images_[m - 1]->GetMeshVertexAtIndex(i);
			ready[i] = images_[m - 1]->GetReadyMeshVertexAtIndex(i);
			pointL[i] = images_[m - 1]->GetPointLMeshVertexAtIndex(i);
			pointR[i] = images_[m - 1]->GetPointRMeshVertexAtIndex(i);

			VECTOR prevPos = ver[i].pos;
			VECTOR prevReadyPos = ready[i].pos;
			VECTOR prevPointLPos = pointL[i].pos;
			VECTOR prevPointRPos = pointR[i].pos;

			//回転前の座標から９０度回転させる
			//人数選択メッシュ
			VECTOR pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, prevPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotMeshPos(pos, i);

			//Readyメッシュ
			pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, prevReadyPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotReadyMeshPos(pos, i);

			//左矢印メッシュ
			pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, prevPointLPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotPointLMeshPos(pos, i);

			//右矢印メッシュ
			pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, prevPointRPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotPointRMeshPos(pos, i);
		}
	}

	for (int i = 0; i < camera.size(); i++)
	{
		//プレイヤーごとの操作でオブジェクトを変更する
		images_[i]->ChangeObject(input_[i], i);
		//プレイヤーの準備状態をいれる
		isOk_[i] = images_[i]->GetReady();
		//プレイヤーごとの操作で変更した役職に応じて出すキャラクターを変更
		characters_[i]->SetRole(images_[i]->GetRole());
	}

	//キャラクターの位置と向きをカメラの回転をもとに設定
	for (int i = 1; i < camera.size(); i++)
	{
		//位置
		characters_[i]->SetPos(AsoUtility::RotXZPos(DEFAULT_CAMERA_POS, characters_[i - 1]->GetFrontPos(), AsoUtility::Deg2RadF(90.0f)));
		characters_[i]->SetRot(Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-90.0f * i), 0.0f));
		//向き
		characters_[i]->SetChickenPos(AsoUtility::RotXZPos(DEFAULT_CAMERA_POS, characters_[i - 1]->GetChickenPos(), AsoUtility::Deg2RadF(90.0f)));
		characters_[i]->SetRotChicken(Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-90.0f * i), 0.0f));
	}

	//全員準備完了状態で1Pが決定ボタン押下で説明画面へ
	if (checkAllReady && input_[0].config_ == KEY_CONFIG::DECIDE)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::EXP);
	}
}

void SelectScene::MaxUpdate(void)
{
	//何もしない
}

void SelectScene::DisplayDraw(void)
{
	//画面複製していないので先頭だけ描画
	images_[0]->Draw();
	//ディスプレイ数選択用画像描画
	DrawRotaGraph(Application::SCREEN_SIZE_X/2, IMAGE_POS_Y,1.0f, 0.0f, imgDisplay_, true);
}

void SelectScene::NumberDraw(void)
{
	//画面複製していないので先頭だけ描画
	images_[0]->Draw();
	//人数選択用画像描画
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, IMAGE_POS_Y, 1.0f, 0.0f, imgPlayer_, true);
}

void SelectScene::OperationDraw(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();

	//選択するのは1Pだけなので先頭だけ描画
	images_[0]->Draw();

	for (int i = 1; i < camera.size(); i++)
	{
		enemys_[i]->Draw();
	}

	//ウィンドウが複製されていたら1Pにはデバイス選択を、他には待機中の画像を描画
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
{//何もしない
}

void SelectScene::ChangeSelect(const SELECT _state)
{
	//状態遷移
	select_ = _state;

	// 各状態遷移の初期処理
	stateChanges_[select_]();
}

void SelectScene::Process1PInput(void)
{
	auto& ins = InputManager::GetInstance();

	for (auto& i : input_)
	{
		//何も押されてないとき
		i.config_ = KEY_CONFIG::NONE;
	}
	
	//2P以降は全員PAD入力
	for (int i = 1; i < PLAYER_NUM; i++) {
		input_[i].cntl_ = SceneManager::CNTL::PAD;
	}

	//1Pの入力デバイスによって操作処理を変更する
	switch (Get1PDevice())
	{
	case SceneManager::CNTL::KEYBOARD:
		KeyBoardProcess();		//キー操作
		break;

	case SceneManager::CNTL::PAD:
		PadProcess();		//パッド操作
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

	if (ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsTrgDown(KEY_INPUT_RETURN))input_[0].config_ = KEY_CONFIG::DECIDE;
	if (ins.IsTrgDown(KEY_INPUT_C))input_[0].config_ = KEY_CONFIG::CANCEL;
}

void SelectScene::PadProcess(void)
{
	auto& ins = InputManager::GetInstance();

	// 左スティックの横軸
	int leftStickX_[PLAYER_NUM];
	//縦軸
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
		//スティック上入力
		if (leftStickY_[i] < -1)
		{
			input_[i].config_ = KEY_CONFIG::UP;

		}
		//スティック下入力
		if (leftStickY_[i] > 1)
		{
			input_[i].config_ = KEY_CONFIG::DOWN;
		}
		//スティック左入力
		if (leftStickX_[i] < -1)
		{
			input_[i].config_ = KEY_CONFIG::LEFT;
		}
		//スティック右入力
		if (leftStickX_[i] > 1)
		{
			input_[i].config_ = KEY_CONFIG::RIGHT;
		}

		//決定ボタン（パッドNoが1から)
		if (ins.IsPadBtnTrgDown(static_cast<InputManager::JOYPAD_NO>(i + 1), InputManager::JOYPAD_BTN::RIGHT))
		{
			input_[i].config_ = KEY_CONFIG::DECIDE;
		}
		//キャンセルボタン（パッドNoが1から)
		if (ins.IsPadBtnTrgDown(static_cast<InputManager::JOYPAD_NO>(i + 1), InputManager::JOYPAD_BTN::DOWN))
		{
			input_[i].config_ = KEY_CONFIG::CANCEL;
		}
	}
}

void SelectScene::Control1PDevice(void)
{
	InputManager& ins = InputManager::GetInstance();
	int key = CheckHitKeyAll(DX_CHECKINPUT_KEY);		//入力を調べる(キーボード)
	int padNum = GetJoypadNum();						//パッドの数を取得する
	int padState = CheckHitKeyAll(DX_CHECKINPUT_PAD);	//入力を調べる(パッド)

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

bool SelectScene::IsAllReady(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();
	for (int i = 0; i < camera.size(); i++)
	{
		//一人でも準備ができていなかったらfalse
		if (!isOk_[i])
		{
			return false;
		}
	}
	return true;
}