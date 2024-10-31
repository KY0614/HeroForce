#pragma once
#include "../../Common/ImageFader.h"
#include "LevelBase.h"

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

	LevelupNotice();
	~LevelupNotice();

	void Init();
	void Update();
	void Draw();

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

	//インスタンス
	LevelBase* level_;
	ImageFader* imgFader_;

};

