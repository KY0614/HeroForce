#pragma once
#include <functional>
#include "SceneBase.h"
#include "../Manager/Generic/SceneManager.h"

class SkyDome;
class StageManager;
class SelectCharacter;
class WaitEnemy;
class SelectImage;

class SelectScene :public SceneBase
{
public:

	//サウンド関連
	static constexpr int BGM_VOLUME = 128;

	//キャラクター関連
	static constexpr int PLAYER_NUM = SceneManager::PLAYER_NUM;	//最大プレイ人数

	static constexpr int ROLE_NUM = SceneManager::PLAYER_NUM;	//最大役職数

	static constexpr float CHARACTER_SCALE = 0.5f;				//キャラクターの拡大率

	//カメラ関連
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };		//カメラの座標
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//カメラの注視点座標


	//画像関連
	static constexpr int IMAGE_POS_Y = 60;		//画像のY座標
	static constexpr int WAITIMAGE_POS_Y = 60;	//待機中画像のY座標

	//選択している種類
	enum class SELECT 
	{
		DISPLAY,	//ディスプレイ数
		NUMBER,		//人数
		OPERATION,	//1Pをキーボード操作にするかどうか
		ROLE,		//役職
		MAX
	};

	//キーコンフィグ
	enum class  KEY_CONFIG
	{
		NONE,		//何も入力していない

		UP,			//上
		DOWN,		//下
		LEFT,		//左
		RIGHT,		//右
					
		DECIDE,		//決定
		CANCEL		//キャンセル
	};

	//デバイス情報
	struct Device
	{
		SceneManager::CNTL cntl_;	//入力するデバイス
		KEY_CONFIG config_;			//キーコンフィグ
	};	

	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	// 初期化処理
	virtual void Init(void) override;
	// 更新ステップ
	virtual void Update(void) override;
	// 描画処理
	virtual void Draw(void) override;
	// 解放処理
	virtual void Release(void) override;

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="_state">遷移する状態</param>
	void ChangeSelect(const SELECT _state);

	/// <summary>
	/// 1Pのキー入力とコントローラ入力の操作処理
	/// </summary>
	/// <param name=""></param>
	void Process1PInput(void);

	/// <summary>
	/// キーボード操作処理
	/// </summary>
	void KeyBoardProcess(void);

	/// <summary>
	/// コントローラー操作処理
	/// </summary>
	void PadProcess(void);

	/// <summary>
	/// 1Pの入力デバイスを設定する
	/// </summary>
	/// <param name="cntl">デバイスの種類</param>
	void Change1PDevice(SceneManager::CNTL cntl){ input_[0].cntl_ = cntl; }

	/// <summary>
	/// 1Pの入力デバイスの制御
	/// </summary>
	void Control1PDevice(void);

	/// <summary>
	/// 全員が準備完了かどうか返す
	/// </summary>
	/// <returns>true:全員準備完了 false:誰か一人でも完了ではない場合</returns>
	bool IsAllReady(void);

	//ゲッター(取得用関数)----------------------------------------------------------

	/// <summary>
	/// 1Pの入力デバイスを取得する
	/// </summary>
	/// <returns>1Pの入力デバイス</returns>
	SceneManager::CNTL Get1PDevice(void) const{ return input_[0].cntl_; }

	/// <summary>
	/// 1P入力キーコンフィグを取得
	/// </summary>
	/// <returns>1Pの入力キーコンフィグ</returns>
	KEY_CONFIG Get1PConfig(void) const{ return input_[0].config_; }

	/// <summary>
	/// 現在の選択フェーズを取得
	/// </summary>
	/// <returns>現在のselectフェーズ</returns>
	SELECT GetSelect(void) const{ return select_; }

private:

	//状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;
	std::function<void(void)> stateDraw_;
	//状態管理(状態遷移時初期処理)
	std::map<SELECT, std::function<void(void)>> stateChanges_;

	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	//キャラクター
	std::unique_ptr<SelectCharacter>characters_[SceneManager::PLAYER_NUM];

	// 画像
	std::unique_ptr<SelectImage>images_[SceneManager::PLAYER_NUM];

	//待機中に出す敵
	std::unique_ptr<WaitEnemy>enemys_[SceneManager::PLAYER_NUM];

	//背景のステージ
	std::unique_ptr<StageManager> stage_;

	//選択中の種類
	SELECT select_;

	//キーコンフィグ
	KEY_CONFIG key_;

	//それぞれのプレイヤーのデバイスと入力
	Device input_[SceneManager::PLAYER_NUM];

	//プレイヤーが準備完了かどうか(true:準備完了 false:準備未完了)
	bool isOk_[SceneManager::PLAYER_NUM];

	//画像関連
	int imgDisplay_;		//ディスプレイ数選択を促す画像
	int imgPlayer_;			//プレイ人数選択を促す画像
	int imgOperation_;		//仕様デバイス選択を促す画像
	int imgRole_;			//役職選択を促す画像
	int imgWait_;			//待機中の画像

	//状態遷移
	void ChangeStateDisplay(void);
	void ChangeStateNumber(void);
	void ChangeStateOperation(void);
	void ChangeStateRole(void);
	void ChangeStateMax(void);

	//更新処理関連-----------------------------------------------

	void DisplayUpdate(void);		//ディスプレイ数選択中の処理

	void NumberUpdate(void);		//人数選択中の処理

	void OperationUpdate(void);		//操作方法選択中の処理(1Pのみ)

	void RoleUpdate(void);			//役職選択中の処理

	void MaxUpdate(void);			//無

	//描画処理関連-----------------------------------------------

	void DisplayDraw(void);			//ディスプレイ数選択中の処理

	void NumberDraw(void);			//人数選択中の処理

	void OperationDraw(void);		//操作方法選択中の処理(1Pのみ)

	void RoleDraw(void);			//役職選択中の処理

	void MaxDraw(void);				//無
};

