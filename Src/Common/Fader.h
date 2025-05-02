#pragma once

class Fader
{

public:

	//透過最大値
	static constexpr int ALPHA_MAX = 255;

	// 画面を少し暗くするようのアルファ値
	static constexpr int LITTLE_ALPHA = 150;

	// 画面を少し暗くする時のスピード
	static constexpr float LITTLE_FADE_OUT_SPEED = 2.0f;

	// フェードが進む速さ
	static constexpr float SPEED_SCENE = 5.0f;	//シーン遷移
	static constexpr float SPEED_PHASE = 2.0f;	//フェーズ遷移
	static constexpr float SPEED_ALPHA = 3.0f;
	

	// 状態
	enum class STATE
	{
		NONE,			//何もしていない
		FADE_OUT,		// 徐々に暗転
		FADE_IN,		// 徐々に明転	
		FADE_KEEP,		//現在の状態を保つ
		SET_FADE_OUT	// アルファ値を決めて暗転
	};

	// 状態の取得
	STATE GetState(void) const;

	// フェード処理が終了しているか
	bool IsEnd(void) const;

	// 指定フェードを開始する
	void SetFade(STATE state);

	// どれくらい暗くするかのアルファ値を決める
	void SetAlpha(float alpha);

	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);

protected:

	// 状態
	STATE state_;

	// 暗転・明転用透明度
	float alpha_;

	//フェード速度
	float speed_;

	// 透明度の指定値用
	float alphaMax_;

	// 状態(STATE)を保ったまま終了判定を行うため、
	// Update->Draw->Updateの1フレーム判定用
	bool isPreEnd_;

	// フェード処理の終了判定
	bool isEnd_;

private:
	
	//マスク画像領域
	int tmpScreen_;

	//円が他のくりぬき処理
	void CircleMask();

};