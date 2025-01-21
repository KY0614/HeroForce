#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

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
		SKY_DOME,
		STAGE_BARREL,
		STAGE_BENCH,
		STAGE_FENCES_LONG,
		STAGE_FENCES_SHORT,
		STAGE_GROUND,
		STAGE_HOUSE_01,
		STAGE_HOUSE_02,
		STAGE_HOUSE_03,
		STAGE_HOUSE_04,
		STAGE_HOUSE_05,
		STAGE_HUNGER_01,
		STAGE_HUNGER_02,
		STAGE_ROCK_01,
		STAGE_ROCK_02,
		STAGE_ROCK_03,
		STAGE_SACK,
		STAGE_TABLE,
		STAGE_TREE_01,
		STAGE_TREE_02,
		STAGE_TREE_03,
		STAGE_TREE_04,
		STAGE_TREE_05,
		STAGE_WAGON,
		STAGE_WELL,
		STAGE_WOOD,

		//レベル通知用
		LEVEL_UP,
		SELECT_UI,

		//CPU
		CHICKEN,

		//選択画面用
		PLAYER_NUM,		//人数選択
		RIGHT_POINT,	//右矢印
		LEFT_POINT,		//左矢印


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
	void InitGameOver(void);

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

