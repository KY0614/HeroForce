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
	//#define DRAW_DEBUG

	//画像関連
	static constexpr float POINT_SCALE_RATE = 2.0f;	//拡大率
	static constexpr int POINT_SCALE = 52;			//矢印画像の大きさ(正方形)
	static constexpr int LEFT_POS_X = Application::SCREEN_SIZE_X / 2 - (POINT_SCALE * 5);
	static constexpr int RIGHT_POS_X = Application::SCREEN_SIZE_X / 2 + (POINT_SCALE * 5);
	static constexpr int POINT_POS_Y = Application::SCREEN_SIZE_Y / 2 ;

	//頂点関連（４点)--------------------------------------------------------------

	//人数選択などの表示に着かくメッシュ
	static constexpr int VERTEX_NUM = 4;			//頂点数
	static constexpr float VERTEX_ROTSPEED = 1.0f;	//頂点を回転させる速度

	static constexpr float VERTEX_LEFT_X = -50.0f;	//画像(頂点)左のX座標
	static constexpr float VERTEX_RIGHT_X = 50.0f;	//画像(頂点)右のX座標

	static constexpr float VERTEX_UNDER_Y = 70.0f;	//画像下のY座標
	static constexpr float VERTEX_TOP_Y = 170.0f;	//画像上のY座標

	static constexpr float VERTEX_Z = -350.0f;		//画像上のZ座標
	static constexpr float VERTEX_UNDER_Z = -338.0f;//画像下のZ座標

	static constexpr float ROLE_LEFT_X = -90.0f;
	static constexpr float ROLE_RIGHT_X = 0.0f;

	static constexpr float ROLE_UNDER_Y = 50.0f;
	static constexpr float ROLE_TOP_Y = 190.0f;

	static constexpr float ROLE_VERTEX_Z = -338.0f;		//頂点Z座標

	//矢印(左のほうはそのままで右の場合はLEFTとRIGHTを入れ替えてマイナス値をかける)
	static constexpr float POINT_LEFT_X = -110.0f;	//画像(頂点)左のX座標
	static constexpr float POINT_RIGHT_X = -58.0f;	//画像(頂点)右のX座標
	static constexpr float POINT_TOP_Y = 146.0f;	//画像(頂点)下のY座標
	static constexpr float POINT_UNDER_X = 94.0f;	//画像(頂点)上のY座標
													
	static constexpr float POINT_TOP_Z = VERTEX_Z;	//画像上のY座標
	static constexpr float POINT_UNDER_Z = -356.0f;	//画像下のY座標

	static constexpr int BLEND_PARAM = 128;			//ブレンドモードの強さ

	//キー入力関連
	static constexpr float SELECT_TIME = 1.0f;		//キー押下経過時間
	static constexpr float INTERVAL_TIME = 0.6f;	//インターバル上限

	//構造体-----------------------------------------------------------------------
	
	//メッシュ
	struct Mesh {
		VERTEX3D vertex_[VERTEX_NUM];	//頂点情報

		Mesh() : vertex_(){}

		/// <summary>
		/// メッシュを描画する
		/// </summary>
		/// <param name="handle">画像ハンドル</param>
		void DrawTwoMesh(int handle);		
	};

	//矢印
	struct Point {
		int w, h;		//w:底辺,h:高さ	
		bool isToggle_;	//オン、オフの切り替え用
		Mesh mesh_;

		//初期化
		Point() :  w(0), h(0), isToggle_(false),mesh_() {}
		Point(int inw, int inh, bool isT,Mesh& mesh) :
			 w(inw), h(inh), isToggle_(isT) , mesh_(mesh)  {}

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

	void MoveVertexPos(void);

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
	VERTEX3D GetMeshVertex(int i){ return mesh_.vertex_[i]; };
	VERTEX3D GetReadyMeshVertex(int i) { return readyMesh_.vertex_[i]; };
	VERTEX3D GetPointLMeshVertex(int i){ return pointL_.mesh_.vertex_[i]; };
	VERTEX3D GetPointRMeshVertex(int i){ return pointR_.mesh_.vertex_[i]; };

	bool GetReady(void) { return isReady_; };

	VECTOR GetVerPos(void) { return mesh_.vertex_[0].pos; }

	// セッター　--------------------------------------------------

	/// <summary>
	/// メッシュ座標を設定する
	/// </summary>
	/// <param name="pos">設定する座標</param>
	/// <param name="i">設定するvertex配列の指定</param>
	void RotMeshPos(VECTOR pos, int i) { mesh_.vertex_[i].pos = pos; }
	void RotReadyMeshPos(VECTOR pos, int i) { readyMesh_.vertex_[i].pos = pos; }
	void RotPointLMeshPos(VECTOR pos, int i) { pointL_.mesh_.vertex_[i].pos = pos; }
	void RotPointRMeshPos(VECTOR pos, int i) { pointR_.mesh_.vertex_[i].pos = pos; }

private:

	//状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;
	//状態管理(状態遷移時初期処理)
	std::map<SelectScene::SELECT, std::function<void(void)>> stateChanges_;

	//選択中の種類
	SelectScene::SELECT state_;

	//メッシュ
	Mesh mesh_;	//人数選択と操作選択の時のメッシュ
	Mesh readyMesh_;	//準備完了の時のメッシュ
	
	//画像ハンドル
	int* imgPlayerNum_;		//人数選択画像
	int* imgLeftPoint_;		//左向きの矢印画像
	int* imgRightPoint_;	//右向きの矢印画像
	int* imgReady_;	//右向きの矢印画像

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

	//準備オッケーかどうか
	bool isReady_;

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

	float lerpTime_;

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