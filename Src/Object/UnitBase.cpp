#include"../Manager/SceneManager.h"
#include "UnitBase.h"

UnitBase::UnitBase(void)
{
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
	return VECTOR();
}

const VECTOR UnitBase::GetRot(void) const
{
	return VECTOR();
}

const VECTOR UnitBase::GetScl(void) const
{
	return VECTOR();
}

const float UnitBase::GetDef(void) const
{
	return 0.0f;
}

/// <summary>
/// �A�j���[�V�����֐�
/// </summary>
/// <param name="_num">�Đ�����A�j���[�V�����i���o�[</param>
void UnitBase::Anim(const ANIM _anim)
{
	// �A�j���[�V�����Đ�
// �o�ߎ��Ԃ̎擾
	float deltaTime = 1.0f / SceneManager::DEFAULT_FPS;
	// �A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (SPEED_ANIM * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		// ���[�v�Đ�
		stepAnim_ = 0.0f;
	}
	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(mdlId_, animNum_[_anim], stepAnim_);
}

/// <summary>
/// �A�j���[�V�������Z�b�g
/// </summary>
/// <param name="_anim">�Z�b�g����A�j���[�V����</param>
/// <param name="_attachNum">�A�j���[�V�����i���o�[</param>
void UnitBase::ResetAnim(const ANIM _anim)
{
	

	//�f�^�b�`
	MV1DetachAnim(mdlId_, animNum_[anim_]);

	anim_ = _anim;
	//�A�^�b�`
	animNum_[anim_] = MV1AttachAnim(mdlId_, animNum_[anim_]);


	animTotalTime_ = MV1GetAttachAnimTotalTime(mdlId_, animNum_[anim_]);
	stepAnim_ = 0;
}



