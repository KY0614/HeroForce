#pragma once
#include <chrono>
#include<windows.h>
#include<vector>
#include<memory>
#include"../../Application.h"
#include"Camera.h"
#include"../../Common/Fader.h"

class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:
	static constexpr int PLAYER_NUM = 4;	//プレイヤー人数
	//デルタタイム
	static constexpr float DELTA_TIME = 1.0f / 60.0f;
	static constexpr float DELTA_TIME_INIT = 0.016f;

	//ウィンドウ関係
	static constexpr int WINDOW_MAX_X = 2;
	static constexpr int WINDOW_MARGIN = 30;
	static constexpr int MAIN_WINDOW_NUM = 1;

	//３D関係設定用
	static constexpr int BACK_COLOR = 0;
	static constexpr int FOG_COLOR = 5;
	static constexpr float FOG_START = 10000.0f;
	static constexpr float FOG_END = 20000.0f;
	static constexpr VECTOR LIGHT_DIR = { 0.00f, -1.00f, 1.00f };

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		SELECT,
		GAME,
		GAMECLEAR,
		GAMEOVER,
		EXP,
	};

	//コントローラ系統
	enum class CNTL
	{
		NONE,
		KEYBOARD,
		PAD
	};


	//プレイヤーモード
	enum class PLAY_MODE
	{
		USER,
		CPU,
		MAX
	};

	//役職
	enum class ROLE
	{
		NONE,
		KNIGHT,
		AXEMAN,
		MAGE,
		ARCHER
	};
	
	//定数

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);

	// シーンIDの取得
	SCENE_ID GetSceneID(void);

	// デルタタイムの取得
	float GetDeltaTime(void) const;

	/// <summary>
	/// カメラ取得
	/// </summary>
	/// <returns>カメラ(動的配列なのでアクセス範囲注意)</returns>
	std::vector<std::shared_ptr<Camera>> GetCameras(void) const;
	
	/// <summary>
	/// カメラを一つに戻す
	/// </summary>
	void ResetCameras(void);


	//ウィンドウ関係
	// ************************************************************************
	
	/// <summary>
	/// ウィンドウ追加
	/// </summary>
	/// <param name="_mode">追加するウィンドウ情報</param>
	void SetSubWindowH(HWND _mode);

	/// <summary>
	/// ウィンドウ準備(同数のカメラも生成する)
	/// </summary>
	void RedySubWindow(void);

	/// <summary>
	/// ウィンドウの状態をかえる
	/// </summary>
	/// <param name="_mode">変更後の状態</param>
	void ChangeWindowMode(const Application::WINDOW _mode);


	/// <summary>
	/// メイン以外をHIDEに
	/// </summary>
	void SetHideSubWindows(void);

	/// <summary>
	/// ウィンドウを一つ(フルスク)に戻す
	/// </summary>
	void ReturnSolo(void);

	/// <summary>
	/// ウィンドウのパラメータ設定
	/// </summary>
	void SetWindowPram(void);

	/// <summary>
	/// 使用中のウィンドウ数取得
	/// </summary>
	/// <returns>ウィンドウ数</returns>
	const int GetActiveNum(void) { return activeWindowNum_; }

	//上記のセッター
	void SetActiveNum(const int _num) { activeWindowNum_ = _num; }

	/// <summary>
	/// 現在使用しているウィンドウ番号を取得
	/// </summary>
	/// <returns>ウィンドウ番号</returns>
	const int GetNowWindow(void)const;

private:
	// 静的インスタンス
	static SceneManager* instance_;

	//ウィンドウ関係
	std::vector<HWND> subWindowH_;	//ウィンドウハンドルの全体管理(動的配列)
	int activeWindowNum_;			//使用するウィンドウの数
	int nowWindowNum_;				//現在のウィンドウ

	SCENE_ID sceneId_;				//現在のシーン状態
	SCENE_ID waitSceneId_;			//次のシーン

	// フェード
	std::unique_ptr<Fader> fader_;

	// 各種シーン
	SceneBase* scene_;

	// カメラ
	std::vector<std::shared_ptr<Camera>> cameras_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	// コピーコンストラクタも同様
	SceneManager(const SceneManager&);
	// デストラクタも同様
	~SceneManager(void) = default;

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade(void);

	//現在何番目のウィンドウかをセット
	void SetNowWindow(const int _num);
};