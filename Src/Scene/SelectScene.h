#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"
#include "../Common/Vector2.h"


class SelectScene :public SceneBase
{
public:

	#define DEBUG_RECT

	//四角形を描画するために必要なもの
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

	//三角形
	struct Tri {
		Vector2 pos;
		int w, h;	//w:底辺,h:高さ	
		bool isToggle_;

		//初期化子
		Tri() : pos(0, 0), w(0), h(0) ,isToggle_(false) {}
		Tri(float x, float y, int inw, int inh,bool isT) :
			pos(x, y), w(inw), h(inh), isToggle_(isT) {}

		//?の三点のx,y座標
		float LeftX_L() { return pos.x + h / 2;}
		float LeftY_L() { return pos.y + w / 2;}
		float TopX_L()  { return pos.x - h / 2;}
		float TopY_L()  { return pos.y; }
		float RightX_L(){ return pos.x + h / 2;}
		float RightY_L(){ return pos.y - w / 2;}

		//?の三点のx,y座標
		float LeftX_R() { return pos.x - h / 2; }
		float LeftY_R() { return pos.y + w / 2; }
		float TopX_R() { return pos.x + h / 2; }
		float TopY_R() { return pos.y; }
		float RightX_R() { return pos.x - h / 2; }
		float RightY_R() { return pos.y - w / 2; }

		int color_;

		void LeftDraw(unsigned int color);//三角形を描画する
		void RightDraw(unsigned int color);//三角形を描画する
	};

	//選択している種類
	enum class SELECT 
	{
		NUMBER,		//人数
		OPERATION,	//1Pをキーボード操作にするかどうか
		ROLE		//役職
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

	//更新処理関連-----------------------------------------------
	
	void NumberUpdate(void);		//人数選択中の処理
	
	void OperationUpdate(void);		//操作方法選択中の処理(1Pのみ)

	void RoleUpdate(void);			//役職選択中の処理

	//-----------------------------------------------------------

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
	SceneManager::CNTL GetDevice(void);

	//プレイヤーが参加するときに使用したデバイス
	bool GetJoinDevice(void);

	//入力デバイス変更(もうちょっといい実装方法がありそう)
	void ChangeDevice(SceneManager::CNTL device);	

	//キー入力とパッド入力の制御
	void ControllKey(void);

	//デバッグ関連--------------------------------------------------------

	//デバッグ描画
	void DrawDebug(void);

	//矩形と円の当たり判定(デバッグ用)
	bool IsHitRect(Rect& rc, Vector2 pos, int r);

	//--------------------------------------------------------------------

private:
	int playerNum_;

	SELECT select_;

	SceneManager::CNTL device_;

	KEY_CONFIG key;

	//座標
	Vector2 kPos_;	//キーカーソル用座標
	Vector2 cPos_;	//コントローラーカーソル用座標

	//デバッグ関連-------------------------------

	//四角形
	Rect rc;

	//三角形
	Tri triL;
	Tri triR;

	int color_;
	int num;
	int opr;
	int role;
	//-----------------------------------------

	//読み込み用関数
	void Load(void);
};

