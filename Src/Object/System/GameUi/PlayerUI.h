#pragma once
#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/GameSystem/DataBank.h"
#include "../../UnitBase.h"
#include "GamaUIBase.h"

class UnitBase;

class PlayerUI : public GamaUIBase
{
public:

	enum class MESH_TYPE
	{
		HP,
		HP_GAGE,
		PARAM,
		NAMUBER,
	};

	enum class PARAM_TYPE
	{
		ATK,
		DEF,
		MAX
	};

	//パラメーター桁数
	static constexpr int PARAM_DIG_CNT = 3;
	static constexpr int PARAM_TYPE_MAX = static_cast<int>(PARAM_TYPE::MAX);

	//ボックスアルファ値
	static constexpr int BOX_ALPHA = 150;

	//拡大率
	static constexpr float ICON_RATE = 1.0f;
	static constexpr float HP_RATE = 1.0f;
	static constexpr float HP_GAGE_RATE = 1.0f;
	static constexpr float PARAM_RATE = 1.2f;
	static constexpr float NAME_RATE = 1.0f;
	static constexpr float NUMBER_RATE = 0.15f;

	//サイズ
	static constexpr int BOX_SIZE_X = 280;
	static constexpr int BOX_SIZE_Y = 130;
	static constexpr int ICON_SIZE = 128 * ICON_RATE;
	static constexpr int PARAM_SIZE_X = 128 * PARAM_RATE;
	static constexpr int PARAM_SIZE_Y = 24 * PARAM_RATE;
	static constexpr int NAME_SIZE_X = 150 * NAME_RATE;
	static constexpr int NAME_SIZE_Y = 180 * NAME_RATE;
	static constexpr int NUMBER_SIZE = 128 * NUMBER_RATE;
	static constexpr int P_HP_SIZE_X = HP_BAR_SIZE_X * HP_RATE;
	static constexpr int P_HP_SIZE_Y = HP_BAR_SIZE_Y * HP_RATE;
	static constexpr int P_HP_GAGE_SIZE_X = HP_GAGE_SIZE_X * HP_GAGE_RATE;
	static constexpr int P_HP_GAGE_SIZE_Y = HP_GAGE_SIZE_Y * HP_GAGE_RATE;

	//位置
	static constexpr int BOX_POS_X = 0;
	static constexpr int BOX_POS_Y = Application::SCREEN_SIZE_Y;
	static constexpr int BOX_POS_X2 = BOX_POS_X + BOX_SIZE_X;
	static constexpr int BOX_POS_Y2 = BOX_POS_Y - BOX_SIZE_Y;
	static constexpr int ICON_POS_X = ICON_SIZE / 2;
	static constexpr int ICON_POS_Y = Application::SCREEN_SIZE_Y - ICON_SIZE / 2;
	static constexpr int NAME_POS_X = NAME_SIZE_X / 2 + 20;
	static constexpr int NAME_POS_Y = Application::SCREEN_SIZE_Y - ICON_SIZE;
	static constexpr int HPGAGE_POS_X = ICON_SIZE + P_HP_GAGE_SIZE_X / 2 - 10;
	static constexpr int HPGAGE_POS_Y = Application::SCREEN_SIZE_Y - P_HP_GAGE_SIZE_Y / 2 - 20;

	static constexpr int HP_POS_X = ICON_SIZE + 2;
	static constexpr int HP_POS_X2 = HP_POS_X + P_HP_GAGE_SIZE_X - 20;
	static constexpr int HP_POS_Y = Application::SCREEN_SIZE_Y - P_HP_GAGE_SIZE_Y - 7;
	static constexpr int HP_POS_Y2 = HP_POS_Y + P_HP_GAGE_SIZE_Y - 15;

	static constexpr int PARAM_POS_X = ICON_SIZE + PARAM_SIZE_X / 2 - 10;
	static constexpr int PARAM_POS_Y = Application::SCREEN_SIZE_Y - P_HP_GAGE_SIZE_Y - PARAM_SIZE_Y / 2 - 15;

	static constexpr int NUM_POS_X[PARAM_TYPE_MAX][PARAM_DIG_CNT] = { {
			ICON_SIZE + 25 ,
			ICON_SIZE + NUMBER_SIZE + 20 ,
			ICON_SIZE + NUMBER_SIZE * 2 + 15,},{
			ICON_SIZE + NUMBER_SIZE * 3 + 40 ,
			ICON_SIZE + NUMBER_SIZE * 4 + 35,
			ICON_SIZE + NUMBER_SIZE * 5 + 30 } };
	static constexpr int NUM_POS_Y = Application::SCREEN_SIZE_Y - P_HP_GAGE_SIZE_Y - NUMBER_SIZE / 2 - 20;

	//HPバー長さ
	static constexpr float HP_BAR_LENGTH = HP_POS_X2 - HP_POS_X;

	//頂点数
	static constexpr int VERTEXS = 4;

	PlayerUI();
	~PlayerUI();

	void Init(UnitBase& player,const DataBank::PLAYER_INFO& info);
	void Load()override;
	void Update(UnitBase& player);
	void Draw()override;

private:

	//画像
	int *imgIcon_;		//アイコン
	int *imgName_;		//名前
	int imgParam_;		//パラメーター
	int *imgNumber_;	//数字

	//描画スクリーン
	int uiScreen_;

	//アイコン種類
	int iconNum_;

	//攻撃力
	int atk_;

	//防御力
	int def_;

	//バーの長さ
	float barLength_;

	//数字
	int nums_[PARAM_TYPE_MAX][PARAM_DIG_CNT];

	//パラメータ関連の描画
	void DrawParam();
};