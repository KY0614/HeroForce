#pragma once
#include "SceneBase.h"
#include "../Application.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/SkyDome.h"

class SpaceDome;

class TitleScene : public SceneBase
{

public:

	//カメラ関連
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 300.0f, 0.0f };		//カメラの座標
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 1000.0f, 800.0f };		//カメラの注視点座標

	//ロゴ位置
	static constexpr int LOGO_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int LOGO_POS_Y = Application::SCREEN_SIZE_Y / 2 -30;

	//メッセージサイズ
	static constexpr int MES_SIZE_X = 300;
	static constexpr int MES_SIZE_Y = 44;

	//メッセージ位置
	static constexpr int MES_POS_X = Application::SCREEN_SIZE_X / 2 - MES_SIZE_X / 2;
	static constexpr int MES_POS_Y = Application::SCREEN_SIZE_Y - 100;


	// コンストラクタ
	TitleScene();

	//デストラクタ
	~TitleScene() = default;

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:

	//更新用ステップ
	float step_;

	// タイトルロゴ
	int imgLogo_;
	int imgMes_;

	//スカイドーム
	std::unique_ptr<SkyDome> sky_;

};
