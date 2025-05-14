#pragma once
#include<DxLib.h>
#include"../Manager/Generic/SceneManager.h"

class DataBank
{
	//cppやSceneをまたぎ扱う情報等を保存する
public:
	/// <summary>
	/// プレイヤー情報
	/// </summary>
	struct PLAYER_INFO
	{
		SceneManager::CNTL cntrol_;
		SceneManager::PLAY_MODE mode_;
		SceneManager::ROLE role_;
	};

	/// <summary>
	/// 各種情報
	/// </summary>
	enum class INFO
	{
		NONE,
		CNTL,
		MODE,
		ROLE,
		USER_NUM,
		DHISPLAY_NUM,
		FAZE_DUNK_ENEMY,
		GAME_DUNK_ENEMY,
		ALIVE_CHICKEN
	};

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static DataBank& GetInstance(void);

	void Init(void);

#pragma region 入力
	/// <summary>
	/// コントロール方法の入力
	/// </summary>
	/// <param name="_cntl">コントロールの方法(SceneManager::CNTL)を参照</param>
	/// <param name="_playerNum">プレイヤー番号（何Pか)</param>
	void Input(const SceneManager::CNTL _cntl, const int _playerNum);


	/// <summary>
	/// ユーザーかCPUかの入力
	/// </summary>
	/// <param name="_mode">ユーザーかCPUか(SceneManager::PLAY_MODE)を参照</param>
	/// <param name="_playerNum">プレイヤー番号</param>
	void Input(const SceneManager::PLAY_MODE _mode, const int _playerNum);

	/// <summary>
	/// 役職の入力
	/// </summary>
	/// <param name="_role">役職(SceneManager::ROLE)参照</param>
	/// <param name="_playerNum">プレイヤー番号</param>
	void Input(const SceneManager::ROLE _role, const int _playerNum);

	/// <summary>
	/// システムに関する数値入力 ※ユーザー数・ディスプレイ数以外で使用しないこと！！
	/// </summary>
	/// <param name="_info">設定対象
	/// (INFOのUSERとDISPLAYのみ)</param>
	/// <param name="_num">数値</param>
	void Input(const INFO _info, const int _num);
#pragma endregion

#pragma region 出力
	/// <summary>
	/// プレイヤー情報の出力
	/// </summary>
	/// <param name="_playerNum">プレイヤー番号</param>
	/// <returns>プレイヤーに関する情報</returns>
	const PLAYER_INFO Output(const int _playerNum);

	/// <summary>
	/// システムに関する数値出力（画面数・ユーザー数用）
	/// </summary>
	/// <param name="_info">対象
	/// (INFOのUSERとDISPLAYのみ)</param>
	/// <returns>システムに関する情報</returns>
	const int Output(const INFO _info);
#pragma endregion

	// リソースの破棄
	void Destroy(void);

private:
	//各キャラクターの操作タイプを保存
	PLAYER_INFO playerInfo_[SceneManager::PLAYER_NUM];

	//ユーザーの人数(のちのちウィンドウの数としても利用する)
	int userNum_;

	//ディスプレイの数
	int displayNum_;

	//リザルト情報
	int fazeDunk_;		//フェーズごとの倒した敵
	int gameDunk_;		//倒した敵総数
	int aliveChikenNum_;	//ニワトリ生存数

	// 静的インスタンス
	static DataBank* instance_;

	//コンストラクタ及びデストラクタ
	DataBank() = default;
	~DataBank() = default;
};


