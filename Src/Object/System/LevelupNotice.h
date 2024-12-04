#pragma once
#include <string>
#include "../../Common/ImageFader.h"

class LevelupNotice
{
public:

	//画像に関する描画の状態
	enum class STATE
	{
		NONE,
		FADE_IN,	//フェードイン
		MAINTAIN,	//維持
		FIN,		//終了
	};

	//拡大率
	static constexpr float EXPANSION_RATE = 1.0f;

	//拡大最大
	static constexpr float EXPANSION_MAX = 1.2f;

	//維持秒数
	static constexpr float MAINTAIN_SECONDS = 1.5f;

	//フォント関連
	static constexpr int FONT_SIZE = 128;
	static constexpr int FONT_THICK = 3;

	LevelupNotice();
	~LevelupNotice();

	void Init();
	void Update();
	void Draw();
	void Release();

	//変数の初期化
	void Reset();

	//状態の設定
	void SetState(const STATE state);

	//状態の受け取り
	STATE GetState() const { return state_; }

private:

	//状態
	STATE state_;

	//画像
	int img_;

	//カウント
	int cnt_;

	//大きさ
	float scl_;

	//フォント
	int font_;

	//テキスト
	std::string text_;

	//インスタンス
	ImageFader* imgFader_;

};

