#include "ChickenBase.h"

ChickenBase::ChickenBase()
{
	moveSpeed_ = -1.0f;
	fadeStep_ = -1.0f;
	state_ = STATE::NONE;
	aliveState_ = ALIVE_MOVE::MAX;
	targetPos_ = AsoUtility::VECTOR_ZERO;

	// ��ԊǗ�
	stateChanges_.emplace(STATE::NONE, std::bind(&ChickenBase::ChangeStateNone, this));
	stateChanges_.emplace(STATE::ALIVE , std::bind(&ChickenBase::ChangeStateAlive, this));
	stateChanges_.emplace(STATE::DAMAGE , std::bind(&ChickenBase::ChangeStateDamage, this));
	stateChanges_.emplace(STATE::DEATH , std::bind(&ChickenBase::ChangeStateDeath, this));

	// ������ԊǗ�
	aliveChanges_.emplace(ALIVE_MOVE::IDLE, std::bind(&ChickenBase::ChangeAliveIdle, this));
	aliveChanges_.emplace(ALIVE_MOVE::ROTATION, std::bind(&ChickenBase::ChangeAliveWalk, this));
	aliveChanges_.emplace(ALIVE_MOVE::CALL, std::bind(&ChickenBase::ChangeAliveCall, this));
}

ChickenBase::~ChickenBase()
{
}

void ChickenBase::Create(VECTOR &pos)
{
	//���̎󂯎��
	trans_.pos = pos;		//�����ʒu
	
	//���f���ݒ�
	ModelSet();

	//�A�j���[�V�����Ǘ��ԍ��̏�����
	InitAnimNum();

	//�p�����[�^�[�ݒ�
	SetParam();
}

void ChickenBase::Update(void)
{
	//�f�o�b�O����
	DebagUpdate();

	//�A�j���[�V�����J�E���g
	Anim();

	// �X�V�X�e�b�v
 	stateUpdate_();

	//�g�����X�t�H�[���X�V
	trans_.Update();
}

void ChickenBase::Draw(void)
{
	stateDraw_();

	//�f�o�b�O�`��
	DebagDraw();
}

void ChickenBase::ModelSet()
{
	//���f���̏�����
	trans_.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::CHICKEN));
	trans_.scl = SCALE;
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f);

	trans_.Update();
}

void ChickenBase::SetParam()
{
	//�ړ��X�s�[�h
	moveSpeed_ = DEFAULT_SPEED;

	//�Փ˔���p���a
	radius_ = RADIUS;

	// �t�F�[�h����
	fadeStep_ = TIME_FADE;
	
	//�������̍s���������_���Ō��߂�
	aliveState_ = static_cast<ALIVE_MOVE>(GetRand(ALIVE_MOVE_MAX - 1));

	//�������(���������̏�Ԃ����߂Ă���s��)
    ChangeState(STATE::ALIVE);

	//�A�j���[�V�������
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
}

void ChickenBase::InitAnimNum(void)
{
	animNum_.emplace(ANIM::IDLE, ANIM_IDLE);
	animNum_.emplace(ANIM::WALK, ANIM_WALK);
	animNum_.emplace(ANIM::DAMAGE, ANIM_DAMAGE);
	animNum_.emplace(ANIM::DEATH, ANIM_DEATH);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_CALL);
}

void ChickenBase::SetTarget(const VECTOR pos)
{
	targetPos_ = pos;
}

void ChickenBase::SetDamage(const int damage)
{
	hp_ -= damage;
}

void ChickenBase::ChangeState(STATE state)
{
	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();
}

void ChickenBase::ChangeStateNone()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateNone, this);
	stateDraw_ = std::bind(&ChickenBase::DrawNone, this);
}

void ChickenBase::ChangeStateAlive()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateAlive, this);
	stateDraw_ = std::bind(&ChickenBase::DrawAlive, this);

	//�������̍s����Ԃ̔��f
	ChangeAliveState(aliveState_);
}

void ChickenBase::ChangeStateDamage()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateDamage, this);
	stateDraw_ = std::bind(&ChickenBase::DrawDamage, this);
}

void ChickenBase::ChangeStateDeath()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateDeath, this);
	stateDraw_ = std::bind(&ChickenBase::DrawDeath, this);
}

void ChickenBase::ChangeAliveState(ALIVE_MOVE state)
{
	// ��ԕύX
	aliveState_ = state;

	// �e��ԑJ�ڂ̏�������
	aliveChanges_[aliveState_]();
}

void ChickenBase::ChangeAliveIdle()
{
	stateAliveUpdate_ = std::bind(&ChickenBase::AliveIdle , this);
}

void ChickenBase::ChangeAliveWalk()
{
	stateAliveUpdate_ = std::bind(&ChickenBase::AliveRotation, this);
}

void ChickenBase::ChangeAliveCall()
{
	stateAliveUpdate_ = std::bind(&ChickenBase::AliveCall, this);
}

void ChickenBase::UpdateNone()
{
	//("#'��')~~���邱�ƂȂ�
}

void ChickenBase::UpdateAlive()
{
   	stateAliveUpdate_();
}

void ChickenBase::UpdateDamage()
{
	//�A�j���[�V������ύX
	ResetAnim(ANIM::DAMAGE, DEFAULT_SPEED_ANIM);
}

void ChickenBase::UpdateDeath()
{
	fadeStep_ -= SceneManager::GetInstance().GetDeltaTime();
	if (fadeStep_ < 0.0f)
	{
		return;
	}

	//�A�j���[�V������ύX
	ResetAnim(ANIM::DEATH, DEFAULT_SPEED_ANIM);
}

void ChickenBase::DrawNone()
{

}

void ChickenBase::DrawAlive()
{
	MV1DrawModel(trans_.modelId);
}

void ChickenBase::DrawDamage()
{
	MV1DrawModel(trans_.modelId);
}

void ChickenBase::DrawDeath()
{
	// ���Ԃɂ��F�̐��`���
	float diff = TIME_FADE - fadeStep_;
	auto c = AsoUtility::Lerp(FADE_C_FROM, FADE_C_TO, (diff / TIME_FADE));
	// ���f���̃}�e���A�����擾
	int num = MV1GetMaterialNum(trans_.modelId);
	for (int i = 0; i < num; i++)
	{
		// ���f���̃f�B�t���[�Y�J���[��ύX
		MV1SetMaterialDifColor(trans_.modelId, i, c);
	}
	// ���f���̕`��
	MV1DrawModel(trans_.modelId);
}

void ChickenBase::AliveIdle()
{
	LookTarget();

	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
}

void ChickenBase::AliveRotation()
{
	ResetAnim(ANIM::WALK, DEFAULT_SPEED_ANIM);

	VECTOR addAxis = AsoUtility::VECTOR_ZERO;
	addAxis.y = 0.8f;

	if (!AsoUtility::EqualsVZero(addAxis))
	{
		//�����]����������]�ʂ��N�H�[�^�j�I���ō��
		Quaternion rotPow = Quaternion();
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.x),
				AsoUtility::AXIS_X));

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.y),
				AsoUtility::AXIS_Y));

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.z),
				AsoUtility::AXIS_Z));

		// ��]�ʂ�������(����)
		trans_.quaRot = trans_.quaRot.Mult(rotPow);
	}

	// �O�������擾
	VECTOR forward = trans_.GetForward();

	// �ړ�
	trans_.pos = VAdd(trans_.pos, VScale(forward, moveSpeed_));
}

void ChickenBase::AliveCall()
{
	//�^�[�Q�b�g�𒍖�
	LookTarget();

	ResetAnim(ANIM::UNIQUE_1, DEFAULT_SPEED_ANIM);
}

void ChickenBase::LookTarget()
{
	//�W�I�ւ̕����x�N�g�����擾						��TODO:�x�N�g����SceneGame������炤
	VECTOR targetVec = VSub(targetPos_, trans_.pos);

	//���K��
	targetVec = VNorm(targetVec);

	//Y���W�͕K�v�Ȃ��̂ŗv�f������
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//��]
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);
}

void ChickenBase::FinishAnim(void)
{
	switch (anim_)
	{
	case UnitBase::ANIM::IDLE:
	case UnitBase::ANIM::WALK:
	case UnitBase::ANIM::UNIQUE_1:
		//���[�v�Đ�
		stepAnim_ = 0.0f;
		break;

	case UnitBase::ANIM::DAMAGE:
		ChangeState(STATE::ALIVE);
		break;

	case UnitBase::ANIM::DEATH:
		//1��ŏI��
		break;
	}
}

void ChickenBase::DebagUpdate()
{
	auto& ins = InputManager::GetInstance();

	//�|��鏈���̊m�F
	if (ins.IsTrgDown(KEY_INPUT_K))
	{
		ChangeState(STATE::DEATH);
	}
	//�_���[�W�����̊m�F
	else if (ins.IsTrgDown(KEY_INPUT_O))
	{
		ChangeState(STATE::DAMAGE);
	}
}

void ChickenBase::DebagDraw()
{
	int divNum = 20;
	int color = 0xff00ff;
	bool fill = false;

	//�����蔻��̕`��
	DrawSphere3D(trans_.pos, radius_, divNum, color, color, fill);
}