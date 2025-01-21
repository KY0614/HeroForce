#pragma once
#include<DxLib.h>
#include"../Manager/Generic/SceneManager.h"

class DataBank
{
	//cppやSceneをまたぎ扱う情報等を保存する
public:

	struct PLAYER_INFO
	{
		SceneManager::CNTL cntrol_;
		SceneManager::PLAY_MODE mode_;
		SceneManager::ROLE role_;
	};

	enum class INFO
	{
		NONE,
		CNTL,
		MODE,
		ROLE,
		USER_NUM,
		DHISPLAY_NUM,
	};

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static DataBank& GetInstance(void);

	void Init(void);
	
	//データ入力
	void Input(const SceneManager::CNTL _cntl,const int _playerNum);		//コントロール方法の入力
	void Input(const SceneManager::PLAY_MODE _mode,const int _playerNum);	//ユーザーかCPU科の入力
	void Input(const SceneManager::ROLE _role, const int _playerNum);		//役職の入力
	void Input(const INFO _info, const int _num);							//システムに関する数値入力（画面数・ユーザー数用）
	
	//データ出力
	const PLAYER_INFO Output(const int _playerNum);	//プレイヤーに関する情報を返す
	const int Output(const INFO _info);				//システムに関する数値出力（画面数・ユーザー数用）

	// リソースの破棄
	void Destroy(void);

private:
	//各キャラクターの操作タイプを保存
	PLAYER_INFO playerInfo_[SceneManager::PLAYER_NUM];

	//ユーザーの人数(のちのちウィンドウの数としても利用する)
	int userNum_;

	//ディスプレイの数
	int displayNum_;

	// 静的インスタンス
	static DataBank* instance_;

	//コンストラクタ及びデストラクタ
	DataBank() = default;
	~DataBank() = default;
};

