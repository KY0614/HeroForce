#pragma once
#include <vector>
#include <string>
#include<memory>
#include "../../Common/Vector2.h"
#include "Cursor.h"

class Cursor;

class LevelSelect
{
public:

	//状態
	enum class STATE
	{
		NONE,
		EXPANSION,
		SELECT,
		FIN, 
	};

	//強化項目画像サイズ
	static constexpr int ELE_IMG_SIZE_X = 340;
	static constexpr int ELE_IMG_SIZE_Y = 250;

	//選択項目数
	static constexpr int SELECT_ELEMENT = 4;

	//拡大率
	static constexpr float SCALE_RATE = 0.01f;

	//拡大最大
	static constexpr float SCALE_MAX = 0.7f;

	//フォント関連
	static constexpr int FONT_MES_SIZE = 36;
	static constexpr int FONT_MES_THICK = 5;
	static constexpr int FONT_EXP_SIZE = 24;
	static constexpr int FONT_EXP_THICK = 3;

	//メッセージ描画位置
	static constexpr int MES_TEXT_POS_X = Application::SCREEN_SIZE_X / 2 + 30;
	static constexpr int MES_TEXT_POS_Y = 48;

	//強化要素位置
	static constexpr int INTERVEL = 100;
	static constexpr int ELEMENT_POS_LU_X = Application::SCREEN_SIZE_X / 4 + 40;
	static constexpr int ELEMENT_POS_LU_Y = Application::SCREEN_SIZE_Y / 4 + 70;

	static constexpr int ELEMENT_POS_RU_X = Application::SCREEN_SIZE_X / 4 * 3 - 40;
	static constexpr int ELEMENT_POS_RU_Y = Application::SCREEN_SIZE_Y / 4 + 70;

	static constexpr int ELEMENT_POS_LD_X = Application::SCREEN_SIZE_X / 4 + 40;
	static constexpr int ELEMENT_POS_LD_Y = Application::SCREEN_SIZE_Y / 4 * 3 + 10;

	static constexpr int ELEMENT_POS_RD_X = Application::SCREEN_SIZE_X / 4 * 3 - 40;
	static constexpr int ELEMENT_POS_RD_Y = Application::SCREEN_SIZE_Y / 4 * 3 + 10;

	//背景アルファ値
	static constexpr int BACK_ALPHA = 150;

	//パーセント用数字
	static constexpr int POW = 30;

	//テキスト位置調整用
	static constexpr int TEXT_OFFSET_POS_Y = 90;

	//コンストラクタ
	LevelSelect();

	//デストラクタ
	~LevelSelect() = default;

	//初期化
	void Init();

	//更新
	void Update();

	//描画
	void Draw();

	//解放
	void Release();

	//変数の初期化
	void Reset();

	//読み込み
	void Load();

	/// <summary>
	/// 状態を返す
	/// </summary>
	/// <returns></returns>状態
	inline const STATE GetState() const { return state_; }

	/// <summary>
	/// 種類を返す
	/// </summary>
	/// <param name="plNum"></param>プレイヤー番号
	/// <returns></returns>選択した種類
	inline LevelScreenManager::TYPE GetType(const int _plNum) { return selectTypes_[_plNum]; }
	
	//スキップ用処理
	void SetSkipState();

private:

	//デバッグ用プレイヤー人数
	int plNum_;

	//画像
	int img_;
	int imgSelects_[LevelScreenManager::TYPE_MAX];
	int *imgCursors_;
	int imgBack_;

	//強化要素数
	int element_;

	//フォント
	int fontMes_;
	int fontExp_;

	//決定項目
	std::vector<LevelScreenManager::TYPE> selectTypes_;

	//強化項目
	struct Element
	{
		//拡大率
		float scl_ = 0.0f;

		//座標
		Vector2 pos_ = {};

		//サイズ
		Vector2 size_ = {};

		//種類
		LevelScreenManager::TYPE type_ = LevelScreenManager::TYPE::MAX;

		//テキストの表示
		bool isText_ = false;
	};

	//エレメント
	std::vector<Element> ele_;

	//状態
	STATE state_;

	//テキスト
	std::string mesText_;
	std::string expTexts_[LevelScreenManager::TYPE_MAX];

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void()>> stateChanges_;

	// 状態管理
	std::function<void()> stateUpdate_;	//更新

	//カーソル
	std::vector<std::unique_ptr<Cursor>> cursors_;

	//初期座標の設定
	void SetFirstPos();

	//フォント生成
	void CreateFonts();	
	
	//種類のランダム決定
	LevelScreenManager::TYPE GetRandType();

	//状態変更処理
	void ChangeState(const STATE _state);
	void ChangeStateNone();
	void ChangeStateExpansion();
	void ChangeStateSelect();
	void ChangeStateFin();

	//状態別更新処理
	void UpdateNone();
	void UpdateExpansion();
	void UpdateSelect();
	void UpdateFin();

	void DebagDraw();

};

