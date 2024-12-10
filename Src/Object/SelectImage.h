#pragma once
#include"../Utility/AsoUtility.h"
#include"Common/Transform.h"
#include "./UnitBase.h"

class SelectImage : public UnitBase
{
public:
	//�R���X�g���N�^
	SelectImage();

	//�f�X�g���N�^
	~SelectImage() = default;

	//���
	virtual void Destroy(void);

	//��{�����̂S�͉��z�֐�������̂ł�������I�[�o�[���C�h����悤��
	//������
	virtual void Init(void)override;
	//�X�V
	virtual void Update(void)override;
	//�`��
	virtual void Draw(void)override;

private:
	
	int playerNumImg_;
	int leftPointImg_;
	int rightPointImg_;



	void Load(void);
};

