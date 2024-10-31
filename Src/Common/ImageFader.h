#pragma once
#include "Fader.h"
#include "Vector2.h"

class ImageFader:public Fader
{
public:

	//•`‰æ(ˆø”‚Å‰æ‘œ‚Ìî•ñ‚ğ“Ç‚İ‚Ş)
	void Draw(int img, Vector2 pos, float scl, float angle, bool trans, bool reverse);

};

