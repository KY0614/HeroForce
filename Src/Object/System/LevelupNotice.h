#pragma once
#include <string>
#include <memory>
#include <functional>
#include "../../Common/Fader.h"
#include "LevelScreenManager.h"

class LevelScreenManager;

class LevelupNotice
{
public:

	//画像に関する描画の状態
	enum class STATE
	{
		NONE,
		FADE_IN,	//フェードイン
		MAINTAIN,	//維持
		FIN,		//終了
	};

	//拡大率
	static constexpr float EXPANSION_RATE = 3.0f;

	//拡大最大
	static constexpr float EXPANSION_MAX = 1.2f;

	//維持秒数
	static constexpr int MAINTAIN_SECONDS = 1.5 * Application::DEFAULT_FPS;

	//フォント関連
	static constexpr int FONT_TEXT_SIZE = 128;
	static constexpr int FONT_LEVEL_SIZE = 156;
	static constexpr int FONT_THICK = 3;

	//透過開始時間(レベルテキスト)
	static constexpr int START_ALPHA_LEVEL = 2 * Application::DEFAULT_FPS;
	
	//UI位置
	static constexpr int MES_TEXT_POS_X = FONT_TEXT_SIZE /2;
	static constexpr int MES_TEXT_POS_Y = 100;
	static constexpr int LEVEL_TEXT_POS_X = 8;
	static constexpr int LEVEL_TEXT_POS_Y = 70;


	LevelupNotice();
	~LevelupNotice();

	void Init();
	void Update();
	void Draw();
	void Release();
	
	//読み込み
	void Load();

	//変数の初期化
	void Reset();

	//状態の設定
	void ChangeState(const STATE state);

	//レベルの設定
	void SetNewLevel(const int& newLevel);

	//状態を返す
	inline STATE GetState() const { return state_; }

private:

	//状態
	STATE state_;

	//画像
	int img_;

	//カウント
	int cnt_;

	//レベル
	int newLevel_;

	//大きさ
	float scl_;
	
	//アルファ値
	float alphaMes_;
	float alphaLevel_;

	//フォント
	int fontMes_;
	int fontLevel_;

	//テキスト
	std::string text_;

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	// 状態管理
	std::function<void(void)> stateUpdate_;	//更新

	//状態変更処理
	void ChangeStateNone();
	void ChangeStateFade();
	void ChangeStateMaintain();
	void ChangeStateFin();

	//更新処理
	void UpdateNone();
	void UpdateFade();
	void UpdateMaintain();
	void UpdateFin();

	//描画
	void DrawMessage();
	void DrawLevel();

};

