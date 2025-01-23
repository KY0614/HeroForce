#pragma once
#include <functional>
#include <memory>
#include <map>
#include "../../Common/Transform.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../UnitBase.h"

class ChickenBase : public UnitBase
{
public:	
	
	//���
	enum class STATE
	{
		NONE,	//�Ȃ�
		ALIVE,	//����
		DAMAGE, //�_���[�W
		DEATH,	//���j
		END		//�I��
	};

	enum class ALIVE_MOVE
	{
		IDLE,		//�����~�܂�
		ROTATION,	//���̏�𓦂����
		CALL,		//�������Ă�
		MAX		
	};

	//�L�����N�^�[�T�C�Y
	static constexpr VECTOR SCALE = { 1.3f,1.3f, 1.3f };

	//�p�����[�^�[�֘A
	static constexpr int DEFAULT_LIFE = 300;
	static constexpr int DEFAULT_ATK = 0;
	static constexpr int DEFAULT_DEF = 50;
	static constexpr float DEFAULT_SPEED = 3.0f;

	//�A�j���[�V�������x
	static constexpr float DEFAULT_SPEED_ANIM = 50.0f;
	
	//�A�j���[�V�����ԍ�
	static constexpr int ANIM_IDLE = 1;
	static constexpr int ANIM_WALK = 10;
	static constexpr int ANIM_DAMAGE = 9;
	static constexpr int ANIM_DEATH =11;
	static constexpr int ANIM_CALL = 6;

	//������Ԏ��̏�Ԏ��
	static constexpr int ALIVE_MOVE_MAX = static_cast<int>(ALIVE_MOVE::MAX);

	//�Փ˔���p���̔��a
	static constexpr float RADIUS = 50.0f;

	//�t�F�[�h����
	static constexpr float TIME_FADE = 4.0f;

	//�t�F�[�h�J�n�E�I���F
	static constexpr COLOR_F FADE_C_FROM = { 1.0f, 1.0f, 1.0f, 1.0f };
	static constexpr COLOR_F FADE_C_TO = { 0.8f, 0.1f, 0.1f, 0.0f };

	ChickenBase();
	~ChickenBase();

	void Create(VECTOR& pos);	//�����ʒu�ƃ^�[�Q�b�g�̃g�����X�t�H�[�������炤
	void Update(void)override;
	void Draw(void)override;
	
	//���f���ݒ�
	void ModelSet();

	//�p�����[�^�[�̐ݒ�
	virtual void SetParam();

	//�A�j���[�V�����ԍ��̏�����
	virtual void InitAnimNum(void);

	//�^�[�Q�b�g�̍��W�ݒ�
	void SetTarget(const VECTOR pos);

	//�_���[�W��^����
	void SetDamage(const int damage);

	//��Ԃ𓾂�
	const STATE GetState(void) const{ return state_; }

protected:

	//�ړ��X�s�[�h
	float moveSpeed_;

	//�t�F�[�h�p�X�e�b�v
	float fadeStep_;

	//�^�[�Q�b�g�p���
	VECTOR targetPos_;

	//���
	STATE state_;

	//�������̏��
	ALIVE_MOVE aliveState_;
	
	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	std::map<ALIVE_MOVE, std::function<void(void)>> aliveChanges_;

	// ��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;

	// ��ԊǗ�(�`��)
	std::function<void(void)> stateDraw_;

	// ��������ԊǗ�
	std::function<void(void)> stateAliveUpdate_;

	//��ԕύX
	void ChangeState(STATE state);
	void ChangeStateNone();
	void ChangeStateAlive();
	void ChangeStateDamage();
	void ChangeStateDeath();

	void ChangeAliveState(ALIVE_MOVE state);
	void ChangeAliveIdle();
	void ChangeAliveWalk();
	void ChangeAliveCall();

	//��ԕʍX�V
	void UpdateNone();
	void UpdateAlive();
	void UpdateDamage();
	void UpdateDeath();

	//��ԕʕ`��
	void DrawNone();
	void DrawAlive();
	void DrawDamage();
	void DrawDeath();

	//������Ԃł̏�ԕʍX�V
	void AliveIdle();
	void AliveRotation();
	void AliveCall();

	//�^�[�Q�b�g�����鏈��
	void LookTarget();

	//�A�j���[�V�����̏I������
	void FinishAnim() override;

	//�f�o�b�O
	void DebagUpdate();
	void DebagDraw();
};

