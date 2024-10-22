#pragma once
class LevelBase
{
public:

	//コンストラクタ
	LevelBase(void);

	//デストラクタ
	~LevelBase(void);

	//定数
	static constexpr float CONSTANT_GAGE = 100;

	//解放
	virtual void Destroy(void);

	//基本処理の４つは仮想関数化するのでしっかりオーバーライドするように
	//初期化
	virtual void Init(void);

	//更新
	virtual void Update(void);

	//描画
	virtual void Draw(void);

	//読み込み
	void Load(void);

	//経験値の設定
	void SetExp(float value);

	//ゲージの設定
	void SetGage(int level);
	
	//経験値の状態確認
	void CheckExp();

	//ゲッター
	const float GetExp(void)const { return exp_; };

	//デバッグ機能
	void Debag();

protected:

	//現在のレベル
	int nowLevel_;

	//経験値
	float exp_;

	//ゲージ(次のレベルまでの経験値量)
	float gage_;

};

