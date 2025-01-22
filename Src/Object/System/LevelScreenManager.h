#pragma once

class Fader;
class LevelupNotice;
class LevelupSelect;

class LevelScreenManager
{
public:

	//状態管理
	enum class STATE
	{
		NONE,		//なし
		NOTICE,		//通知
		SELECT,		//選択
		FIN
	};	

	//強化要素の種類
	enum class TYPE
	{
		ATTACK,		//攻撃力の上昇
		DEFENSE,	//防御力の上昇
		SPEED,		//移動速度の上昇
		LIFE,		//最大体力の上昇
	  /*TRAP_FLOOR,
		TRAP_CANNON,
		TRAP_POW_UP_F,
		TRAP_POW_UP_C,*/
		MAX
	};
	
	//ゲージ最大値
	static constexpr float CONSTANT_GAGE = 100.0f;

	//アルファ値最大
	static constexpr float ALPHA_MAX = 150.0f;

	//透過スピード
	static constexpr float ALPHA_SPEED = 3.0f;

	//種類最大
	static constexpr int TYPE_MAX = static_cast<int>(TYPE::MAX);

	//コンストラクタ
	LevelScreenManager(void);

	//デストラクタ
	~LevelScreenManager(void);



	//基本処理の４つは仮想関数化するのでしっかりオーバーライドするように
	//初期化
	virtual void Init(void);

	//更新
	virtual void Update(void);
	void NoticeUpdate(void);	//通知用の更新処理
	void PowerUpdate(void);		//強化選択の更新処理

	//描画
	virtual void Draw(void);

	//解放	
	virtual void Release(void);

	//読み込み
	void Load(void);

	//初期化処理
	void Reset();

	//経験値の設定
	void SetExp(const float value);

	//ゲージの設定
	void SetGage(const int level);

	//ステートの設定
	void SetState(const STATE state);
	
	//経験値の状態確認
	void CheckExp();

	//暗転
	void FaderDraw();

	//ゲッター
	inline float GetExp(void)const { return exp_; };
	inline TYPE GetType(void)const { return type_; };
	inline STATE GetState(void)const { return state_; };

	//デバッグ機能
	void Debag();

protected:

	//状態
	STATE state_;

	//種類
	TYPE type_;

	//現在のレベル
	int nowLevel_;

	//経験値
	float exp_;

	//ゲージ(次のレベルまでの経験値量)
	float gauge_;

	//アルファ値
	float alpha_;

	//インスタンス
	Fader* fader_;
	LevelupNotice* notice_;
	LevelupSelect* select_;

};