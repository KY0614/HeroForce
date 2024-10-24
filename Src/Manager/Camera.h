#pragma once
#include <DxLib.h>
#include <cmath>
#include <DirectXMath.h>
#include "../Common/Quaternion.h"

class Transform;

class Camera
{

public:

	//�J�����X�s�[�h�FNEAR
	static constexpr float SPEED = 10.0f;

	//�J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	//�J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	//�J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	//�J�����ʒu���璍���_�܂ł̑��΍��W
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -400.0f, 500.0f };
//	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };

	//�Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 500.0f, -500.0f };
//	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 25.0f, -80.0f };

	//�Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(�΂˕t��)
	static constexpr VECTOR RELATIVE_F2C_POS_SPRING = { 0.0f, 40.0f, 150.0f };

	//�ړ����x�̍ő�l
	static constexpr float MAX_MOVE_SPEED = 5.0f;

	//����
	static constexpr float MOVE_ACC = 0.5f;

	//����
	static constexpr float MOVE_DEC = 0.2f;

	//�ړ��͈͂̉�p(�ŏ�)
	static constexpr float MOVE_SIZE_X = 300.0f;
	static constexpr float MOVE_SIZE_Y = 320.0f - 80.0f;

	//�J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	//��_�J����
		FREE,			//�t���[���[�h
		FOLLOW,			//�Ǐ]���[�h
		FOLLOW_SPRING,	//�΂˕t���Ǐ]���[�h
		FOLLOW_DELAY
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);

	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowSpring(void);
	void SetBeforeDrawFollowDelay(void);

	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;

	//�J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	//�Ǐ]�Ώۂ̐ݒ�
	const void SetFollow(const Transform* follow);

	bool IsCheckFollow(VECTOR centerPos1, VECTOR size1, VECTOR centerPos2, VECTOR size2);

	//// �Փ˔���
	//bool IsCollisionRect(Vector2 stPos1, Vector2 edPos1, Vector2 stPos2, Vector2 edPos2);

	////2D�̋�`���m�̓����蔻��
	//bool IsCollisionRectCenter(Vector2 centerPos1, Vector2 size1, Vector2 centerPos2, Vector2 size2);

private:

	//�Ǐ]�Ώ�
	const Transform* followTransform_;

	//�J�������[�h
	MODE mode_;

	//�J�����̈ʒu
	VECTOR pos_;

	//�J�����̒����_
	VECTOR targetPos_;

	//�J�����̏����
	VECTOR cameraUp_;

	//�J�����̉�]
	Quaternion rot_;

	//�J�����̑��x(�ړ���)
	VECTOR velocity_;

	//�J�����������ʒu�ɖ߂�
	void SetDefault(void);

	//�J�����V�F�C�N
	void Shake(void);
	
	//�J�����V�F�C�N�����邽�߂̏���
	void SetShake(float intensity,float duration);

	bool isVibrating;
	float vibrationStrength;	//�U���̋���
	int vibrationDuration;		//�U���̎�������
	int currentVibrationTime;   //���݂̐U������
	

	//�ړ�����
	void ProcessMove(void);

	//�ړ�
	void Move(void);

	//����
	void Accele(float speed);

	//�����i�X�s�[�h������������j
	void Decelerate(float speed);

	// �ړ���
	float moveSpeed_;

	//����
	VECTOR moveDir;
};

