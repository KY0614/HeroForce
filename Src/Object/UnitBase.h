#pragma once
#include"Common/Transform.h"


//test

class UnitBase
{
public:

	//�U���֌W
	struct ATK
	{
		VECTOR pos_;		//�ʒu
		float atk_;			//�U����
		float duration_;	//��������
		float Backlash_;	//�㌄

	//�U���͎擾
		const float GetAtk(void)const { return atk_; };
	//�ʒu�擾
		const VECTOR GetPos(void)const { return pos_; };
	};


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
	//�h���
	const float GetDef(void)const {return def_;};

protected:

	int mdlId_;			//���f���ۑ�
	int hp_;			//�̗�
	Transform trans_;	//�ʒu���֌W
	float def_;			//�h���

};

