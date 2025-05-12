#pragma once
#include <functional>
#include <memory>
#include <map>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Decoration/EffectManager.h"
#include "../Manager/Decoration/SoundManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/DataBank.h"
#include "../Object/Character/PlayableChara/Other/ClearPlayers.h"
#include "SceneBase.h"

class SkyDome;
class StageManager;
class ClearChicken;
class ClearPlayers;
class GameClearScene : public SceneBase
{
public:

	//状態
	enum class STATE
	{
		HAPPY,		//つかの間の喜び
		DISPLAY,	//メッセージの表示
	};

	//チキン生成数
	static constexpr int CHICKEN_CREATES = 4;

	//メッセージ位置
	static constexpr int MES_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES_POS_Y = Application::SCREEN_SIZE_Y / 2 - 100;

	static constexpr int MES2_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES2_POS_Y = Application::SCREEN_SIZE_Y / 2 + 100;

	//カメラ関連
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };		//カメラの座標
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//カメラの注視点座標

	static constexpr VECTOR CHICKEN_POS_1 = { -200.0f, -50.0f, 20.0f };
	static constexpr VECTOR CHICKEN_POS_2 = { -120.0f, -50.0f, 30.0f };
	static constexpr VECTOR CHICKEN_POS_3 = { 150.0f, -50.0f, 40.0f };
	static constexpr VECTOR CHICKEN_POS_4 = { 230.0f, -50.0f, 40.0f };

	//エフェクト再生位置
	static constexpr VECTOR EFC_POS = { 0.0f, -100.0f, 100.0f };		

	//エフェクトサイズ
	static constexpr float EFC_SIZE = 20.0f;

	//アルファ値最大
	static constexpr int ALPHA_MAX = 255;
	static constexpr int ALPHA_MIN = 130;

	//状態遷移秒数
	static constexpr float CHANGE_SECOND = 2.0f;

	//コンストラクタ
	GameClearScene();

	//デストラクタ
	~GameClearScene() = default;

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:

	//クリアメッセージ画像
	int imgClear_;

	//シーン遷移メッセージ画像
	int imgMes_;
	
	//更新用
	float step_;

	//アルファ値
	float alpha_;

	//アルファ追加値
	float alphaAdd_;

	//状態
	STATE state_;

	//スカイドーム
	std::unique_ptr<SkyDome> sky_;

	//チキン
	std::unique_ptr<ClearChicken> chickens_[CHICKEN_CREATES];

	//プレイヤー
	std::unique_ptr<ClearPlayers> player_;

	//ステージ
	std::unique_ptr<StageManager> stage_;

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void()>> stateChanges_;

	// 状態管理
	std::function<void()> stateUpdate_;	//更新

	//状態変更
	void ChangeState(const STATE state);
	void ChangeStateHappy();
	void ChangeStateDisplay();

	//各種更新処理
	void UpdateHappy();
	void UpdateDisplay();
};