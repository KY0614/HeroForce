#include "../Object/Stage/SkyDome.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Character/Chiken/ClearChicken.h"
#include "../Object/Character/PlayableChara/Other/ClearPlayers.h"
#include "GameClearScene.h"

GameClearScene::GameClearScene()
{
	sky_ = nullptr;
	stage_ = nullptr;
	for (auto& c : chickens_) { c = nullptr; }
	player_ = nullptr;
	imgMes_ = -1;

	state_ = STATE::HAPPY;

	stateChanges_.emplace(STATE::HAPPY, std::bind(&GameClearScene::ChangeStateHappy, this));
	stateChanges_.emplace(STATE::DISPLAY, std::bind(&GameClearScene::ChangeStateDisplay, this));
}

void GameClearScene::Init(void)
{
	auto& res = ResourceManager::GetInstance();
	auto& snd = SoundManager::GetInstance();

	//スカイドーム
	sky_ = std::make_unique<SkyDome>();
	sky_->Init();

	//ステージ
	stage_ = std::make_unique<StageManager>();
	stage_->Init();

	//チキン
	VECTOR pos[CHICKEN_CREATES] = { CHICKEN_POS_1,CHICKEN_POS_2,CHICKEN_POS_3,CHICKEN_POS_4 };
	for (int i = 0; i < CHICKEN_CREATES; i++)
	{
		chickens_[i] = std::make_unique<ClearChicken>();
		chickens_[i]->Create(pos[i]);
		chickens_[i]->SetTarget(DEFAULT_CAMERA_POS);
	}

	//プレイヤー
	player_ = std::make_unique<ClearPlayers>();
	player_->Init();

	//画像
	imgMes_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CONGRATULATIONS).handleId_;

	//エフェクト
	EffectManager::GetInstance().Add(EffectManager::EFFECT::FIREWORK,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::FIREWORK).handleId_);

	// カメラモード：定点カメラ
	auto camera = SceneManager::GetInstance().GetCameras();
	camera[0]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
	camera[0]->ChangeMode(Camera::MODE::FIXED_POINT);

	auto& efe = EffectManager::GetInstance();
	efe.Play(
		EffectManager::EFFECT::FIREWORK,
		EFFECT_POS,
		Quaternion(),
		EFFECT_SIZE,
		SoundManager::SOUND::NONE);

	//音関係
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::GAMECLEAR_SE,
		res.Load(ResourceManager::SRC::GAMECLEAR_SE).handleId_);
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAMECLEAR_BGM,
		res.Load(ResourceManager::SRC::GAMECLEAR_BGM).handleId_);

	//最初にSEを再生させる
	snd.Play(SoundManager::SOUND::GAMECLEAR_SE);

	ChangeState(STATE::HAPPY);
}

void GameClearScene::Update(void)
{	
	auto& efe = EffectManager::GetInstance();
	auto& ins = InputManager::GetInstance();
	auto& scm = SceneManager::GetInstance();
	auto& snd = SoundManager::GetInstance();

	stateUpdate_();

	//各種オブジェクト処理
	sky_->Update();
	stage_->Update();
	for (auto& c : chickens_) { c->Update(); }
	player_->Update();

	//エフェクトが非再生中の場合
	if (efe.IsPlayEffect(EffectManager::EFFECT::FIREWORK))
	{
		//エフェクトを再生
		efe.Play(
			EffectManager::EFFECT::FIREWORK,
			EFFECT_POS,
			Quaternion(),
			EFFECT_SIZE,
			SoundManager::SOUND::NONE);
	}

	// シーン遷移
	if (ins.IsTrgDown(KEY_INPUT_SPACE) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD3, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD4, InputManager::JOYPAD_BTN::RIGHT))
	{
		scm.ChangeScene(SceneManager::SCENE_ID::TITLE);
		snd.Stop(SoundManager::SOUND::GAMECLEAR_BGM);
		snd.Stop(SoundManager::SOUND::GAMECLEAR_SE);
		efe.Stop(EffectManager::EFFECT::FIREWORK);
	}
}

void GameClearScene::Draw(void)
{
	sky_->Draw();
	stage_->Draw();
	player_->Draw();
	for (auto& c : chickens_) { c->Draw(); }

	//メッセージの描画
	DrawRotaGraph(
		MES_POS_X, MES_POS_Y,
		1.0f,
		0.0f,
		imgMes_,
		true,
		false);
}

void GameClearScene::Release(void)
{
	sky_->Release();
	stage_->Destroy();
}

void GameClearScene::ChangeState(const STATE state)
{	
	// 状態受け取り
	state_ = state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();
}

void GameClearScene::ChangeStateHappy()
{
	stateUpdate_ = std::bind(&GameClearScene::UpdateHappy, this);
}

void GameClearScene::ChangeStateDisplay()
{	
	stateUpdate_ = std::bind(&GameClearScene::UpdateDisplay, this);
}

void GameClearScene::UpdateHappy(void)
{
	step_ += SceneManager::GetInstance().GetDeltaTime();

	if (step_ >= CHANGE_SECOND)
	{
		SoundManager::GetInstance().Stop(SoundManager::SOUND::GAMECLEAR_SE);
		SoundManager::GetInstance().Play(SoundManager::SOUND::GAMECLEAR_BGM);
		ChangeState(STATE::DISPLAY);
	}
}

void GameClearScene::UpdateDisplay(void)
{
	alpha_ += alphaAdd_;
	if (alpha_ > ALPHA_MAX) { alphaAdd_ = -1.0f; }
	else if (alpha_ < ALPHA_MIN) { alphaAdd_ = 1.0f; }
}

void GameClearScene::DebagPlay()
{
	/*auto& ins = InputManager::GetInstance();
	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		pos.x++;
	}
	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		pos.x--;
	}
	if (ins.IsNew(KEY_INPUT_UP))
	{
		pos.z++;
	}
	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		pos.z--;
	}*/
}
