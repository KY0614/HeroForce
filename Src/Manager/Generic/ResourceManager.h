#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	//カーソル画像用定数
	static constexpr int CARSORS_NUM_X = 4;
	static constexpr int CARSORS_NUM_Y = 1;
	static constexpr int CARSORS_SIZE = 64;

	//数字画像用定数
	static constexpr int NUMBERS_NUM_X = 5;
	static constexpr int NUMBERS_NUM_Y = 2;
	static constexpr int NUMBERS_SIZE = 128;

	//レベルアップ画面エフェクト用定数
	static constexpr int LV_EFE_NUM_X = 10;
	static constexpr int LV_EFE_NUM_Y = 6;
	static constexpr int LV_EFE_SIZE = 256;

	// リソース名
	enum class SRC
	{
		TITLE,
		GAMEOVER,
		RESTART,
		NEXT,
		//モデル
		STAGE_1,		//ステージ①
		PLAYER_MAGE,	//魔法使い（プレイヤー）
		PLAYER_KNIGHT,	//騎士（プレイヤー）
		PLAYER_ARCHER,	//弓使い（プレイヤー）
		PLAYER_AXEMAN,	//斧使い（プレイヤー）
		ENEMY_MAGE,		//魔法使い（敵）
		ENEMY_BRIGANT,	//山賊（敵）
		ENEMY_ARCHER,	//弓使い（敵）
		ENEMY_AXEMAN,	//斧使い（敵）
		ENEMY_GOLEM,	//ゴーレム(敵)

		//ステージ
		STAGE_01,
		DECO_01,
		SKY_DOME,

		//UI
		HP_PLAYER,
		HP_ENEMY,
		HP_CPU,
		HP_GAGE,
		CIRCLE_GAGE,
		CIRCLE_EXP_GAGE,
		NUMBERS,

		//レベル通知用
		LEVEL_UP,
		SELECT_UI,
		CARSOLS,
		LEVEL_SCREEN_EFE,

		//強化
		ATTACK_UP_UI,
		DEFENCE_UP_UI,
		SPEED_UP_UI,
		LIFE_UP_UI,
		ATTACK_UP_EFE,
		DEFENCE_UP_EFE,
		SPEED_UP_EFE,
		LIFE_UP_EFE,

		//CPU
		CHICKEN,
		HELP,

	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);
	//シーンごとにデータを読み込むことにする
	void InitTitle(void);
	void InitSelect(void);
	void InitGame(void);
	void InitResult(void);

	void ResourcePlayer(void);
	void ResourceEnemy(void);
	void ResourceStage(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void) = default;

	// 内部ロード
	Resource* _Load(SRC src);

};

