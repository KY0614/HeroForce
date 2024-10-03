#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Transform.h"
#include "Camera.h"

Camera::Camera(void)
{
	mode_ = MODE::NONE;
	pos_ = { 0.0f, 0.0f, 0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	rot_ = Quaternion::Identity();
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// �J�����̏����ݒ�
	SetDefault();
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
	
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;

	case Camera::MODE::FOLLOW_SPRING:
		SetBeforeDrawFollowSpring();
		break;
	}

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// �������Ȃ�
}

void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();

	// �ړ�
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }
	
	// ��]���Ɨʂ����߂�
	const float ROT_POW = 1.0f;
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x = -1.0f; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x = 1.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y = -1.0f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y = 1.0f;}


	// �J�������W�𒆐S�Ƃ��āA�����_����]������
	if (!AsoUtility::EqualsVZero(axisDeg))
	{
		// ����̉�]�ʂ�����
		Quaternion rotPow;
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.z), AsoUtility::AXIS_Z));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.x), AsoUtility::AXIS_X));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.y), AsoUtility::AXIS_Y));

		// �J�����̉�]�̍���̉�]�ʂ�������i�����j
		rot_ = rot_.Mult(rotPow);

		// �����_�̑��΍��W����]������
		VECTOR rotLocalPos = rot_.PosAxis(RELATIVE_C2T_POS);

		// �����_�X�V
		targetPos_ = VAdd(pos_, rotLocalPos);

		// �J�����̏�����X�V
		cameraUp_ = rot_.GetUp();
	}

	// �ړ�����
	if (!AsoUtility::EqualsVZero(moveDir))
	{
		// �ړ� = ���W + �ړ���
		// �ړ��� = ���� * �X�s�[�h 

		// ���͂��ꂽ�������J�����̉�]�����g���āA
		// �J�����̐i�s�����ɕϊ�����
		VECTOR direction = rot_.PosAxis(moveDir);
		// �ړ���
		VECTOR movePow = VScale(direction, SPEED);
		// �ړ�����
		pos_ = VAdd(pos_, movePow);
		targetPos_ = VAdd(targetPos_, movePow);
	}
}

void Camera::SetBeforeDrawFollow(void)
{
	//// �Ǐ]�Ώۂ̈ʒu
	//VECTOR followPos = followTransform_->pos;
	//
	//// �Ǐ]�Ώۂ̌���
	//Quaternion followRot = followTransform_->quaRot;
	//
	//// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	//VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_FOLLOW);
	//
	//// �J�����ʒu�̍X�V
	//pos_ = VAdd(followPos, relativeCPos);
	//
	//// �J�����ʒu���璍���_�܂ł̑��΍��W
	//VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);
	//
	//// �����_�̍X�V
	//targetPos_ = VAdd(pos_, relativeTPos);
	//
	//// �J�����̏����
	//cameraUp_ = followRot.PosAxis(rot_.GetUp());
}

void Camera::SetBeforeDrawFollowSpring(void)
{
	// �΂˒萔(�΂˂̋���)
	float POW_SPRING = 24.0f;
	float dampening = 2.0f * sqrt(POW_SPRING);

	// �f���^�^�C��
	float delta = SceneManager::GetInstance().GetDeltaTime();


	//VECTOR

	// ���z�ʒu
	//VECTOR idealPos = VAdd(shipPos, relative);
	
	// ���ۂƗ��z�̍�
	//VECTOR diff = VSub(pos_, idealPos);
	
	// �� = -�o�l�̋��� �~ �o�l�̐L�� - ��R �~ �J�����̑��x
	//VECTOR force = VScale(diff, -POW_SPRING);
	//force = VSub(force, VScale(velocity_, dampening));
	
	// ���x�̍X�V
	//velocity_ = VAdd(velocity_, VScale(force, delta));
}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

void Camera::ChangeMode(MODE mode)
{

	// �J�����̏����ݒ�
	SetDefault();

	// �J�������[�h�̕ύX
	mode_ = mode;

	// �ύX���̏���������
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	}

}

void Camera::SetFollow(const Transform* follow)
{
}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	// �����_
	targetPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	// �J�����̏����
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// �J������X���ɌX���Ă��邪�A
	// ���̌X������Ԃ��p�x�[���A�X�������Ƃ���
	rot_ = Quaternion::Identity();

}
