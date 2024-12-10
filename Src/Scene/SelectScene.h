#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"
#include "../Common/Vector2.h"

class SkyDome;
class StageManager;
class SelectPlayer;
class SelectImage;

class SelectScene :public SceneBase
{
public:

	#define DEBUG_RECT
	#define DEBUG_TEST

	//三角形用の定数
	static constexpr int TRI_SCALE = 150;	//大きさ
	static constexpr int TRI_POS_Y = 450;	//Y座標

	//四角形の大きさ
	static constexpr int RECT_SCALE = 300;

	//図形同士の間隔
	static constexpr int PRI_SPACE = 100;


	//キー押下経過時間
	static constexpr float SELECT_TIME = 1.0f;

	//インターバル上限
	static constexpr float INTERVAL_TIME = 0.5f;

	static constexpr int PLAYER_NUM = SceneManager::PLAYER_NUM;
	static constexpr int ROLE_NUM = SceneManager::PLAYER_NUM;

	//カメラ関連
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };		//カメラの座標
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//カメラの注視点座標

	static constexpr float CHARACTER_SCALE = 0.5f;

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

		//左の三点のx,y座標
		float LeftX_L() { return pos.x + h / 2;}
		float LeftY_L() { return pos.y + w / 2;}
		float TopX_L()  { return pos.x - h / 2;}
		float TopY_L()  { return pos.y; }
		float RightX_L(){ return pos.x + h / 2;}
		float RightY_L(){ return pos.y - w / 2;}

		//右の三点のx,y座標
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

		UP_TRG,
		DOWN_TRG,
		LEFT_TRG,
		RIGHT_TRG,

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

	void InitModel(void);

	//更新処理関連-----------------------------------------------
	
	void NumberUpdate(void);		//人数選択中の処理
	
	void OperationUpdate(void);		//操作方法選択中の処理(1Pのみ)

	void RoleUpdate(void);			//役職選択中の処理

	//描画処理関連-----------------------------------------------

	void NumberDraw(void);			//人数選択中の処理

	void OperationDraw(void);		//操作方法選択中の処理(1Pのみ)

	void RoleDraw(void);			//役職選択中の処理

	//-----------------------------------------------------------


	//選択するもの(人数or役職)の種類を変える
	void ChangeSelect(SELECT select);

	//キー入力とコントローラ入力を共通化
	void KeyConfigSetting(void);

	//今使用している入力デバイスを取得する
	SceneManager::CNTL GetDevice(void);

	//入力デバイス変更(もうちょっといい実装方法がありそう)
	void ChangeDevice(SceneManager::CNTL device);	

	//キー入力とパッド入力の制御
	void ControllDevice(void);

	//ゲッター	----------------------------------------------------------

	KEY_CONFIG GetKeyConfig(void);	//入力キーを取得

	//デバッグ関連--------------------------------------------------------

	//デバッグ描画
	void DrawDebug(void);

	//矩形と円の当たり判定(デバッグ用)
	bool IsHitRect(Rect& rc, Vector2 pos, int r);

	//--------------------------------------------------------------------

private:
	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	//プレイヤー
	std::unique_ptr<SelectPlayer>players_[SceneManager::PLAYER_NUM];

	// 画像
	std::unique_ptr<SelectImage>images_[SceneManager::PLAYER_NUM];

	//背景のステージ
	StageManager* stage_;

	Transform trans_;
	Transform tests_[SceneManager::PLAYER_NUM];

	//選択中の種類
	SELECT select_;

	//デバイス
	SceneManager::CNTL device_;

	//キーコンフィグ
	KEY_CONFIG key_;

	//デバッグ関連-------------------------------

	//四角形
	Rect rc;

	//三角形
	Tri triL;
	Tri triR;
	
	//プレイヤー人数
	int playerNum_;
	int player_[SceneManager::PLAYER_NUM];
	
	bool isPad_;	//1Pの入力タイプ
	SceneManager::CNTL selectedCntl_;
	
	int role_;	//職種

	//キーを何秒押しているか
	float keyPressTime_;

	bool press_;

	//人数を一定間隔で加算していくためのインターバル用時間(加算して次加算するまでの間)
	float interval_;

};

