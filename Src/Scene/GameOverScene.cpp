#include<DxLib.h>
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Decoration/SoundManager.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene()
{
	imgBack_ = -1;
	imgGameOver_ = -1;
	imgMes_ = -1;

	alphaAdd_ = -1.0f;
	alpha_ = -1.0f;
	step_ = -1.0f;

	state_ = STATE::DOWN;

	stateChanges_.emplace(STATE::DOWN, std::bind(&GameOverScene::ChangeStateDown, this));
	stateChanges_.emplace(STATE::DISPLAY, std::bind(&GameOverScene::ChangeStateDisplay, this));
}

void GameOverScene::Init()
{
	auto& res = ResourceManager::GetInstance();
	auto& snd = SoundManager::GetInstance();

	// カメラモード：定点カメラ
	auto camera = SceneManager::GetInstance().GetCameras();
	camera[0]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
	camera[0]->ChangeMode(Camera::MODE::FIXED_POINT);

	//ゲームオーバー画像
	imgGameOver_ = res.Load(ResourceManager::SRC::GAMEOVER).handleId_;
	imgBack_ = res.Load(ResourceManager::SRC::GAMEOVER_BACK).handleId_;
	imgMes_ = res.Load(ResourceManager::SRC::CHANGE_TITLE_UI).handleId_;

	//音関係
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::GAMEOVER_SE, 
		res.Load(ResourceManager::SRC::GAMEOVER_SE).handleId_);
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAMEOVER_BGM, 
		res.Load(ResourceManager::SRC::GAMEOVER_BGM).handleId_);

	//最初にSEを再生させる
	snd.Play(SoundManager::SOUND::GAMEOVER_SE);

	//プレイヤー
	player_ = std::make_unique<OverPlayers>();
	player_->Init();

	//各種変数初期化
	alphaAdd_ = 1.0f;
	alpha_ = ALPHA_MAX;
	step_ = 0.0f;

	//初期状態
	ChangeState(STATE::DOWN);
}

void GameOverScene::Update()
{
	InputManager& ins = InputManager::GetInstance();

	//プレイヤー更新
	player_->Update();

	//状態別更新
	stateUpdate_();

	//シーン遷移
	if (ins.IsNew(KEY_INPUT_SPACE) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT)) {
		SoundManager & snd = SoundManager::GetInstance();

		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);

		//音声停止
		snd.Stop(SoundManager::SOUND::GAMEOVER_BGM);
		snd.Stop(SoundManager::SOUND::GAMEOVER_SE);
	}
}

void GameOverScene::Draw()
{
	//背景
	DrawExtendGraph(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		imgBack_,
		true
	);

	//ゲームオーバーメッセージ画像
	DrawRotaGraph(
		MES_DEFAULT_POS_X,
		MES_DEFAULT_POS_Y,
		1.0f,
		0.0f,
		imgGameOver_,
		true);

	//シーン遷移メッセージ描画
	if (state_ == STATE::DISPLAY) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
		DrawRotaGraph(
			MES2_DEFAULT_POS_X,
			MES2_DEFAULT_POS_Y,
			1.0f,
			0.0f,
			imgMes_,
			true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	
	//プレイヤー
	player_->Draw();
}

void GameOverScene::Release()
{
	//音声の停止
	SoundManager& snd = SoundManager::GetInstance();
	snd.Stop(SoundManager::SOUND::GAMEOVER_BGM);
	snd.Stop(SoundManager::SOUND::GAMEOVER_SE);
}

void GameOverScene::ChangeState(const STATE state)
{
	// 状態受け取り
	state_ = state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();
}

void GameOverScene::ChangeStateDown()
{
	stateUpdate_ = std::bind(&GameOverScene::UpdateDown, this);
}

void GameOverScene::ChangeStateDisplay()
{
	stateUpdate_ = std::bind(&GameOverScene::UpdateDisplay, this);
}

void GameOverScene::UpdateDown()
{
	//ステップ更新
	step_ += SceneManager::GetInstance().GetDeltaTime();

	if (step_ >= CHANGE_SECOND)
	{
		SoundManager::GetInstance().Stop(SoundManager::SOUND::GAMEOVER_SE);
		SoundManager::GetInstance().Play(SoundManager::SOUND::GAMEOVER_BGM);
		ChangeState(STATE::DISPLAY);
	}
}

void GameOverScene::UpdateDisplay()
{
	//アルファ値更新
	alpha_ += alphaAdd_;
	if (alpha_ > ALPHA_MAX) { alphaAdd_ = -1.0f; }
	else if (alpha_ < ALPHA_MIN) { alphaAdd_ = 1.0f; }
}
