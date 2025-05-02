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

	//キャラクターパラメータ定数
	static constexpr int CHARA_PARAM_NUM_X = 1;
	static constexpr int CHARA_PARAM_NUM_Y = 4;
	static constexpr int CHARA_PARAM_SIZE_X = 640;
	static constexpr int CHARA_PARAM_SIZE_Y = 360;

	//スコアランク用定数
	static constexpr int RANKS_NUM_X = 4;
	static constexpr int RANKS_NUM_Y = 1;
	static constexpr int RANK_SIZE = 300;

	//煙
	static constexpr int SMOKE_NUM_X = 16;
	static constexpr int SMOKE_NUM_Y = 1;
	static constexpr int SMOKE_SIZE_X = 1024 / SMOKE_NUM_X;
	static constexpr int SMOKE_SIZE_Y = 77 / SMOKE_NUM_Y;

	//アイコン
	static constexpr int CHARA_ICON_NUM_X = 4;
	static constexpr int CHARA_ICON_NUM_Y = 1;
	static constexpr int CHARA_ICON_SIZE = 128;

	//キャラネーム
	static constexpr int NAME_NUM_X = 1;
	static constexpr int NAME_NUM_Y = 4;
	static constexpr int NAME_SIZE_X = 150 / NAME_NUM_X;
	static constexpr int NAME_SIZE_Y = 180 / NAME_NUM_Y;

	//スキルアイコン
	static constexpr int SKILL_ICON_NUM_X = 1;
	static constexpr int SKILL_ICON_NUM_Y = 2;
	static constexpr int SKILL_ICON_SIZE = 120;

	// リソース名
	enum class SRC
	{
		TITLE,
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
		ARROW,

		//ステージ
		STAGE_01,
		DECO_01,

		//ゲームクリア
		CONGRATULATIONS,
		FIREWORK,
		GAMECLEAR_SE,
		GAMECLEAR_BGM,

		//ゲームオーバー
		GAMEOVER,
		GAMEOVER_SE,
		GAMEOVER_BGM,
		GAMEOVER_BACK,

		//UI
		HP_PLAYER,
		HP_ENEMY,
		HP_CPU,
		HP_GAGE,
		CIRCLE_GAGE,
		CIRCLE_EXP_GAGE,
		NUMBERS,
		CHARA_PARAMS,
		TO_TITLE,
		START,
		TIME,
		MISSION_NOMAL,
		MISSION_LAST,
		CHANGE_TITLE_UI,
		CHARA_ICONS,
		HP_BOSS,
		HP_BOSS_GAGE,
		NUMBERS2,
		PARAM_UI,
		ROLE_NAMES,
		TIMER_BACK,

		//レベル通知用
		LEVEL_UP,
		SELECT_UI,
		CARSOLS,
		LEVEL_SCREEN_EFE,
		LEVEL_UP_BACK,


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

		//選択画面用
		PLAYER_NUM,		//人数選択
		DISPLAY_NUM,	//人数選択
		RIGHT_POINT,	//右矢印
		LEFT_POINT,		//左矢印
		READY,			//準備
		DEVICE,			//デバイス
		COMING_SOON,	//ComingSoon
		PLAYER_IMG,
		DISPLAY_IMG,
		OPERATION_IMG,
		ROLE_IMG,
		WAIT_IMG,

		//タイトル画面
		TITLE_LOGO,
		PLEASE_KEY,

		//リザルト
		RANKS,
		REZALT_BACK,

		SKY_DOME,
		ROLE,			//役職

		//リザルト
		RESULT,
		RANK_S,
		RANK_A,
		RANK_B,
		RANK_C,

		//BGM
		GAME_NOMAL_BGM,
		GAME_LAST_BGM,
		TITLE_BGM,
		SELECT_BGM,

		//効果音
		HIT_SND,
		PLAYER_DETH_SND,
		ENEMY_DETH_SND,
		CHICKEN_DETH_SND,
		SCENE_CHANGE_SE1,
		CHANGE_SELECT,
		CANCEL_SELECT,
		DECIDE_SELECT,
		FAZE_REZALT_SE,
		POWER_UP_SE,
		
		SKILL_CHANGE,

		ARCHER_SKILL2,	//アーチャースキル2
		BUFF_ARROW_HIT,  //アーチャースキル2がプレイヤーに当たったら


		//エフェクト
		SMOKE,
		DAMAGE_EFE,
		//敵エフェクト
		BOSS_PUNCH_EFE,
		BOSS_SHOUT_EFE,
		BOSS_SHOUT_ATK_EFE,
		STATE_DOWN_EFE,

		//プレイヤーエフェクト
		ARROW_RED,		//矢のエフェクト(赤)
		ARROW_WHITE,	//矢のエフェクト(白)

		CHARGE_AXE_HIT, //アックスの溜めスキル
		GUARD,			//ガード
		CHARGE_SKILL,	//チャージエフェクト
		HIT2,			//プレイヤーから敵への当たりエフェクト

		CARSOL_SE,
		LEVEL_UP_SE,
		EXPLAN_SWITCH,

		//説明書
		EXPLAN,
		EXPLAN2,
		EXPLAN3,
		EXPLAN4,
		EXPLAN5,
		EXPLAN6,
		EXPLAN7,

		//スキルUI関係
		SKILL_UI,
		SKILL_KNIGHT,
		SKILL_AXEMAN,
		SKILL_ARCHER,
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
	void InitGameClear(void);
	void InitExplan(void);

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

