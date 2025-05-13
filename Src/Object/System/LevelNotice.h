#pragma once
#include <string>
#include <memory>
#include <functional>
#include "../../Common/Fader.h"
#include "../../Manager/Decoration/EffectManager.h"
#include "LevelScreenManager.h"

class LevelScreenManager;

class LevelNotice
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

	//エフェクト拡大率
	static constexpr float EFFECT_RATE = 1.5f;

	//維持秒数
	static constexpr float MAINTAIN_SECONDS =1.5f;

	//フォント関連
	static constexpr int FONT_TEXT_SIZE = Application::SCREEN_SIZE_X * 156 / Application::DEFA_SCREEN_SIZE_X;
	static constexpr int FONT_LEVEL_SIZE = Application::SCREEN_SIZE_X * 172 / Application::DEFA_SCREEN_SIZE_X;
	static constexpr int FONT_THICK = 3;

	//透過開始時間(レベルテキスト)
	static constexpr float START_ALPHA_LEVEL =2;

	//エフェクトスピード
	static constexpr int EFFECT_ANIM_SPPED = 3;
	
	//UI位置
	static constexpr int MES_TEXT_POS_X = FONT_TEXT_SIZE /2;
	static constexpr int MES_TEXT_POS_Y = 100;
	static constexpr int LEVEL_TEXT_POS_X = 8;
	static constexpr int LEVEL_TEXT_POS_Y = 70;
	static constexpr int EFFECT_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int EFFECT_POS_Y = (Application::SCREEN_SIZE_Y - FONT_LEVEL_SIZE) / 2 + LEVEL_TEXT_POS_Y;

	//コンストラクタ
	LevelNotice();

	//デストラクタ
	~LevelNotice() = default;

	void Init();
	void Update();
	void Draw();
	void Release();
	
	//読み込み
	void Load();

	//リセット
	void Reset();

	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="_state"></param>状態
	void ChangeState(const STATE _state);

	/// <summary>
	/// 新しいレベル設定
	/// </summary>
	/// <param name="_newLevel"></param>新しいレベル
	void SetNewLevel(const int& _newLevel);

	//状態を返す
	inline const STATE GetState() const { return state_; }

private:
	
	//エフェクト画像
	int *imgEfe_;

	//エフェクト更新用ステップ
	int efeStep_;

	//エフェクトのアニメーション番号
	int efeAnimNum_;

	//エフェクトのスピード
	int efeSpeed_;

	//エフェクトの再生
	bool isEfe_;

	//状態
	STATE state_;

	//カウント
	float cnt_;

	//レベル
	int newLevel_;

	//大きさ
	float scl_;
	
	//アルファ値
	float alphaMes_;	//メッセージ用
	float alphaLevel_;	//レベル用

	//フォント
	int fontMes_;		//メッセージ用フォント
	int fontLevel_;		//レベル用フォント

	//テキスト
	std::string text_;

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void()>> stateChanges_;

	// 状態管理
	std::function<void()> stateUpdate_;	//更新

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

	//エフェクト処理
	void EffectUpdate();

	//描画
	void DrawMessage();
	void DrawLevel();

};

