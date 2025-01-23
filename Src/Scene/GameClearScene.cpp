#include "../Object/Stage/SkyDome.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Character/Chiken/ClearChicken.h"
#include "GameClearScene.h"

GameClearScene::GameClearScene()
{
}

void GameClearScene::Init(void)
{
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
}

void GameClearScene::Update(void)
{	
	auto& efe = EffectManager::GetInstance();

	//各種オブジェクト処理
	sky_->Update();
	stage_->Update();
	for (auto& c : chickens_) { c->Update(); }

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

	//シーン遷移
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameClearScene::Draw(void)
{
	sky_->Draw();
	stage_->Draw();
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
