#include <chrono>
#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include<cassert>

#include "../Scene/TitleScene.h"
#include "../Scene/SelectScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/GameOverScene.h"
#include "../Scene/GameClearScene.h"
#include "../Scene/ExplanScene.h"
#include "ResourceManager.h"
#include "Camera.h"
#include"../Manager/GameSystem/Collision.h"
#include"../Manager/GameSystem/DataBank.h"
#include"../Manager/GameSystem/CharacterParamData.h"
#include"../Manager/Decoration/EffectManager.h"
#include"../Manager/Decoration/SoundManager.h"
#include"../Manager/GameSystem/Timer.h"
#include"../Shader/PixelShader.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr){
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	//各マネージャの生成
	//判定の生成
	Collision::CreateInstance();
	DataBank::CreateInstance();
	EffectManager::CreateInstance();
	SoundManager::CreateInstance();
	PixelShader::CreateInstance();

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	CharacterParamData::CreateInstance();

	fader_ = std::make_unique<Fader>();
	fader_->Init();

	// カメラ
	std::shared_ptr<Camera>c = std::make_shared<Camera>();
	c->Init();
	cameras_.push_back(std::move(c));

	scene_ = new TitleScene();
	scene_->Init();

	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// 3D用の設定
	Init3D();

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);

	//メインウィンドウを追加
	subWindowH_.push_back(NULL);
	activeWindowNum_ = MAIN_WINDOW_NUM;	//メインをアクティブにするので初期値１
	nowWindowNum_ = 0;
}

void SceneManager::Init3D(void)
{

	// 背景色設定
	SetBackgroundColor(BACK_COLOR, BACK_COLOR ,BACK_COLOR);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);

	// 正面から斜め下に向かったライト
	ChangeLightTypeDir(LIGHT_DIR);

	// ライトの設定
	//ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// フォグ設定
	SetFogEnable(true);
	SetFogColor(FOG_COLOR, FOG_COLOR, FOG_COLOR);
	SetFogStartEnd(FOG_START, FOG_END);

}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)return;

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_){
		Fade();
	}
	else{
		scene_->Update();
	}

	// カメラ更新
	for (auto& c : cameras_){
		c->Update();
	}
}

void SceneManager::Draw(void)
{
	//カウンタ（配列要素数)
	int cnt = 0;

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	for (HWND hwnd : subWindowH_){
		//カウンタがウィンドウのアクティブ数を超えたら終了
		if (cnt >= activeWindowNum_)return;

		//フロントバッファの画像を消去
		ClearDrawScreen();

		if (hwnd){
			SetScreenFlipTargetWindow(hwnd); // 追加ウィンドウをターゲットに設定

		}else{
			SetScreenFlipTargetWindow(NULL); // メインウィンドウをターゲットに設定
		}
		//カメラの描画
		cameras_[cnt]->SetBeforeDraw();

		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();
		//現在のウィンドウ数セット
		SetNowWindow(cnt);

		//ゲーム内容描画
		// 描画
		scene_->Draw();

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();


		// 暗転・明転
		fader_->Draw();

		//フロントバッファに書き出し
		ScreenFlip();
		//カウンタ増加
		cnt++;
	}
}

void SceneManager::Destroy(void)
{

	scene_->Release();
	delete scene_;

	// カメラ
	for (auto& c : cameras_){
		c->Release();
	}

	DataBank::GetInstance().Destroy();
	EffectManager::GetInstance().Destroy();
	SoundManager::GetInstance().Destroy();
	PixelShader::GetInstance().Destroy();
	Timer::GetInstance().Destroy();

	delete instance_;
}

void SceneManager::ChangeScene(SCENE_ID nextId)
{

	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}
//カメラ情報（全体）を取得
std::vector<std::shared_ptr<Camera>> SceneManager::GetCameras(void) const
{
	return cameras_;
}
//カメラを要素数①になるまで削除する
void SceneManager::ResetCameras(void)
{
	auto size = cameras_.size();
	//末尾から消していくので最後の一つが残るように調整
	size --;
	for (int i = 0; i < size; i++){
		cameras_.pop_back();
	}
}

void SceneManager::SetSubWindowH(HWND _mode)
{
	subWindowH_.push_back(_mode);
}

void SceneManager::RedySubWindow(void)
{
	//ウィンドウの設定
	int num = DataBank::GetInstance().Output(DataBank::INFO::USER_NUM);
	SetActiveNum(num);
	ChangeWindowMode(Application::WINDOW::SHOW);
	SetWindowPram();

	//すでに一つは生成されているので初期値は①
	for (int i = 1; i < num; i++){
		//生成及び初期化
		auto c = std::make_shared<Camera>();
		c->Init();
		//格納
		cameras_.push_back(std::move(c));
	}
}

//ウィンドウの状態を変える
void SceneManager::ChangeWindowMode(const Application::WINDOW _mode)
{
	int cnt = MAIN_WINDOW_NUM;
	for (HWND hwnd : subWindowH_){
		if (cnt > activeWindowNum_)return;
		if (cnt == MAIN_WINDOW_NUM){
			cnt++;
			continue;
		}
		ShowWindow(hwnd, static_cast<int>(_mode));

		cnt++;
	}
}
//サブウィンドウを隠す状態に(ゲームシーンから別のシーンに移動するときに使用)
void SceneManager::SetHideSubWindows(void)
{
	//少し雑に作るので要見直し
	//もしかしたらこの関数すらいらない可能性はある
	int cnt = MAIN_WINDOW_NUM;
	for (HWND hwnd : subWindowH_){
		if (cnt == MAIN_WINDOW_NUM){	//メインウィンドウはスキップ
			cnt++;
			continue;
		}
		ShowWindow(hwnd, static_cast<int>(Application::WINDOW::HIDE));

		cnt++;
	}
}

//フルスクリーンに戻す
void SceneManager::ReturnSolo(void)
{
	//画面枚数を一枚に戻す
	SetActiveNum(MAIN_WINDOW_NUM);
	//サブウィンドウを隠す
	SetHideSubWindows();
	//フルスクに戻る
	SetWindowSize(Application::DEFA_SCREEN_SIZE_X, Application::DEFA_SCREEN_SIZE_Y);
}



SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	subWindowH_ = {};
	activeWindowNum_ = -1;
	nowWindowNum_ = -1;
	cameras_ = {};

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;
	// デルタタイム
	deltaTime_ = DELTA_TIME;	
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = DELTA_TIME_INIT;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	auto& resM = ResourceManager::GetInstance();

	// リソースの解放
	resM.Release();
	EffectManager::GetInstance().Release();
	SoundManager::GetInstance().Release();

	// シーンを変更する
	sceneId_ = sceneId;

	// 現在のシーンを解放
	if (scene_ != nullptr){
		scene_->Release();
		delete scene_;
	}

	switch (sceneId_){
	case SCENE_ID::TITLE:
		ChangeWindowMode(Application::WINDOW::HIDE);
		scene_ = new TitleScene();
		resM.InitTitle();
		break;		
	
	case SCENE_ID::SELECT:
		scene_ = new SelectScene();
   		resM.InitSelect();
		break;	
	
	case SCENE_ID::GAME:
		//ウィンドウの設定
		RedySubWindow();
		scene_ = new GameScene();
		resM.InitGame();
		break;

	case SCENE_ID::GAMEOVER:
		scene_ = new GameOverScene();
		resM.InitGameOver();
		break;

	case SCENE_ID::GAMECLEAR:
		scene_ = new GameClearScene();
		resM.InitGameClear();
		break;

	case SCENE_ID::EXP:
		scene_ = new ExplanScene();
		resM.InitExplan();
		break;
	}

	scene_->Init();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState){
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd()){
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd()){
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}
}

void SceneManager::SetNowWindow(const int _num)
{
	nowWindowNum_ = _num;
}

//ウィンドウのサイズ及び位置設定
void SceneManager::SetWindowPram(void)
{
	//現在はディスプレイ一枚と仮定して制作している
	//後で対応版の制作が必要
	const int DISPLAY_X = 1920;
	const int DISPLAY_Y = 1080;


	//配列の要素数なのです初期値０
	int cnt = 0;

	int posX = 0;
	int posY = 0;


	int displayNum = DataBank::GetInstance().Output(DataBank::INFO::DHISPLAY_NUM);
	int userNum = DataBank::GetInstance().Output(DataBank::INFO::USER_NUM);

	int r = 1;
	//ディスプレイが余っているとき
	if (displayNum > userNum) r = userNum;
	else r = displayNum;


	int sizeX = DISPLAY_X * r;
	int sizeY = DISPLAY_Y;

	if (activeWindowNum_ != displayNum)sizeX /= 2;
	if (activeWindowNum_ > displayNum)sizeY /= 2;
	if (sizeX > DISPLAY_X)sizeX = DISPLAY_X;

	for (HWND hwnd : subWindowH_){
		//cntは配列の要素数を表すのでactiveWindow未満の間だけ
		if (cnt >= activeWindowNum_)return;
		cnt++;

		if (cnt == MAIN_WINDOW_NUM){
			SetWindowSize(sizeX - WINDOW_MARGIN / 2, sizeY - WINDOW_MARGIN);
			//SetWindowSizeExtendRate(0.95, 0.95);
			SetWindowPosition(posX, posY);
		}
		else{
			SetWindowPos(hwnd, NULL, posX, posY, sizeX, sizeY, NULL);
			//MoveWindow(hwnd, posX, posY, sizeX, sizeY, true);
		}
		posX += sizeX;

		if (cnt == WINDOW_MAX_X){
			posY += sizeY;
			posX = 0;
		}
	}
}

const int SceneManager::GetNowWindow(void) const
{
	return nowWindowNum_;
}
