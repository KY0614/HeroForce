#pragma once
#include <chrono>
#include<windows.h>
#include<vector>

#include"../Application.h"

class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:

	static constexpr int PLAYER_NUM = 4;	//プレイヤー人数

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		SELECT,
		GAME,
		CLEAR,
	};

	//コントローラ系統
	enum class CNTL
	{
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

	// カメラの取得
	std::vector<std::unique_ptr<Camera>> GetCameras(void) const;

	//ウィンドウセッター
	void SetSubWindowH(HWND _mode);
	//モードを変える
	void ChangeWindowMode(const Application::WINDOW _mode);
	//フルスクに戻す
	void ReturnSolo(void);

	//ウィンドウの位置やサイズの調整
	void SetWindowPram(void);

	//使用するウィンドウの数のゲッター
	const int GetActiveNum(void) { return activeWindowNum_; }
	//上記のセッター
	void SetActiveNum(const int _num) { activeWindowNum_ = _num; }

private:
	// 静的インスタンス
	static SceneManager* instance_;

	//ウィンドウ関係
	std::vector<HWND> subWindowH_;	//ウィンドウハンドルの全体管理(動的配列)
	int activeWindowNum_;			//使用するウィンドウの数

	SCENE_ID sceneId_;				//現在のシーン状態
	SCENE_ID waitSceneId_;			//次のシーン

	// フェード
	Fader* fader_;

	// 各種シーン
	SceneBase* scene_;

	// カメラ
	std::vector<std::unique_ptr<Camera>> cameras_;

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

};