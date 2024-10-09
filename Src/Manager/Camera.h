#pragma once
#include <DxLib.h>
#include <cmath>
#include <DirectXMath.h>
#include "../Common/Quaternion.h"

class Transform;

class Camera
{

public:

	// �J�����X�s�[�h�FNEAR
	static constexpr float SPEED = 10.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// �J�����ʒu���璍���_�܂ł̑��΍��W
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };

	// �Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 25.0f, -80.0f };

	// �Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(�΂˕t��)
	static constexpr VECTOR RELATIVE_F2C_POS_SPRING = { 0.0f, 40.0f, 150.0f };

	// �ړ����x�̍ő�l
	static constexpr float MAX_MOVE_SPEED = 3.5f;

	// ����
	static constexpr float MOVE_ACC = 0.5;

	// ����
	static constexpr float MOVE_DEC = 0.05f;


	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// ��_�J����
		FREE,			// �t���[���[�h
		FOLLOW,			// �Ǐ]���[�h
		FOLLOW_SPRING	// �΂˕t���Ǐ]���[�h
	};

	VECTOR Lerp(VECTOR start, VECTOR end, float t) {
		return VGet(
			start.x + (end.x - start.x) * t,
			start.y + (end.y - start.y) * t,
			start.z + (end.z - start.z) * t
		);
	}

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);

	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowSpring(void);

	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);

private:

	// �Ǐ]�Ώ�
	const Transform* followTransform_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����̒����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;

	// �J�����̉�]
	Quaternion rot_;

	// �J�����̑��x(�ړ���)
	VECTOR velocity_;

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	//�J�����V�F�C�N
	void Shake(void);
	
	//�J�����V�F�C�N�����邽�߂̏���
	void SetShake(float intensity,float duration);

	float shakeIntensity; // �U���̋���
	float shakeDuration;  // �U���̎�������
	float shakeTime;      // ���݂̐U������
	

	// �ړ�����
	void ProcessMove(void);

	// �ړ�
	void Move(void);

	// ����
	void Accele(float speed);

	// �����i�X�s�[�h������������j
	void Decelerate(float speed);

	float moveSpeed_;

	VECTOR moveDir;
};

