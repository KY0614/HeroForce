#include"../Utility/AsoUtility.h"
#include "Arrow.h"

Arrow::Arrow(void)
{
}

void Arrow::Init(const int _mdlId, const Transform& _trans)
{
	//�����̈��p��
	trans_ = _trans;
	trans_.modelId = _mdlId;

	//���X���f���̏�����
	trans_.pos = VAdd(trans_.pos, ARROW_LOCAL_POS);
	trans_.scl = { 1.0f,1.0f,1.0f };
	trans_.quaRot = Quaternion::LookRotation(trans_.rot);
	trans_.quaRotLocal =
		Quaternion::Euler(AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);

	trans_.Update();

	ChangeState(STATE::SHOT);
	SetIsAlive(true);
	atkPow_ = 0.0f;
	speed_ = 1.0f;
}

void Arrow::Update(void)
{
	switch (state_)
	{
	case Arrow::STATE::NONE:
		break;
	case Arrow::STATE::SHOT:
		//�ړ�����
		Move();
		break;
	case Arrow::STATE::DESTROY:
		break;
	case Arrow::STATE::END:
		break;
	default:
		break;
	}

	// ���f������̊�{���X�V
	trans_.Update();
}

void Arrow::Draw(void)
{
	//�V���b�g��Ԃ̂Ƃ��ɂ̂ݕ`�悷��
	if (state_ == STATE::SHOT)MV1DrawModel(trans_.modelId);
}

void Arrow::Release()
{
}

void Arrow::Destroy(void)
{
	ChangeState(STATE::DESTROY);
	SetIsAlive(false);
}

void Arrow::Move(void)
{
	// �O�������擾
	VECTOR forward = trans_.GetForward();
	//�������̎擾
	VECTOR downward = trans_.GetDown();

	// �ړ�
	//�O��
	trans_.pos =
		VAdd(trans_.pos, VScale(forward, speed_));
	//����
	trans_.pos =
		VAdd(trans_.pos, VScale(downward, GRAVITY));
}
