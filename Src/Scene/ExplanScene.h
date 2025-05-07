#pragma once
#include "SceneBase.h"

class ExplanScene : public SceneBase
{
public:

	//説明書枚数
	static constexpr int EXPLAN_CNT = 7;

	//大きさ
	static constexpr float EXP_RATE = 0.6f;
	static constexpr int POINT_SIZE = 54;

	//フォントサイズ
	static constexpr int FONT_SIZE = 36;

	//アルファ値最大
	static constexpr int ALPHA_MAX = 255;
	static constexpr int ALPHA_MIN = 130;

	//メッセージ位置
	static constexpr int MES_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES_POS_Y = Application::SCREEN_SIZE_Y - FONT_SIZE - 15;

	//コンストラクタ
	ExplanScene();

	//デストラクタ
	~ExplanScene() = default;

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:

	//画像
	int imgExp_[EXPLAN_CNT];	//説明画像
	int *imgPoint_;				//ポイント画像

	//アルファ値
	float alpha_;

	//アルファ追加値
	float alphaAdd_;

	//更新用ステップ
	float step_;

	//フォント
	int font_;

	//説明書番号
	int cntExp_;

	//スティック判定
	bool isStick_;

};

