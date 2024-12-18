#pragma once
#include <functional>
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"
#include "../Common/Vector2.h"

class SkyDome;
class StageManager;
class SelectPlayer;
class SelectImage;

class SelectScene :public SceneBase
{
public:

	//#define DEBUG_RECT

	//三角形用の定数
	static constexpr int TRI_SCALE = 150;	//大きさ
	static constexpr int TRI_POS_X = Application::SCREEN_SIZE_X / 2;	//X座標
	static constexpr int TRI_POS_Y = Application::SCREEN_SIZE_Y / 2;	//Y座標

	//四角形の大きさ
	static constexpr int RECT_SCALE = 300;

	//図形同士の間隔
	static constexpr int PRI_SPACE = 100;


	////キー押下経過時間
	//static constexpr float SELECT_TIME = 1.0f;

	////インターバル上限
	//static constexpr float INTERVAL_TIME = 0.5f;

	static constexpr int PLAYER_NUM = SceneManager::PLAYER_NUM;
	static constexpr int ROLE_NUM = SceneManager::PLAYER_NUM;

	//カメラ関連
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };		//カメラの座標
	static constexpr VECTOR DEFAULT_TARGET_ONE = { 0.0f, 150.0f, -100.0f };		//カメラの注視点座標
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//カメラの注視点座標
	static constexpr VECTOR DEFAULT_TARGET_TWO = { 100.0f, 150.0f, -100.0f };		//カメラの注視点座標
	static constexpr VECTOR DEFAULT_TARGET_THREE = { 0.0f, 150.0f, 100.0f };		//カメラの注視点座標
	static constexpr VECTOR DEFAULT_TARGET_FOUR = { -100.0f, 150.0f, -100.0f };		//カメラの注視点座標

	static constexpr float CHARACTER_SCALE = 0.5f;

	//選択している種類
	enum class SELECT 
	{
		NUMBER,		//人数
		OPERATION,	//1Pをキーボード操作にするかどうか
		ROLE,		//役職
		MAX
	};

	// キーコンフィグ
	enum class  KEY_CONFIG
	{
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT,

		UP_TRG,
		DOWN_TRG,
		LEFT_TRG,
		RIGHT_TRG,

		DECIDE
	};

	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void Release(void) override;

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="_state">遷移する状態</param>
	void ChangeSelect(const SELECT _state);

	


	////選択するもの(人数or役職)の種類を変える
	//void ChangeSelect(SELECT select);

	//キー入力とコントローラ入力を共通化
	void KeyConfigSetting(void);

	//今使用している入力デバイスを取得する
	SceneManager::CNTL GetDevice(void);

	//入力デバイス変更(もうちょっといい実装方法がありそう)
	void ChangeDevice(SceneManager::CNTL device);	

	//キー入力とパッド入力の制御
	void ControllDevice(void);

	//ゲッター	----------------------------------------------------------

	KEY_CONFIG GetKeyConfig(void) { return key_; }	//入力キーを取得

	SELECT GetSelect(void) { return select_; }		//現在の選択フェーズを取得

	//デバッグ関連--------------------------------------------------------

	//デバッグ描画
	void DrawDebug(void);

	//--------------------------------------------------------------------

private:

	//状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;
	//状態管理(状態遷移時初期処理)
	std::map<SELECT, std::function<void(void)>> stateChanges_;

	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	//プレイヤー
	std::shared_ptr<SelectPlayer>players_[SceneManager::PLAYER_NUM];

	// 画像
	std::unique_ptr<SelectImage>images_[SceneManager::PLAYER_NUM];
	std::unique_ptr<SelectImage>image_;

	//背景のステージ
	StageManager* stage_;

	Transform trans_;
	Transform tests_[SceneManager::PLAYER_NUM];

	//選択中の種類
	SELECT select_;

	//デバイス
	SceneManager::CNTL device_;

	//キーコンフィグ
	KEY_CONFIG key_;

	//状態遷移
	void ChangeStateNumber(void);
	void ChangeStateOperation(void);
	void ChangeStateRole(void);
	void ChangeStateMax(void);

	//更新処理関連-----------------------------------------------

	void NumberUpdate(void);		//人数選択中の処理

	void OperationUpdate(void);		//操作方法選択中の処理(1Pのみ)

	void RoleUpdate(void);			//役職選択中の処理

	void MaxUpdate(void);			//無

	//描画処理関連-----------------------------------------------

	void NumberDraw(void);			//人数選択中の処理

	void OperationDraw(void);		//操作方法選択中の処理(1Pのみ)

	void RoleDraw(void);			//役職選択中の処理

	//-----------------------------------------------------------

	//デバッグ関連-------------------------------
	SceneManager::CNTL selectedCntl_;
	
};

