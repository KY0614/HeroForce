#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"
#include "../Common/Vector2.h"


class SelectScene :public SceneBase
{
public:

	#define DEBUG_RECT

	struct Rect {
		Vector2 pos;
		int w, h;
		Rect() : pos(0, 0), w(0), h(0) {}
		Rect(float x, float y, int inw, int inh) :
			pos(x, y), w(inw), h(inh) {}

		float Left() { return pos.x - w / 2; }
		float Top() { return pos.y - h / 2; }
		float Right() { return pos.x + w / 2; }
		float Bottom() { return pos.y + h / 2; }

		int color_;

		void Draw(unsigned int color);//自分の矩形を描画する
	};

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

	//選択するもの(人数or役職)の種類を変える
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

	//入力デバイス変更(もうちょっといい実装方法がありそう)
	void SetDevice(DEVICE device);

	//矩形と円の当たり判定(デバッグ用)
	bool IsHitRect(Rect& rc, Vector2 pos , int r);

private:
	int playerNum_[SceneManager::PLAYER_NUM];

	SELECT select_;

	DEVICE device_;

	KEY_CONFIG key;

	VECTOR pos_;

	int img_;

	Rect rc[SceneManager::PLAYER_NUM];

	int color_;
	int num;

	//読み込み用関数
	void Load(void);
};

