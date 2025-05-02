#pragma once
#include <vector>
#include <string>
#include<memory>
#include "../../Common/Vector2.h"
#include "LevelScreenManager.h"

class Carsor;
class LevelScreenManager;

class LevelupSelect
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
	static constexpr float SCALE_MAX = 0.7f;//Application::SCREEN_SIZE_X * 1.5f / Application::DEFA_SCREEN_SIZE_X;

	//フォント関連
	static constexpr int FONT_MES_SIZE = 36;//Application::SCREEN_SIZE_X * 72 / Application::DEFA_SCREEN_SIZE_X;
	static constexpr int FONT_MES_THICK = 5;
	static constexpr int FONT_EXP_SIZE = 24;//Application::SCREEN_SIZE_X * 36 / Application::DEFA_SCREEN_SIZE_X;
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

	LevelupSelect();
	~LevelupSelect();

	void Init();
	void Update();
	void Draw();
	void Release();

	//変数の初期化
	void Reset();

	//読み込み
	void Load();

	//状態の受け取り
	STATE GetState() const { return state_; }

	//種類の受け取り
	inline LevelScreenManager::TYPE GetType(const int plNum) { return selectTypes_[plNum]; }
	
	//種類のランダム決定
	inline LevelScreenManager::TYPE GetRandType();

	//スキップ用処理
	void SetSkipState();

private:

	//デバッグ用プレイヤー人数
	int plNum_;

	//画像
	int img_;
	int imgSelects_[LevelScreenManager::TYPE_MAX];
	int *imgCarsors_;
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
		float scl_;

		//座標
		Vector2 pos_;

		//サイズ
		Vector2 size_;

		//種類
		LevelScreenManager::TYPE type_;

		//テキストの表示
		bool isText_;
	};

	//エレメント
	std::vector<Element> ele_;

	//状態
	STATE state_;

	//テキスト
	std::string mesText_;
	std::string expTexts_[LevelScreenManager::TYPE_MAX];

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	// 状態管理
	std::function<void(void)> stateUpdate_;	//更新

	//カーソル
	std::vector<std::unique_ptr<Carsor>> carsors_;

	//初期座標の設定
	void SetFirstPos();

	//フォント生成
	void CreateFonts();

	//衝突判定
	bool IsCollisionBoxCircle(Vector2 pos1, Vector2 size1, Vector2 pos2, float radius2);	//計算処理
	Vector2 V2Sub(Vector2 value1, Vector2 value2);

	//状態変更処理
	void ChangeState(const STATE state);
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

