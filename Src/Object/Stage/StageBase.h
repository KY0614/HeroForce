#pragma once
#include"../Common/Transform.h"


class StageBase
{
public:
	//�R���X�g���N�^
	StageBase(void);
	//�f�X�g���N�^
	~StageBase(void);
	//���
	virtual void Destroy(void);

	//��{�����̂S�͉��z�֐�������̂ł�������I�[�o�[���C�h����悤��
	//������
	virtual void Init(void);
	//�X�V
	virtual void Update(void);
	//�`��
	virtual void Draw(void);

protected:

	int mdlId_;			//���f���ۑ�
	Transform trans_;	//�ʒu���֌W

};

