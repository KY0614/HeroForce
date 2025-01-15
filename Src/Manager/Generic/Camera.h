#pragma once
#include <DxLib.h>
#include <cmath>
#include <DirectXMath.h>
#include "../Common/Quaternion.h"

class Transform;

class Camera
{

public:
	//�J�����̕`���(Near,Far)�֘A�̒萔------------------------------------------------------
	
	static constexpr float SPEED = 10.0f;			//�J�����X�s�[�h�FNEAR

	
	static constexpr float CAMERA_NEAR = 40.0f;		//�J�����N���b�v�FNEAR

	
	static constexpr float CAMERA_FAR = 15000.0f;	//�J�����N���b�v�FNEAR

	//�J�������W�֘A�̒萔---------------------------------------------------------------------
	
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };			//�J�����̏������W

	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -400.0f, 500.0f };			//�J�����ʒu���璍���_�܂ł̑��΍��W

	
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 500.0f, -500.0f };	//�Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(���S�Ǐ])

	//static constexpr VECTOR RELATIVE_F2C_POS_SPRING = { 0.0f, 40.0f, 150.0f };	//�Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(�΂˕t��)
	
	//�J�����ړ��֘A�̒萔---------------------------------------------------------------------
	
	static constexpr float MAX_MOVE_SPEED = 5.0f;	//�ړ����x�̍ő�l

	static constexpr float MOVE_ACC = 0.5f;			//����

	static constexpr float MOVE_DEC = 0.2f;			//����

	//�J�����h�炵�֘A�̒萔--------------------------------------------------------------------

	static constexpr float TIME_SHAKE = 0.5f;		//����

	static constexpr float WIDTH_SHAKE = 5.0f;		//��

	static constexpr float SPEED_SHAKE = 40.0f;		//�X�s�[�h

	//�J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	//��_�J����
		FREE,			//�t���[���[�h
		FOLLOW,			//�Ǐ]���[�h
		FOLLOW_SPRING,	//�΂˕t���Ǐ]���[�h
		SHAKE			//�J�����h�炵
	};

	//�R���X�g���N�^
	Camera(void);

	//�f�X�g���N�^
	~Camera(void);

	//����������
	void Init(void);

	//�X�V����
	void Update(void);

	//�J�����̕`�惂�[�h�֘A------------------
	void SetBeforeDraw(void);

	void SetBeforeDrawFixedPoint(void);		//��_�J����
	void SetBeforeDrawFree(void);			//�t���[�J����
	void SetBeforeDrawFollow(void);			//�Ǐ]�J����
	void SetBeforeDrawFollowSpring(void);	//�΂˒Ǐ]�J����
	void SetBeforeDrawShake(void);			//�J�����V�F�C�N

	//----------------------------------------

	// �`�揈��
	void Draw(void);

	//�������
	void Release(void);

	//���W�擾
	VECTOR GetPos(void) const;

	//�J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	//�Ǐ]�Ώۂ̐ݒ�
	const void SetFollow(const Transform* follow);

	void SetPos(const VECTOR& pos,const VECTOR& target);

private:

	//�Ǐ]�Ώ�
	const Transform* followTransform_;

	//�J�������[�h
	MODE mode_;			
	MODE currentMode_;	//Shake���[�h�ɑJ�ڂ���ۂɌ��݂̃��[�h�ۑ����邽�߂̕ϐ�

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

	//�ړ���
	float moveSpeed_;

	//����
	VECTOR moveDir;

	//��ʗh�炵�p
	float stepShake_;

	VECTOR defaultPos_;

	VECTOR shakeDir_;
	
	//�J�����������ʒu�ɖ߂�
	void SetDefault(void);

	//�J�����V�F�C�N
	void Shake(void);

	//�J�����V�F�C�N�����邽�߂̏���
	void SetShake(float intensity, float duration);

	//�ړ�����
	void ProcessMove(void);

	//�ړ�
	void Move(void);

	//����
	void Accele(float speed);

	//�����i�X�s�[�h������������j
	void Decelerate(float speed);

};
