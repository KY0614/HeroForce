#include "../Utility/AsoUtility.h"
#include "../Application.h"
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
	//�J�����̏����ݒ�
	SetDefault();

	SetShake(0.0f, 0.0f);

	moveSpeed_ = 0.0f;
}

void Camera::Update(void)
{
	Shake();
}

void Camera::SetBeforeDraw(void)
{

	//�N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::NONE:
		SetBeforeDrawFollow();
		break;

	case Camera::MODE::FIXED_POINT:
		//SetBeforeDrawFixedPoint();
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

	//�J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//�������Ȃ�
}

void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();

	ProcessMove();

	Decelerate(MOVE_DEC);

	Move();

	//N�L�[�������ƃJ�������V�F�C�N����悤��
	//������
	if (ins.IsTrgDown(KEY_INPUT_N))
	{
		SetShake(10.5f, 5.0f);
		isVibrating = true;
	}

	Shake();

	DrawFormatString(0, 0, 0xffffff, "moveSpeed_ : %f", moveSpeed_);
}

void Camera::SetBeforeDrawFollow(void)
{
	//�Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = followTransform_->pos;

	//�Ǐ]�Ώۂ̌���
	Quaternion followRot = followTransform_->quaRot;

	//�Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	//�J�����ʒu�̍X�V
	pos_ = VAdd(followPos, relativeCPos);

	//�J�����ʒu���璍���_�܂ł̑��΍��W
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);

	//�����_�̍X�V
	targetPos_ = VAdd(pos_, relativeTPos);

	//�J�����̏����
	cameraUp_ = followRot.PosAxis(rot_.GetUp());
}

void Camera::SetBeforeDrawFollowSpring(void)
{
	//�΂˒萔(�΂˂̋���)
	float POW_SPRING = 24.0f;

	//�΂˒萔�i�΂˂̒�R�j
	float dampening = 2.0f * sqrt(POW_SPRING);

	//�f���^�^�C��
	float delta = SceneManager::GetInstance().GetDeltaTime();

	//3D��������l�p
	//delta = 1.0f / 60.0f;

	//�Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = followTransform_->pos;

	//�Ǐ]�Ώۂ̌���
	Quaternion followRot = followTransform_->quaRot;

	//�Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_SPRING);

	//���z�ʒu
	VECTOR idealPos = VAdd(followPos, relativeCPos);

	//���ۂƗ��z�̍�
	VECTOR diff = VSub(pos_, idealPos);

	//�� = -�o�l�̋��� �~ �o�l�̐L�� - ��R �~ �J�����̑��x
	VECTOR force = VScale(diff, -POW_SPRING);
	force = VSub(force, VScale(velocity_, dampening));

	//���x�̍X�V
	velocity_ = VAdd(velocity_, VScale(force, delta));

	//�J�����ʒu�̍X�V
	pos_ = VAdd(pos_, VScale(velocity_, delta));

	//�J�����ʒu���璍���_�܂ł̑��΍��W
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);

	//�����_�̍X�V
	targetPos_ = VAdd(pos_, relativeTPos);

	//�J�����̏����
	cameraUp_ = followRot.PosAxis(rot_.GetUp());
}

void Camera::Draw(void)
{
	DrawBox(Application::SCREEN_SIZE_X / 2 - 50, Application::SCREEN_SIZE_Y / 2 - 50,
		Application::SCREEN_SIZE_X / 2 + 50, Application::SCREEN_SIZE_Y / 2 + 50,
		0xFFFFFF,true);
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

	//�J�����̏����ݒ�
	SetDefault();

	//�J�������[�h�̕ύX
	mode_ = mode;

	//�ύX���̏���������
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
	followTransform_ = follow;
}

void Camera::SetDefault(void)
{

	//�J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	//�����_
	targetPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	//�J�����̏����
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	//�J������X���ɌX���Ă��邪�A
	//���̌X������Ԃ��p�x�[���A�X�������Ƃ���
	rot_ = Quaternion::Identity();

	velocity_ = AsoUtility::VECTOR_ZERO;

}

void Camera::Shake(void)
{
	if (isVibrating) {
		if (currentVibrationTime < vibrationDuration) {
			// �U���̌v�Z
			pos_.x += (rand() % 200 - 100) / 1000.0f * vibrationStrength;
			pos_.y += (rand() % 200 - 100) / 1000.0f * vibrationStrength;
			currentVibrationTime++;
		}
		else {
			// �U���I��
			isVibrating = false;
			currentVibrationTime = 0;
		}
	}

	// �J�����̈ʒu��ݒ�
	SetCameraPositionAndTargetAndUpVec(pos_, targetPos_, cameraUp_);;
}

void Camera::SetShake(float intensity, float duration)
{
	vibrationStrength = intensity;
	vibrationDuration = duration;
}

void Camera::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	//�ړ�
	//moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; Accele(MOVE_ACC); }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; Accele(MOVE_ACC); }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; Accele(MOVE_ACC); }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; Accele(MOVE_ACC); }

	//��]���Ɨʂ����߂�
	const float ROT_POW = 1.0f;
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x = -1.0f; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x = 1.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y = -1.0f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y = 1.0f; }


	//�J�������W�𒆐S�Ƃ��āA�����_����]������
	if (!AsoUtility::EqualsVZero(axisDeg))
	{
		//����̉�]�ʂ�����
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

		//�J�����̉�]�̍���̉�]�ʂ�������i�����j
		rot_ = rot_.Mult(rotPow);

		//�����_�̑��΍��W����]������
		VECTOR rotLocalPos = rot_.PosAxis(RELATIVE_C2T_POS);

		//�����_�X�V
		targetPos_ = VAdd(pos_, rotLocalPos);

		//�J�����̏�����X�V
		cameraUp_ = rot_.GetUp();
	}
}

void Camera::Move(void)
{
	//�ړ�����
	if (!AsoUtility::EqualsVZero(moveDir))
	{
		//�ړ� = ���W + �ړ���
		//�ړ��� = ���� * �X�s�[�h 

		//���͂��ꂽ�������J�����̉�]�����g���āA
		//�J�����̐i�s�����ɕϊ�����
		VECTOR direction = rot_.PosAxis(moveDir);

		//�ړ���
		VECTOR movePow = VScale(direction, moveSpeed_);

		//�ړ�����
		pos_ = VAdd(pos_, movePow);

		targetPos_ = VAdd(targetPos_, movePow);
	}
}

void Camera::Accele(float speed)
{
	moveSpeed_ += speed;

	//���x����(�E����)
	if (moveSpeed_ > MAX_MOVE_SPEED)
	{
		moveSpeed_ = MAX_MOVE_SPEED;
	}

	//���x����(������)
	if (moveSpeed_ < -MAX_MOVE_SPEED)
	{
		moveSpeed_ = -MAX_MOVE_SPEED;
	}
}

void Camera::Decelerate(float speed)
{
	//�E�����̈ړ�������������
	if (moveSpeed_ > 0.0f)
	{
		moveSpeed_ -= speed;
		if (moveSpeed_ < 0.0f)
		{
			moveSpeed_ = 0.0f;
		}
	}

	//�������̈ړ�������������
	if (moveSpeed_ < 0.0f)
	{
		moveSpeed_ += speed;
		if (moveSpeed_ > 0.0f)
		{
			moveSpeed_ = 0.0f;
		}
	}
}
