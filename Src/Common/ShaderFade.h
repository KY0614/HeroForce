#pragma once
#include "Fader.h"
#include "Vector2.h"

class ShaderFade :public Fader
{
public:

	ShaderFade();
	~ShaderFade();

	//描画(引数で画像の情報を読み込む)
	void Init()override;
	void Draw()override;

private:

	//フェード用画像
	int imgFade_;

};

