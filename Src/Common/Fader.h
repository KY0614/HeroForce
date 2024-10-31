#pragma once

class Fader
{

public:

	// 通知時のアルファ値
	static constexpr int NOTICE_ALPHA = 150;
	static constexpr float NOTICE_SPEED_ALPHA = 2.0f;

	// フェードが進む速さ
	static constexpr float SPEED_ALPHA = 5.0f;

	// 状態
	enum class STATE
	{
		NONE, 
		FADE_OUT,		// 徐々に暗転
		FADE_IN,		// 徐々に明転	
		FADE_NOTICE		//レベルアップ通知用
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

	// 状態(STATE)を保ったまま終了判定を行うため、
	// Update->Draw->Updateの1フレーム判定用
	bool isPreEnd_;

	// フェード処理の終了判定
	bool isEnd_;

};
