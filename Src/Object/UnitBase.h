#pragma once
#include"Common/Transform.h"
#include<unordered_map>


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
		const bool IsBacklash_(void)const { return 0 < (cnt_ - duration_) && (cnt_ - duration_) <= backlash_; };
	//���[�V�������I�����Ă��邩
		const bool IsFinishMotion(void)const { return cnt_ > (duration_ + backlash_); };
	//�J�E���^�[���Z�b�g
		void ResetCnt(void) { cnt_ = 0; };
	};

	enum class ANIM
	{
		IDLE,		//�ҋ@
		WALK,		//����
		RUN,		//����
		SKILL_1,	//�X�L���@
		SKILL_2,	//�X�L���A
		DODGE,		//���
		DAMAGE,		//��_��
		DEATH,		//���S
		ENTRY,		//�o��
		UNIQUE_1,	//�ŗL�̂�@
		UNIQUE_2,	//�ŗL�̂�A
	};

	//�萔
	// �A�j���[�V�����̍Đ����x
	static constexpr float SPEED_ANIM = 20.0f;


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

	const Transform* GetTransformEntity(void)const;

	//�ʒu
	virtual const VECTOR GetPos(void)const;
	//�p�x
	virtual const VECTOR GetRot(void)const;
	//�傫��
	virtual const VECTOR GetScl(void)const;
	//�h���
	virtual const float GetDef(void)const;

	//�A�j���[�V�����֐�
	void Anim(const int _num);
	//�A�j���[�V�������Z�b�g
	void ResetAnim(const ANIM _anim);

protected:

	int mdlId_;			//���f���ۑ�
	int hp_;			//�̗�
	Transform trans_;	//�ʒu���֌W
	float def_;			//�h���

	//�A�j���֌W
	ANIM anim_;
	std::unordered_map<ANIM, int> animNum_;	//�A�j���[�V�����i���o�[�i�[�z��B
	int animTotalTime_;		//�A�j���[�V�����̑��Đ�����
	int stepAnim_;			//�A�j���[�V�����̍Đ�����
};

