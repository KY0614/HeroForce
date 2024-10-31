#pragma once
#include "Fader.h"
#include "Vector2.h"

class ImageFader:public Fader
{
public:

	//描画(引数で画像の情報を読み込む)
	void Draw(int img, Vector2 pos, float scl, float angle, bool trans, bool reverse);

};

