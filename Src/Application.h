#pragma once
#include <string>
#include<windows.h>

//#define DEBUG_SCREEN

class Application
{

public:
#ifdef DEBUG_SCREEN
	static constexpr int SCREEN_SIZE_X = 800;
	static constexpr int SCREEN_SIZE_Y = 600;
	// スクリーンサイズ

	//static constexpr int SCREEN_SIZE_X = 1920;
	//static constexpr int SCREEN_SIZE_Y = 1080;
#endif // DEBUG_SCREEN


	static constexpr int DEFA_SCREEN_SIZE_X = 1920;
	static constexpr int DEFA_SCREEN_SIZE_Y = 1080;

	static constexpr int SCREEN_SIZE_X = 800;
	static constexpr int SCREEN_SIZE_Y = 600;

	//FPS
	static constexpr int DEFAULT_FPS = 60;
	static constexpr float FRAME_RATE = 1000 / 60;
	static constexpr float FPS_USE_SUBWINDOW = 1000000 / 60;

	//ウィンドウ
	static constexpr int SUBWINDOW_NUM = 3;

	// データパス関連
	//-------------------------------------------
	static const std::string PATH_IMAGE;
	static const std::string PATH_MODEL;
	static const std::string PATH_ENEMY;
	static const std::string PATH_PLAYER;
	static const std::string PATH_ARROW;
	static const std::string PATH_STAGE;
	static const std::string PATH_SKYDOME;
	static const std::string PATH_OBJECT;
	static const std::string PATH_EFFECT;
	static const std::string PATH_TEXT;
	static const std::string PATH_FONT;
	static const std::string PATH_JSON;
	static const std::string PATH_BGM;
	static const std::string PATH_WAVE;
	//-------------------------------------------

	//ウィンドウモード設定
	enum class WINDOW
	{
		HIDE,			//非表示
		NOMAL,			//通常
		MIN,			//最小化表示
		MAX,			//最大化表示
		NOACTIVE,		//表示するがアクティブではない
		SHOW,			//表示(現在状態に依存)
		MINIMIZE,		//最小化する
		MIN_NOACTIVE,	//最小化してアクティブをなくす
		SHOW_NOACTIVE,	//表示するがアクティブにはしない
		SIZE_RESET,		//サイズを元に戻す
		DEFAULT,		//初期状態で表示
	};

	//サブウィンドウの個数(大事なのは要素数)
	//メインウィンドウがすでにあるので追加で作るのは三枚まで
	const char* szClassNme[SUBWINDOW_NUM] =
	{
	"window1",
	"window2",
	"window3",
	//もっとウィンドウを作る場合はここを増やします
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Application& GetInstance(void);

	// 初期化
	void Init(void);

	// ゲームループの開始
	void Run(void);

	// リソースの破棄
	void Destroy(void);

	// 初期化成功／失敗の判定
	bool IsInitFail(void) const;

	// 解放成功／失敗の判定
	bool IsReleaseFail(void) const;

private:

	//ウィンドウハンドル
	HWND hWnd_;

	//フレーム固定用
	int currentFrame_;	//現在のフレームを保存
	int lastFrame_;		//最後に実行したフレームを保存

	// 静的インスタンス
	static Application* instance_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;


	//ウィンドウの初期化
	void InitWindows(const int _num);

	//エフェクシアの初期化
	void InitEffekseer(void);

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application(void);

	// コピーコンストラクタも同様
	Application(const Application&);

	// デストラクタも同様
	~Application(void) = default;

};