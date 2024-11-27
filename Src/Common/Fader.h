#pragma once

class Fader
{

public:

	// 通知時のアルファ値
	static constexpr int NOTICE_ALPHA = 150;
	static constexpr float NOTICE_SPEED_ALPHA = 2.0f;

	// フェードが進む速さ
	static constexpr float SPEED_SCENE = 5.0f;	//シーン遷移
	static constexpr float SPEED_PHASE = 2.0f;	//フェーズ遷移
	

	// 状態
	enum class STATE
	{
		NONE,			//何もしていない
		FADE_OUT,		// 徐々に暗転
		FADE_IN,		// 徐々に明転	
		FADE_NOTICE	,	//レベルアップ通知用
		FADE_KEEP,		//現在の状態を保つ
	};

	// 状態の取得
	STATE GetState(void) const;

	// フェード処理が終了しているか
	bool IsEnd(void) const;

	// 指定フェードを開始する
	void SetFade(STATE state);


	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);

protected:

	// 状態
	STATE state_;

	// 透明度
	float alpha_;

	//フェード速度
	float speed_;

	// 状態(STATE)を保ったまま終了判定を行うため、
	// Update->Draw->Updateの1フレーム判定用
	bool isPreEnd_;

	// フェード処理の終了判定
	bool isEnd_;

};
