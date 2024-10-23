#include "../Application.h"
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

	 isVibrating = false;
	 vibrationStrength = 0.1f; 
	 vibrationDuration = 20;	
	 currentVibrationTime = 0; 

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
		break;
	
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;

	case Camera::MODE::FOLLOW_SPRING:
		SetBeforeDrawFollowSpring();
		break;

	case Camera::MODE::FOLLOW_DELAY:
		SetBeforeDrawFollowDelay();
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

void Camera::SetBeforeDrawFollowDelay(void)
{
	int screenX = Application::SCREEN_SIZE_X;
	int screenY = Application::SCREEN_SIZE_Y;

	//������W
	int topL = screenX / 2 - 150;
	//����
	int underL = screenX / 2 + 150;
	//�E��
	int topR = screenY / 2 - 80;
	//�E��
	int underR = screenY / 2 + 80;

	//�ړ��͈͂̍ő��p
	DrawBox(screenX / 2 - 400, screenY / 2 - 200,
		screenX / 2 + 400, screenY / 2 + 200,
		0xFF00FFF, false);

	//�J�������ړ����n�߂��p
	DrawBox(topL, topR,
		underL, underR,
		0xFFFFFF, false);


	//�Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = followTransform_->pos;
	VECTOR zero = { 0.0f,0.0f,0.0f };


	//�Ǐ]�Ώۂ̌���
	Quaternion followRot = followTransform_->quaRot;

	//
	VECTOR pos2D = ConvWorldPosToScreenPos(followPos);
	VECTOR pos3D = ConvScreenPosToWorldPos(pos2D);

	DrawFormatString(0, 70, 0xFFFFFF, "SCREENSIZE X : %d,Y : %d", screenX, screenY);
	DrawFormatString(0, 90, 0xFFFFFF, "WP2SP : %.2f,%.2f,%.2f", pos2D.x, pos2D.y, pos2D.z);
	DrawFormatString(0, 110, 0xFFFFFF, "Player : %.2f,%.2f,%.2f", followPos.x, followPos.y, followPos.z);

	DrawCircle(pos3D.x + screenX/2, -pos3D.z , 50, 0xFF0000, false);

	auto& ins = InputManager::GetInstance();

	Vector2 mPos = ins.GetMousePos();

	//�}�E�X�̍��W�����[���h���W�ɕϊ�
	VECTOR startPos = { mPos.x,mPos.y,0.0f };
	startPos = ConvScreenPosToWorldPos(startPos);


	VECTOR endPos = { pos2D.x,pos2D.y,pos2D.z };
	endPos = ConvScreenPosToWorldPos(endPos);



	//�J�����̕������Œ肷��p
	VECTOR i = { 0.0f,0.0f,0.0f };
	Quaternion forward = Quaternion::Euler(i);

	//�Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = forward.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	//�J�����ʒu�̍X�V
	pos_ = VAdd(zero, relativeCPos);

	//�J�����ʒu���璍���_�܂ł̑��΍��W
	VECTOR relativeTPos = forward.PosAxis(RELATIVE_C2T_POS);

	//�����_�̍X�V
	targetPos_ = VAdd(pos_, relativeTPos);

	//�J�����̏����
	cameraUp_ = forward.PosAxis(rot_.GetUp());
	//	}

	DrawLine3D(startPos, followPos, 0xff9999);


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
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::FOLLOW_SPRING:
		break;
	case Camera::MODE::FOLLOW_DELAY:
		break;
	}

}

const void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

//bool Camera::IsCollisionRect(Vector2 stPos1, Vector2 edPos1, Vector2 stPos2, Vector2 edPos2)
//{
//	if (stPos1.x < edPos2.x
//		&& stPos2.x < edPos1.x
//		&& stPos1.y < edPos2.y
//		&& stPos2.y < edPos1.y)
//	{
//		return true;
//	}
//
//	return false;
//}
//
//bool Camera::IsCollisionRectCenter(Vector2 centerPos1, Vector2 size1, Vector2 centerPos2, Vector2 size2)
//{
//	// ��`1(������W�A�E����W)
//	Vector2 stPos1 = centerPos1;
//	Vector2 edPos1 = centerPos1;
//	Vector2 hSize1 = { size1.x / 2, size1.y / 2 };
//
//	stPos1.x -= hSize1.x;
//	stPos1.y -= hSize1.y;
//	edPos1.x += hSize1.x;
//	edPos1.y += hSize1.y;
//
//	// ��`�Q(������W�A�E����W)
//	Vector2 stPos2 = centerPos2;
//	Vector2 edPos2 = centerPos2;
//	Vector2 hSize2 = { size2.x / 2, size2.y / 2 };
//
//	stPos2.x -= hSize2.x;
//	stPos2.y -= hSize2.y;
//	edPos2.x += hSize2.x;
//	edPos2.y += hSize2.y;
//
//	// ��`���m�̏Փ˔���
//	// ��`�P�̍������A��`�Q�̉E���傫��
//	// ��`�Q�̍������A��`�P�̉E���傫��
//	if (IsCollisionRect(stPos1, edPos1, stPos2, edPos2))
//	{
//		return true;
//	}
//	return false;
//}

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

}

void Camera::SetShake(float intensity, float duration)
{

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
