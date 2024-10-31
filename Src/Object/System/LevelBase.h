#pragma once

class Fader;
class LevelupNotice;
class PowerSelect;

class LevelBase
{
public:

	//状態管理
	enum class STATE
	{
		NONE,		//なし
		NOTICE,		//通知
		SELECT		//選択
	};

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
	void NoticeUpdate(void);	//通知用の更新処理
	void PowerUpdate(void);		//強化選択の更新処理

	//描画
	virtual void Draw(void);

	//解放
	virtual void Release(void);

	//読み込み
	void Load(void);

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
	const float GetExp(void)const { return exp_; };

	//デバッグ機能
	void Debag();

protected:

	//状態
	STATE state_;

	//現在のレベル
	int nowLevel_;

	//経験値
	float exp_;

	//ゲージ(次のレベルまでの経験値量)
	float gauge_;

	//インスタンス
	Fader* fader_;
	LevelupNotice* notice_;
	PowerSelect* power_;

};