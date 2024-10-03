#pragma once
#include"Common/Transform.h"


//test

class UnitBase
{
public:
	//�R���X�g���N�^
	UnitBase(void);
	//�f�X�g���N�^
	~UnitBase(void);
	//���
	virtual void Destroy(void);

	//��{�����̂S�͉��z�֐�������̂ł�������I�[�o�[���C�h����悤��
	//������
	virtual void Init(void);
	//�X�V
	virtual void Update(void);
	//�`��
	virtual void Draw(void);

	//�Q�b�^�[�e��
	//�����m�F(�������Ă����true)
	const bool IsAlive(void)const {return hp_ > 0;};
	//�ʒu
	const VECTOR GetPos(void)const {return trans_.pos;};
	//�p�x
	const VECTOR GetRot(void)const {return trans_.rot;};
	//�傫��
	const VECTOR GetScl(void)const {return trans_.scl;};
	//�U����
	const float GetAtk(void)const {return atk_;};
	//�h���
	const float GetDef(void)const {return def_;};

private:

	int mdlId_;			//���f���ۑ�
	int hp_;			//�̗�
	Transform trans_;	//�ʒu���֌W
	float atk_;			//�U����
	float def_;			//�h���

};

