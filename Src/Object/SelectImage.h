#pragma once
#include <functional>
#include"../Utility/AsoUtility.h"
#include"Common/Transform.h"
#include "../Common/Vector2.h"
#include "./UnitBase.h"
#include "../Scene/SelectScene.h"

//class SelectScene;
class SelectPlayer;

class SelectImage : public UnitBase
{
public:
	//画像関連
	static constexpr float POINT_SCALE_RATE = 2.0f;	//拡大率
	static constexpr int POINT_SCALE = 52;			//矢印画像の大きさ(正方形)
	static constexpr int LEFT_POS_X = Application::SCREEN_SIZE_X / 2 - (POINT_SCALE * 5);
	static constexpr int RIGHT_POS_X = Application::SCREEN_SIZE_X / 2 + (POINT_SCALE * 5);
	static constexpr int POINT_POS_Y = Application::SCREEN_SIZE_Y / 2 ;

	//頂点関連（４点)
	static constexpr int VERTEX_NUM = 4;			//頂点数
	static constexpr float VERTEX_ROTSPEED = 1.0f;	//頂点を回転させる速度

	static constexpr float VERTEX_LEFT_X = -50.0f;	//
	static constexpr float VERTEX_RIGHT_X = 50.0f;	//

	static constexpr float VERTEX_UNDER_Y = 70.0f;
	static constexpr float VERTEX_TOP_Y = 170.0f;

	static constexpr float VERTEX_Z = -350.0f;

	//キー入力関連
	static constexpr float SELECT_TIME = 1.0f;		//キー押下経過時間
	static constexpr float INTERVAL_TIME = 0.6f;	//インターバル上限

	//構造体-----------------------------------------------------------------------
	
	//メッシュ
	struct Mesh {
		VERTEX3D vertex_[4];	//頂点情報
		VERTEX3D testVertex_[4];	//頂点情報
		int imgHandle_;			//画像ハンドル

		void DrawTwoMesh(int handle);
		void DrawTwoMeshTest(int handle);
	};

	//矢印
	struct Point {
		Vector2 pos;	//座標
		int w, h;		//w:底辺,h:高さ	
		bool isToggle_;	//オン、オフの切り替え用
		int imgHandle_;	//画像ハンドル

		//初期化
		Point() : pos(0, 0), w(0), h(0), isToggle_(false),imgHandle_(-1) {}
		Point(float x, float y, int inw, int inh, bool isT,int img) :
			pos(x, y), w(inw), h(inh), isToggle_(isT) ,imgHandle_(img) {}

		void PointDraw(void);//矢印を描画する
	};

	//------------------------------------------------------------------------------

	//コンストラクタ
	SelectImage(SelectScene& select, std::shared_ptr<SelectPlayer> player);

	//デストラクタ
	~SelectImage(void) = default;

	//解放
	virtual void Destroy(void);

	//初期化
	virtual void Init(void)override;
	//更新
	virtual void Update(void)override;
	//描画
	virtual void Draw(void)override;

	/// <summary>
	/// 選択しているオブジェクトを変更する
	/// </summary>
	/// <param name="input">入力デバイス</param>
	/// <param name="i">入力デバイスに応じたプレイヤーナンバー</param>
	void ChangeObject(SelectScene::Device& input,int i);

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="_state">遷移する状態</param>
	void ChangeSelect(const SelectScene::SELECT _state);

	//ゲッター　---------------------------------------------------

	int GetRole(void) { return role_; };	//選んでいる役職を取得

	/// <summary>
	/// メッシュの頂点情報を取得
	/// </summary>
	/// <param name="i">vertex配列の指定</param>
	/// <returns>指定したvertexの頂点情報</returns>
	VERTEX3D GetMeshVertex(int i);

	// セッター　--------------------------------------------------

	/// <summary>
	/// メッシュ座標を設定する
	/// </summary>
	/// <param name="pos">設定する座標</param>
	/// <param name="i">設定するvertex配列の指定</param>
	void SetMeshPos(VECTOR pos, int i);

private:

	//状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;
	//状態管理(状態遷移時初期処理)
	std::map<SelectScene::SELECT, std::function<void(void)>> stateChanges_;

	//選択中の種類
	SelectScene::SELECT state_;

	//メッシュ
	Mesh mesh_[SceneManager::PLAYER_NUM];
	Mesh testMesh_;
	
	//画像ハンドル
	int* imgPlayerNum_;		//人数選択画像
	int imgLeftPoint_;		//左向きの矢印画像
	int imgRightPoint_;		//右向きの矢印画像

	//メッシュの頂点座標用（4つの頂点）
	VECTOR leftTop_;		//左上
	VECTOR leftBottom_;		//左下
	VECTOR rightTop_;		//右上	
	VECTOR rightBottom_;	//右下
	float angle_;		

	//矢印の構造体
	Point pointL_;		//左
	Point pointR_;		//右

	//プレイヤー人数
	int playerNum_;

	//1Pの入力タイプ
	bool isPad_;	

	//職種
	int role_;

	//キーを何秒押しているか
	float keyPressTime_;
	
	//キーの判定を１回だけ取得する用
	bool press_;

	//人数を一定間隔で加算していくためのインターバル用時間(加算して次加算するまでの間)
	float interval_;

	//インスタンス
	SelectScene& selectScene_;
	std::shared_ptr<SelectPlayer> player_;

	VECTOR target_[SceneManager::PLAYER_NUM];

	//関数-------------------------------------------------------------------------------------

	void Load(void);	//読み込み用

	void InitVertex(void);
	VECTOR RotateVertex(VECTOR pos, VECTOR center, float angle);

	//状態遷移
	void ChangeStateNumber(void);
	void ChangeStateOperation(void);
	void ChangeStateRole(void);

	//更新処理関連-----------------------------------------------

	void NumberUpdate(void);		//人数選択中の処理

	void OperationUpdate(void);		//操作方法選択中の処理(1Pのみ)

	void RoleUpdate(void);			//役職選択中の処理

	//描画処理関連-----------------------------------------------

	void NumberDraw(void);			//人数選択中の処理

	void OperationDraw(void);		//操作方法選択中の処理(1Pのみ)

	void RoleDraw(void);			//役職選択中の処理

	void PointsDraw(void);			//矢印（２つとも）描画

	//-----------------------------------------------------------
};

