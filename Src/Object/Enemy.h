#pragma once
#include "UnitBase.h"

class Enemy :
    public UnitBase
{
public:

	//�R���X�g���N�^


	//���
	void Destroy(void)override;

	//������
	void Init(void)override;
	//�X�V
	void Update(void)override;
	//�`��
	void Draw(void)override;

};

