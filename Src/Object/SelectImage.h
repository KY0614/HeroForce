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
	static constexpr float POINT_SCALE = 52.0f;			//矢印画像の大きさ
	static constexpr float IMAGE_SCALE = 50.0f;			//画像の大きさ

	//頂点関連（４点)--------------------------------------------------------------

	//人数選択などの表示に使うメッシュ
	static constexpr int VERTEX_NUM = 4;			//頂点数
	static constexpr float VERTEX_ROTSPEED = 1.0f;	//頂点を回転させる速度

	static constexpr float VERTEX_LEFT_X = -25.0f;	//画像(頂点)左のX座標
	static constexpr float VERTEX_RIGHT_X = 25.0f;	//画像(頂点)右のX座標
	
	static constexpr float VERTEX_TOP_Y = 150.0f;	//画像上のY座標	
	static constexpr float VERTEX_UNDER_Y = 90.0f;	//画像下のY座標 

	static constexpr float VERTEX_Z = -400.0f;		//画像上のZ座標
	static constexpr float VERTEX_UNDER_Z = -392.0f;//画像下のZ座標

	//矢印(左のほうはそのままで右の場合はLEFTとRIGHTを入れ替えてマイナス値をかける)
	static constexpr float POINT_LEFT_X = -50.0f;	//画像(頂点)左のX座標
	static constexpr float POINT_RIGHT_X = -28.0f;	//画像(頂点)右のX座標
	static constexpr float POINT_TOP_Y = 130.0f;	//画像(頂点)下のY座標
	static constexpr float POINT_UNDER_Y = 110.0f;	//画像(頂点)上のY座標
													
	static constexpr float POINT_TOP_Z = VERTEX_Z;	//画像上のZ座標
	static constexpr float POINT_UNDER_Z = -402.0f;	//画像下のZ座標

	static constexpr float ROLE_MESH_LEFT_X = -55.0f;
	static constexpr float ROLE_MESH_RIGHT_X = 15.0f;

	static constexpr float ROLE_MESH_TOP_Z = 170.0f;
	static constexpr float ROLE_MESH_UNDER_Z = 100.0f;

	static constexpr int BLEND_PARAM = 128;			//ブレンドモードの強さ

	//キー入力関連
	static constexpr float SELECT_TIME = 1.0f;		//キー押下経過時間
	static constexpr float INTERVAL_TIME = 0.6f;	//インターバル上限

	//構造体-----------------------------------------------------------------------
	
	//メッシュ
	struct Mesh {
		VERTEX3D vertex_[VERTEX_NUM];	//頂点情報

		//初期化
		Mesh() : vertex_(){}

		/// <summary>
		/// メッシュを描画する
		/// </summary>
		/// <param name="handle">画像ハンドル</param>
		void DrawTwoMesh(int handle);		
	};

	//矢印
	struct Point {
		bool isToggle_;	//オン、オフの切り替え用
		Mesh mesh_;		//矢印用のメッシュ

		//初期化
		Point() : isToggle_(false),mesh_() {}
		Point(bool isT,Mesh& mesh) :
			 isToggle_(isT) , mesh_(mesh)  {}
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

	//役職選択の際に表示するメッシュの座標へ移動させる
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

	//選んでいる役職を取得
	int GetRole(void) { return role_; };	

	/// <summary>
	/// メッシュの頂点情報を取得
	/// </summary>
	/// <param name="i">vertex配列の指定</param>
	/// <returns>指定したvertexの頂点情報</returns>
	VERTEX3D GetMeshVertex(int i){ return mesh_.vertex_[i]; };
	VERTEX3D GetReadyMeshVertex(int i) { return readyMesh_.vertex_[i]; };
	VERTEX3D GetPointLMeshVertex(int i){ return pointL_.mesh_.vertex_[i]; };
	VERTEX3D GetPointRMeshVertex(int i){ return pointR_.mesh_.vertex_[i]; };

	//準備完了かどうか得る
	bool GetReady(void) { return isReady_; };

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
	Mesh mesh_;			//人数選択と操作選択の時のメッシュ
	Mesh readyMesh_;	//準備完了の時のメッシュ
	
	//画像ハンドル
	int* imgPlayerNum_;		//人数選択画像
	int* imgDisplayNum_;	//ディスプレイ数選択画像
	int* imgLeftPoint_;		//左向きの矢印画像
	int* imgRightPoint_;	//右向きの矢印画像
	int* imgReady_;			//準備完了画像
	int* imgRoleNum_;		//役職の画像
	int* imgDeviceNum_;		//役職の画像
	int* imgComingSoon_;	//ComingSoonの画像

	//メッシュの頂点座標用（4つの頂点）
	VECTOR leftTop_;		//左上
	VECTOR leftBottom_;		//左下
	VECTOR rightTop_;		//右上	
	VECTOR rightBottom_;	//右下
	float angle_;		

	//矢印の構造体
	Point pointL_;		//左
	Point pointR_;		//右

	//ディスプレイ数
	int displayNum_;

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
	SelectScene& selectScene_;				//セレクトシーン
	std::shared_ptr<SelectPlayer> player_;	//セレクトシーン用のプレイヤー

	//カメラ用のターゲット座標
	VECTOR target_[SceneManager::PLAYER_NUM];

	//関数-------------------------------------------------------------------------------------

	//読み込み用
	void Load(void);	

	//頂点座標初期化
	void InitVertex(void);

	//頂点座標回転用
	VECTOR RotateVertex(VECTOR pos, VECTOR center, float angle);

	//状態遷移
	void ChangeStateDisplay(void);
	void ChangeStateNumber(void);
	void ChangeStateOperation(void);
	void ChangeStateRole(void);

	//更新処理関連-----------------------------------------------

	void DisplayUpdate(void);		//ディスプレイ数選択中の処理

	void NumberUpdate(void);		//人数選択中の処理

	void OperationUpdate(void);		//操作方法選択中の処理(1Pのみ)

	void RoleUpdate(void);			//役職選択中の処理

	//描画処理関連-----------------------------------------------

	void DisplayDraw(void);			//ディスプレイ数選択中の処理

	void NumberDraw(void);			//人数選択中の処理

	void OperationDraw(void);		//操作方法選択中の処理(1Pのみ)

	void RoleDraw(void);			//役職選択中の処理

	void PointsDraw(void);			//矢印（２つとも）描画

	//-----------------------------------------------------------
};