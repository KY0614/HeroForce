#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"


class SelectScene :public SceneBase
{
public:

	//選択している種類
	enum class SELECT 
	{
		NUMBER,		//人数
		ROLE		//役職
	};

	//入力デバイス
	enum class DEVICE
	{
		NONE,
		KEY,
		PAD
	};

	// キーコンフィグ
	enum class  KEY_CONFIG
	{
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		DECIDE
	};

	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	//人数選択中の処理
	void NumberUpdate(void);

	//役職選択中の処理
	void RoleUpdate(void);

	//デバッグ描画
	void DrawDebug(void);

	//選択するものの種類を変える
	void ChangeSelect(SELECT select);

	void ProcessSelect(void);

	//キー入力とコントローラ入力を共通化
	void KeyConfigSetting(void);

	//カーソル移動処理
	void ProcessCursor(void);

	//カーソルの移動量
	void MoveCursor(float pow ,VECTOR dir);

	//今使用している入力デバイスを取得する
	DEVICE GetDevice(void);

	//プレイヤーが参加するときに使用したデバイス
	DEVICE GetJoinDevice(void);

	void SetDevice(DEVICE device);

private:
	int playerNum_[SceneManager::PLAYER_NUM];

	SELECT select_;

	DEVICE device_;

	KEY_CONFIG key;

	VECTOR pos_;

	int img_;

	//読み込み用関数
	void Load(void);
};

