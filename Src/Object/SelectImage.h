#pragma once
#include"../Utility/AsoUtility.h"
#include"Common/Transform.h"
#include "../Common/Vector2.h"
#include "./UnitBase.h"

class SelectScene;

class SelectImage : public UnitBase
{
public:
	//画像関連
	static constexpr float POINT_SCALE_RATE = 2.0f;	//拡大率
	static constexpr int POINT_SCALE = 52;			//矢印画像の大きさ(正方形)
	static constexpr int LEFT_POS_X = Application::SCREEN_SIZE_X / 2 - (POINT_SCALE * 5);
	static constexpr int RIGHT_POS_X = Application::SCREEN_SIZE_X / 2 + (POINT_SCALE * 5);
	static constexpr int POINT_POS_Y = Application::SCREEN_SIZE_Y / 2 ;

	//キー入力関連
	static constexpr float SELECT_TIME = 1.8f;		//キー押下経過時間
	static constexpr float INTERVAL_TIME = 0.6f;	//インターバル上限

	//矢印
	struct Point {
		Vector2 pos;
		int w, h;	//w:底辺,h:高さ	
		bool isToggle_;
		int imgHandle_;

		//初期化子
		Point() : pos(0, 0), w(0), h(0), isToggle_(false),imgHandle_(-1) {}
		Point(float x, float y, int inw, int inh, bool isT,int img) :
			pos(x, y), w(inw), h(inh), isToggle_(isT) ,imgHandle_(img) {}

		void PointDraw(void);//矢印を描画する
	};

	//コンストラクタ
	SelectImage(SelectScene& select);

	//デストラクタ
	~SelectImage(void) = default;

	//解放
	virtual void Destroy(void);

	//基本処理の４つは仮想関数化するのでしっかりオーバーライドするように
	//初期化
	virtual void Init(void)override;
	//更新
	virtual void Update(void)override;
	//描画
	virtual void Draw(void)override;

	void Update1(void);
	void UpdateOther(void);

private:
	
	//画像ハンドル
	int* imgPlayerNum_;
	int imgLeftPoint_;
	int imgRightPoint_;

	Point pointL_;
	Point pointR_;

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

	void Load(void);	//読み込み用

	//更新処理関連-----------------------------------------------

	void NumberUpdate(void);		//人数選択中の処理

	void OperationUpdate(void);		//操作方法選択中の処理(1Pのみ)

	void RoleUpdate(void);			//役職選択中の処理

	//描画処理関連-----------------------------------------------

	void NumberDraw(void);			//人数選択中の処理

	void OperationDraw(void);		//操作方法選択中の処理(1Pのみ)

	void RoleDraw(void);			//役職選択中の処理

	//-----------------------------------------------------------
};

