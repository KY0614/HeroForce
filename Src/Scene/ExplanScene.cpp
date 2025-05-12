#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Decoration/SoundManager.h"
#include "../Manager/GameSystem/TextManager.h"
#include "ExplanScene.h"


ExplanScene::ExplanScene()
{
	int i = -1;
	cntExp_ = -1;
	alpha_ = -1.0;
	alphaAdd_ = -1.0;
	isStick_ = false;
	font_ = -1;
	imgPoint_ =&i;
	step_ = -1.0f;
	for (int i = 0; i < EXPLAN_CNT; i++) {
		imgExp_[i] = -1;
	}
}

void ExplanScene::Init()
{
	auto& res = ResourceManager::GetInstance();
	auto& snd = SoundManager::GetInstance();

	//画像読み込み
	imgExp_[0] = res.Load(ResourceManager::SRC::EXPLAN).handleId_;
	imgExp_[1] = res.Load(ResourceManager::SRC::EXPLAN2).handleId_;
	imgExp_[2] = res.Load(ResourceManager::SRC::EXPLAN3).handleId_;
	imgExp_[3] = res.Load(ResourceManager::SRC::EXPLAN4).handleId_;
	imgExp_[4] = res.Load(ResourceManager::SRC::EXPLAN5).handleId_;
	imgExp_[5] = res.Load(ResourceManager::SRC::EXPLAN6).handleId_;
	imgExp_[6] = res.Load(ResourceManager::SRC::EXPLAN7).handleId_;
	imgPoint_ = res.Load(ResourceManager::SRC::LEFT_POINT).handleIds_;

	//効果音
	SoundManager::GetInstance().Add(
		SoundManager::TYPE::SE,
		SoundManager::SOUND::EXPLAN_SWITCH,
		res.Load(ResourceManager::SRC::EXPLAN_SWITCH).handleId_);
	SoundManager::GetInstance().Add(
		SoundManager::TYPE::SE,
		SoundManager::SOUND::SCENE_CHANGE_1,
		res.Load(ResourceManager::SRC::SCENE_CHANGE_SE1).handleId_);


	//フォント生成
	font_ = CreateFontToHandle(
		TextManager::GetInstance().GetFontName(TextManager::FONT_TYPE::LOGO).c_str(),
		FONT_SIZE,
		0);

	//初期番号
	cntExp_ = 0;
	alphaAdd_ = 1.0f;
	alpha_ = ALPHA_MAX;
	step_ = 0.0f;
}

void ExplanScene::Update()
{
	auto& ins = InputManager::GetInstance();
	auto& snd = SoundManager::GetInstance();
	auto& mng = SceneManager::GetInstance();

	//シーン遷移
	if (ins.IsTrgDown(KEY_INPUT_SPACE) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD3, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD4, InputManager::JOYPAD_BTN::RIGHT))
	{
		snd.Play(SoundManager::SOUND::SCENE_CHANGE_1);
		mng.ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	//説明書を変える
	float stickX = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;
	if (ins.IsTrgDown(KEY_INPUT_A) ||
		ins.IsTrgDown(KEY_INPUT_LEFT) ||
		stickX < 0 && !isStick_)
	{
		//1ページ戻す
		cntExp_--;

		//0以下の時最大ページにする
		if (cntExp_ < 0) { cntExp_ = EXPLAN_CNT - 1; }

		//効果音再生
		snd.Play(SoundManager::SOUND::EXPLAN_SWITCH);

		//スティックは動作してる
		isStick_ = true;
	}
	else if (ins.IsTrgDown(KEY_INPUT_D) ||
		ins.IsTrgDown(KEY_INPUT_RIGHT) ||
		stickX > 0 && !isStick_)
	{
		//1ページ進む
		cntExp_++;

		//最大ページの時0ページに戻す
		if (cntExp_ >= EXPLAN_CNT) { cntExp_ = 0; }

		//効果音再生
		snd.Play(SoundManager::SOUND::EXPLAN_SWITCH);

		//スティックは動作してる
		isStick_ = true;
	}

	//透過計算
	alpha_ += alphaAdd_;
	if (alpha_ > ALPHA_MAX) { alphaAdd_ = -1.0f; }
	else if (alpha_ < ALPHA_MIN) { alphaAdd_ = 1.0f; }

	//スティックを戻したとき判定を戻す
	if (stickX == 0) { isStick_ = false; }
}

void ExplanScene::Draw()
{
	//説明書
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		EXP_RATE,
		0.0f,
		imgExp_[cntExp_],
		true
	);
	//ポイント左
	DrawRotaGraph(
		POINT_SIZE / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f,
		0.0f,
		imgPoint_[0],
		true);
	//ポイント右
	DrawRotaGraph(
		Application::SCREEN_SIZE_X - POINT_SIZE / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f,
		0.0f,
		imgPoint_[0],
		true,
		true);//反転させる

	//メッセージ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y - FONT_SIZE,
		0xffffff,
		font_,
		"SPACE or B スキップ");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ExplanScene::Release()
{
	DeleteFontToHandle(font_);
	SceneManager::GetInstance().ResetCameras();
}
