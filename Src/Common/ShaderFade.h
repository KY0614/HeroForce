#pragma once
#include "Fader.h"
#include "Vector2.h"

class ShaderFade :public Fader
{
public:

	//�`��(�����ŉ摜�̏���ǂݍ���)
	void Init()override;
	void Draw()override;

private:

	//�t�F�[�h�p�摜
	int imgFade_;

};

