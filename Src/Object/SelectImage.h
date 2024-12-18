#pragma once
#include"../Utility/AsoUtility.h"
#include"Common/Transform.h"
#include "../Common/Vector2.h"
#include "./UnitBase.h"

class SelectScene;
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

	//関数ポインタ使ってすっきりさせたい
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

private:
	
	//画像ハンドル
	int* imgPlayerNum_;		//人数選択画像
	int imgLeftPoint_;		//左向きの矢印画像
	int imgRightPoint_;		//右向きの矢印画像

	//メッシュの頂点データ（4つの頂点）
	VERTEX3D vertices_[4];
	VERTEX3D triangle1_[3];
	VERTEX3D triangle2_[3];
	VECTOR leftTop_;
	VECTOR leftBottom_;
	VECTOR rightTop_;
	VECTOR rightBottom_;
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

	SelectScene& selectScene_;
	std::shared_ptr<SelectPlayer> player_;

	void Load(void);	//読み込み用

	void InitVertex(void);
	VECTOR RotateVertex(VECTOR pos, VECTOR center, float angle);
};

