#include<DxLib.h>
#include"../Manager/Resource.h"
#include"../Manager/InputManager.h"
#include"../Manager/ResourceManager.h"
#include"../Utility/AsoUtility.h"
#include "Enemy.h"

Enemy::Enemy()
{
	atkCdt_ = 0.0f;
	exp_ = 1.0f;
	hp_ = 5;
	moveSpeed_ = 0.0f;
	state_ = STATE::NORMAL;
}

void Enemy::Destroy(void)
{
	animNum_.clear();
}

void Enemy::Init(void)
{
	//�L�����ŗL�ݒ�
	SetParam();

	float scl = 0.2f;

	//���ʂ̕ϐ��̏�����
	trans_.scl = { scl,scl,scl };
	trans_.pos = { 0.0f,0.0f,0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
	state_ = STATE::NORMAL;
	atkCdt_ = 0.0f;
	stunDef_ = 0;

	//�U�����̏�����
	InitSkill();
	nowSkill_.ResetCnt();

	trans_.Update();
}

void Enemy::SetParam(void)
{
	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_AXEMAN));

	//�A�j���[�V�����ԍ��̏�����
	InitAnimNum();
	ResetAnim(ANIM::WALK);

	exp_ = 1.0f;
	hp_ = 5;
	stunDefMax_ = 100;
}

void Enemy::Update(void)
{
	//����Ă���Ȃ牽�����Ȃ�
	if (!IsAlive()) { return; }

	//��Ԃ��Ƃ�Update
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		UpdateNml();
		break;

	case Enemy::STATE::ALERT_ATTACK:
		UpdateAlert();
		break;

	case Enemy::STATE::ATTACK:
		UpdateAtk();
		break;

	case Enemy::STATE::BREAK:
		UpdateBreak();
		break;
	}

	//���f�o�b�O�@�_���[�W����
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Q)) { Damage(1); }

	//���f������
	trans_.Update();

	//�A�j���[�V����
	Anim();
}

void Enemy::Damage(const int _damage)
{
	//�_���[�W
	hp_ -= _damage;
	if (!IsAlive()){ ResetAnim(ANIM::DEATH); }
}

void Enemy::InitAnimNum()
{
	animNum_.emplace(ANIM::IDLE, ANIM_IDLE);
	animNum_.emplace(ANIM::WALK, ANIM_WALK);
	animNum_.emplace(ANIM::RUN, ANIM_RUN);
	animNum_.emplace(ANIM::DAMAGE, ANIM_DAMAGE);
	animNum_.emplace(ANIM::DEATH, ANIM_DEATH);
	animNum_.emplace(ANIM::ENTRY, ANIM_ENTRY);
}

void Enemy::UpdateNml(void)
{
	//�U���N�[���_�E�����I������Ȃ�U�������J�n
	if (!IsAtkCD())
	{
		//�U���N�[���_�E���J�E���^������
		atkCdt_ = 0.0f;

		//�U���x����ԂɑJ��
		ChangeState(STATE::ALERT_ATTACK);

		return;
	}

	//�A�j���[�V�����ύX(��x����)
	if (moveSpeed_ == 0.0f && anim_ != ANIM::IDLE) { ResetAnim(ANIM::IDLE); }

	//�N�[���_�E���J�E���^
	atkCdt_++;

	//�ړ�����
	Move();
}

void Enemy::UpdateAlert(void)
{
	//�U���N�[���_�E�����I������Ȃ�U���J�n
	if (!IsAtkCD())
	{
		//�U���N�[���_�E���J�E���^������
		atkCdt_ = 0.0f;

		//�U����ԂɑJ��
		ChangeState(STATE::ATTACK);

		return;
	}

	//�N�[���_�E���J�E���^
	atkCdt_++;

	//�������I����ĂȂ��Ȃ琶������
	if (nowSkill_.IsFinishMotion()) 
	{
		//�����_���ōU������
		RandSkill();
	}
}

void Enemy::UpdateAtk(void)
{
	//�U�����I��������ԑJ��
	if (nowSkill_.IsFinishMotion())
	{
		//�U���N�[���_�E���J�E���^������
		atkCdt_ = 0.0f;

		//�x�e��ԂɑJ��
		ChangeState(STATE::BREAK);

		return;
	}

	//�U���̃J�E���^
	nowSkill_.cnt_++;

	//�U������
	Attack();
}

void Enemy::UpdateBreak(void)
{
	if (atkBtc_ > ATK_BTC)
	{
		//�U���x�e���Ԃ̏�����
		atkBtc_ = 0;

		//�ʏ��ԂɑJ��
		ChangeState(STATE::NORMAL);
		return;
	}

	//�U���x�e���ԃJ�E���^
	atkBtc_++;
}

void Enemy::Search(void)
{
}

void Enemy::Draw(void)
{
	//�f�o�b�O
	//DrawFormatString(0, 0, IsAtkCD() ? 0xffffff : 0xff0000, IsAtkCD() ? "�U���N�[���_�E����" : "�N�[���_�E���I���I�I");
	//DrawFormatString(0, 16, nowSkill_.IsFinishMotion() ? 0xffffff : 0xff0000, nowSkill_.IsFinishMotion() ? "�U�����I�I" : "�U���I���I�I");
	DrawFormatString(0, 16, 0xffffff, "HP = %d", hp_);
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		DrawFormatString(0, 0, 0x00ff00, "���ݏ��:NORMAL");
		break;

	case Enemy::STATE::ALERT_ATTACK:
		DrawFormatString(0, 0, 0x00ff00, "���ݏ��:ALERT_ATTACK");
		break;

	case Enemy::STATE::ATTACK:
		DrawFormatString(0, 0, 0x00ff00, "���ݏ��:ATTACK");
		break;

	case Enemy::STATE::BREAK:
		DrawFormatString(0, 0, 0x00ff00, "���ݏ��:BREAK");
		break;
	}

	if (!IsAlive()) { return; }

	//�G���f���̕`��
	MV1DrawModel(trans_.modelId);

	//�U���̕`��
	if (nowSkill_.IsAttack()) { DrawSphere3D(nowSkill_.pos_, 25.0f, 50.0f, 0xff0f0f, 0xff0f0f, true); }
	else if (nowSkill_.IsBacklash_()) { DrawSphere3D(nowSkill_.pos_, 20.0f, 50.0f, 0xff0f0f, 0xff0f0f, false); }
}

void Enemy::Move(void)
{
	//�ړ��ʂ̏�����
	moveSpeed_ = 0.0f;

	//���͗p
	InputManager& ins = InputManager::GetInstance();

	//�ړ�(�f�o�b�O)
	if (ins.IsNew(KEY_INPUT_UP)) { moveSpeed_ = WALK_SPEED; ProcessMove(moveSpeed_, 0.0f); }
	if (ins.IsNew(KEY_INPUT_RIGHT)){ moveSpeed_ = WALK_SPEED; ProcessMove(moveSpeed_, 90.0f); }
	if (ins.IsNew(KEY_INPUT_DOWN)){ moveSpeed_ = WALK_SPEED; ProcessMove(moveSpeed_, 180.0f); }
	if (ins.IsNew(KEY_INPUT_LEFT)){ moveSpeed_ = WALK_SPEED; ProcessMove(moveSpeed_, 270.0f); }
}

void Enemy::ProcessMove(const float _moveSpeed, const float _deg)
{
	//�A�j���[�V�����ύX(��x����)
	if (anim_ != ANIM::WALK) { ResetAnim(ANIM::WALK); }

	//�����]��
	Turn(_deg, AsoUtility::AXIS_Y);

	//�ړ�����
	VECTOR dir = trans_.quaRot.GetForward();

	//�ړ���
	VECTOR movePow = VScale(dir, _moveSpeed);

	//�ړ�
	trans_.pos = VAdd(trans_.pos, movePow);
}

void Enemy::Turn(float _deg, VECTOR _axis)
{
	//trans_.quaRot = trans_.quaRot.Mult(trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis));
	trans_.quaRot = trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}

void Enemy::InitSkill(void)
{
	//�����ɃX�L���̐����i�[������
	skill_.emplace_back(SKILL_1);
	skill_.emplace_back(SKILL_2);

	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void Enemy::Attack(void)
{
	//�O����
	VECTOR dir = trans_.quaRot.GetForward();

	//���W�̐ݒ�
	nowSkill_.pos_ = VAdd(trans_.pos, VScale(dir, 20.0f));
}

void Enemy::RandSkill(void)
{
	//�X�L���̐�
	size_t size = skill_.size();

	//�X�L���̐������烉���_�������
	int rand = GetRand(size - 1);

	//�����_���łƂ��Ă����U���̎�ނ������甭������X�L���ɐݒ�
	nowSkill_ = skill_[rand];
}
