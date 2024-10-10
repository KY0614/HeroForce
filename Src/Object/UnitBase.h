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
		float pow_;			//�U����
		float duration_;	//�������ԁi�U�����ǂꂭ�炢���������L�q)
		float backlash_;	//�㌄�i�㌄���ǂꂭ�炢���������L�q)
		float cnt_;			//�J�E���^�[

	//�U�������ǂ���
		const bool IsAttack(void)const { return 0 < cnt_ && cnt_ <= duration_; };
	//�㌄���ǂ���
		const bool IsBacklash_(void) { return 0 < (cnt_ - duration_) && (cnt_ - duration_) <= backlash_; };
	//�J�E���^�[���Z�b�g
		void ResetCnt(void) { cnt_ = 0; };
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

	//Transform��������
	const Transform& GetTransform(void)const { return trans_;};

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

