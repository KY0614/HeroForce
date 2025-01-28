#include"../Application.h"
#include "../Lib/nlohmann/json.hpp"
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"

UnitBase::UnitBase(void)
{
	trans_.modelId = -1;
	hp_ = -1;
	trans_.pos = { 0.0f,0.0f,0.0f };
	trans_.scl = { 0.0f,0.0f,0.0f };
	trans_.rot = { 0.0f,0.0f,0.0f };	

	for (auto& tran_ : transArray_)
	{
		tran_.pos = { 0.0f,0.0f,0.0f };
		tran_.scl = { 0.0f,0.0f,0.0f };
		tran_.rot = { 0.0f,0.0f,0.0f };
	}

	anim_ = ANIM::NONE;
	atcAnim_ = -1;
	atkPow_ = -1.0f;
	damage_ = -1;
	animTotalTime_ = -1;
	stepAnim_ = -1.0f;
	speedAnim_ = 1.0f;

	animArray_ = -1;
	animArrayTotalTime_ = -1;
	stepAnimArray_ = -1.0f;
	speedAnimArray_ = -1.0f;

	prePos_ = AsoUtility::VECTOR_ZERO;
	def_ = -1.0f;
	radius_ = -1.0f;
	hpMax_ = -1;

	defAtk_ = -1.0f;
	defDef_ = -1.0f;
	defSpeed_ = -1.0f;
	defHp_ = -1;

	atkUpPercent_ = -1.0f;
	defUpPercent_= -1.0f;
	speedUpPercent_ = -1.0f;
}

UnitBase::~UnitBase(void)
{
}

void UnitBase::Destroy(void)
{
}

void UnitBase::Init(void)
{
}

void UnitBase::Update(void)
{
}

void UnitBase::Draw(void)
{
}

const Transform& UnitBase::GetTransform(void) const
{
	return trans_;
}

const inline Transform* UnitBase::GetTransformEntity(void) const
{
	auto ret = new Transform();

	ret->pos = trans_.pos;
	ret->scl = trans_.scl;
	ret->rot = trans_.rot;
	ret->quaRot = trans_.quaRot;
	return ret;
	return ret;
}

const VECTOR UnitBase::GetPos(void) const
{
	return trans_.pos;
}

const VECTOR UnitBase::GetRot(void) const
{
	return trans_.rot;
}

const VECTOR UnitBase::GetScl(void) const
{
	return trans_.scl;
}

const float UnitBase::GetDef(void) const
{
	return def_;
}

const UnitBase::ATK UnitBase::GetAtk(void) const
{
	return atk_;
}

const float UnitBase::GetRadius(void) const
{
	return radius_;
}

const VECTOR UnitBase::GetPrePos() const
{
	return prePos_;
}

/// <summary>
/// �A�j���[�V�����֐�
/// </summary>
/// <param name="_num">�Đ�����A�j���[�V�����i���o�[</param>
void UnitBase::Anim(void)
{
	// �A�j���[�V�����Đ�
	// �o�ߎ��Ԃ̎擾
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	// �A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (speedAnim_ * deltaTime);
	if (stepAnim_ > animTotalTime_ )
	{
		//�A�j���[�V�����I���������i�p����ōs�������߂Ă����j
		FinishAnim();
	}
	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(trans_.modelId, atcAnim_, stepAnim_);
}


/// <summary>
/// �A�j���[�V�����Z�b�g
/// </summary>
/// <param name="_anim">�A�j���̎w��</param>
/// <param name="_speed">�A�j���[�V�����X�s�[�h</param>
void UnitBase::ResetAnim(const ANIM _anim, const float _speed)
{
	if (anim_ == _anim)return;

	//�A�j���[�V�����X�s�[�h�̕ύX
	speedAnim_ = _speed;

	//�f�^�b�`
	//����atcAnim�̏�����
	MV1DetachAnim(trans_.modelId, atcAnim_);

	anim_ = _anim;
	//�A�^�b�`
	//����atcAnim�̑��
	atcAnim_ = MV1AttachAnim(trans_.modelId, animNum_[anim_]);

	animTotalTime_ = MV1GetAttachAnimTotalTime(trans_.modelId, atcAnim_);
	stepAnim_ = 0.0f;

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(trans_.modelId, atcAnim_, stepAnim_);
}

void UnitBase::AnimArray(void)
{
	// �A�j���[�V�����Đ�
	// �o�ߎ��Ԃ̎擾
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	stepAnimArray_ += (speedAnimArray_ * deltaTime);
	if (stepAnimArray_ > animArrayTotalTime_)
	{
		//�A�j���[�V�����I���������i�p����ōs�������߂Ă����j
		FinishAnim();
	}

	for (auto& tran : transArray_)
	{
		// �Đ�����A�j���[�V�������Ԃ̐ݒ�
		MV1SetAttachAnimTime(tran.modelId, animArray_, stepAnimArray_);
	}
}

void UnitBase::SetIsHit(const bool _flag)
{
	atk_.isHit_ = _flag;
}

void UnitBase::SetDamage(const int attackerPower, const int skillPower)
{
	//�^����_���[�W�𑝂₷
	damage_ += attackerPower * skillPower / defDef_;
}

void UnitBase::SubHp()
{
	//�_���[�W��0���傫�������ׂ�
	if(0 < damage_)
	{
		damage_--;

		hp_--;

		if (hp_ < 0) { hp_ = 0; }
	}
}

void UnitBase::ResetAnimArray(const ANIM _anim, const float _speed)
{
	if (anim_ == _anim)return;

	speedAnimArray_ = _speed;

	for (auto& tran : transArray_)
	{
		//�f�^�b�`
		//����atcAnim�̏�����
		MV1DetachAnim(tran.modelId, animArray_);

		anim_ = _anim;

		//�A�^�b�`
		//����atcAnim�̑��
		animArray_ = MV1AttachAnim(tran.modelId, animNum_[anim_]);

		animArrayTotalTime_ = MV1GetAttachAnimTotalTime(tran.modelId, animArray_);
		stepAnimArray_ = 0.0f;

		// �Đ�����A�j���[�V�������Ԃ̐ݒ�
		MV1SetAttachAnimTime(tran.modelId, animArray_, stepAnimArray_);
	}
}

float UnitBase::GetAnimArrayTime(void)
{
	for (auto& tran : transArray_)
	{
		float ret = MV1GetAttachAnimTime(tran.modelId, animArray_);
		return ret;
	}
}
//���W�̐ݒ�
void UnitBase::SetPos(const VECTOR pos)
{
	trans_.pos = pos;
}

void UnitBase::SetPrePos(void)
{
	trans_.pos = prePos_;
}

//�U���͂̋���
void UnitBase::SetAttack(const float percent)
{
	atkUpPercent_ += percent;			//�������㏸
	atkPow_ = defAtk_ * atkUpPercent_;	//�U���͂��㏸
}
 
//�h��͂̋���
void UnitBase::SetDefense(const float percent)
{
	defUpPercent_ += percent;
	def_ = defDef_ * defUpPercent_;
}

//�ړ����x
void UnitBase::SetSpeed(const float percent)
{
	speedUpPercent_ += percent;
	moveSpeed_ = defSpeed_ * speedUpPercent_;
}

//�̗͋���
void UnitBase::SetHpMax(const float hp)
{
	hpMax_ += hp;
}

void UnitBase::ParamLoad(CharacterParamData::UNIT_TYPE type)
{
	auto& data = CharacterParamData::GetInstance().GetParamData(type);

	//�f�t�H���g�̃X�e�[�^�X�ݒ�
	defAtk_ = data.atk_;
	defDef_ = data.def_;
	defSpeed_ = data.speed_;
	defHp_ = data.hp_;
	radius_ = data.radius_;

	//�ω��p�̐ݒ�
	atkPow_ = defAtk_;
	def_ = defDef_;
	moveSpeed_ = defSpeed_;
	hp_ = defHp_;

	//HP�̍ő�l�̐ݒ�
	hpMax_ = defHp_;

	//�����p�[�Z���g�̏�����
	atkUpPercent_ = DEFAULT_PERCENT;
	defUpPercent_ = DEFAULT_PERCENT;
	speedUpPercent_ = DEFAULT_PERCENT;

}

//�A�j���I�����̓���
void UnitBase::FinishAnim(void)
{
	//���[�v�Đ�
	stepAnim_ = 0.0f;
	stepAnimArray_ = 0.0f;
}

void UnitBase::CntUp(float& _count)
{
	// �o�ߎ��Ԃ̎擾
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	_count += deltaTime;
}

void UnitBase::CntDown(float& _count)
{
	// �o�ߎ��Ԃ̎擾
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	_count -= deltaTime;
}