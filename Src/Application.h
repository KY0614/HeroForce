#pragma once
#include <string>

#define DEBUG_SCREEN

class Application
{

public:
#ifdef DEBUG_SCREEN
	static constexpr int SCREEN_SIZE_X = 800;
	static constexpr int SCREEN_SIZE_Y = 600;
#else
	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1920;
	static constexpr int SCREEN_SIZE_Y = 1080;
#endif // DEBUG_SCREEN



	//FPS
	static constexpr int DEFAULT_FPS = 60;
	static constexpr float frameRate = 1000 / 60;

	// データパス関連
	//-------------------------------------------
	static const std::string PATH_IMAGE;
	static const std::string PATH_MODEL;
	static const std::string PATH_ENEMY;
	static const std::string PATH_PLAYER;
	static const std::string PATH_STAGE;
	static const std::string PATH_OBJECT;
	static const std::string PATH_EFFECT;
	//-------------------------------------------

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

	//フレーム固定用
	int currentFrame_;	//現在のフレームを保存
	int lastFrame_;		//最後に実行したフレームを保存

	// 静的インスタンス
	static Application* instance_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application(void);

	// コピーコンストラクタも同様
	Application(const Application&);

	// デストラクタも同様
	~Application(void) = default;

};