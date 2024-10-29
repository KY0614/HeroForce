#include<DxLib.h>
#include"../../Application.h"
#include"../../Manager/Resource.h"
#include"../../Manager/InputManager.h"
#include"../../Manager/ResourceManager.h"
#include"../../Utility/AsoUtility.h"
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

	//�A�j���[�V�����ԍ��̏�����
	InitAnimNum();

	//���f�o�b�O�@���u���A�j���[�V����
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_1);
	animNum_.emplace(ANIM::SKILL_2, ANIM_SKILL_2);

	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);

	float scl = 0.2f;

	//���ʂ̕ϐ��̏�����
	trans_.scl = { scl,scl,scl };
	trans_.pos = AsoUtility::VECTOR_ZERO;
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
	state_ = STATE::NORMAL;
	atkCdt_ = 0.0f;
	alertCnt_ = 0.0f;
	breakCnt_ = 0.0f;
	stunDef_ = 0;
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);

	//�����蔻��̐ݒ�
	radius_ = MY_COL_RADIUS;
	//�U�����̏�����
	InitSkill();
	atk_.ResetCnt();

	trans_.Update();
}

void Enemy::SetParam(void)
{
	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_AXEMAN));

	//���X�Őݒ肷��
	colRadius_ = MY_COL_RADIUS;
	exp_ = 1.0f;
	hp_ = 5;
	stunDefMax_ = 100;
}

void Enemy::Update(void)
{
	//�A�j���[�V����
	Anim();

	//����Ă���Ȃ牽�����Ȃ�
	if (!IsAlive()) { return; }

	//�����蔻����W�̍X�V
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);

	//��Ԃ��Ƃ�Update
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		UpdateNml();
		break;

	case Enemy::STATE::ALERT:
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
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Q)) { Damage(1, 10); }

	//���f������
	trans_.Update();
}

void Enemy::Damage(const int _damage, const int _stunPow)
{
	//���ɂ���Ă���Ȃ珈�����Ȃ�
	if (!IsAlive()) { return; }

	//�_���[�W
	hp_ -= _damage;

	//�X�^���l
	stunDef_ += _stunPow;

	//���ꂽ�玀�S�A�j���[�V����
	if (!IsAlive()){ ResetAnim(ANIM::DEATH, DEFAULT_SPEED_ANIM); }
}

void Enemy::InitAnimNum()
{
	//���ʃA�j���[�V����
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
		ChangeState(STATE::ALERT);

		return;
	}

	//�ҋ@�A�j���[�V����
	if (moveSpeed_ == 0.0) { ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM); }

	//�����������
	//�N�[���_�E���J�E���^
	atkCdt_++;

	//�ړ�����
	Move();
}

void Enemy::UpdateAlert(void)
{
	//�x���J�E���^���I������Ȃ�U���J�n
	if (!IsAlertTime())
	{
		//�x���J�E���^������
		alertCnt_ = 0.0f;

		//�U����ԂɑJ��
		ChangeState(STATE::ATTACK);

		return;
	}

	//�N�[���_�E���J�E���^
	alertCnt_++;

	//�������I����ĂȂ��Ȃ琶������
	if (atk_.IsFinishMotion())
	{
		//�����_���ōU������
		RandSkill();
	}
}

void Enemy::UpdateAtk(void)
{
	//�U�����I����Ă���Ȃ��ԑJ��
	if (atk_.IsFinishMotion())
	{
		//�x�e��ԂɑJ��
		ChangeState(STATE::BREAK);

		return;
	}

	//**********************************************************
	//�U�����̏���
	//**********************************************************

	//�U���A�j���[�V����
	ResetAnim(nowSkillAnim_, DEFAULT_SPEED_ANIM);

	//�U���̃J�E���^
	atk_.cnt_++;

	//�U������
	Attack();
}

void Enemy::UpdateBreak(void)
{
	if (!IsBreak())
	{
		//�U���x�e���Ԃ̏�����
		breakCnt_ = 0;

		//�ʏ��ԂɑJ��
		ChangeState(STATE::NORMAL);
		return;
	}

	//�ҋ@�A�j���[�V����
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);

	//�U���x�e���ԃJ�E���^
	breakCnt_++;
}

void Enemy::Search(void)
{
}

void Enemy::Draw(void)
{
	//�f�o�b�O
	DrawFormatString(0, Application::SCREEN_SIZE_Y - 16, 0xffffff, "EnemyHP = %d", hp_);
	int statePos = Application::SCREEN_SIZE_Y - 32;
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		DrawFormatString(0, statePos, 0x00ff00, "�G���ݏ��:NORMAL");
		break;

	case Enemy::STATE::ALERT:
		DrawFormatString(0, statePos, 0x00ff00, "�G���ݏ��:ALERT_ATTACK");
		break;

	case Enemy::STATE::ATTACK:
		DrawFormatString(0, statePos, 0x00ff00, "�G���ݏ��:ATTACK");
		break;

	case Enemy::STATE::BREAK:
		DrawFormatString(0, statePos, 0x00ff00, "�G���ݏ��:BREAK");
		break;
	}
	//�G�̓����蔻��
	DrawSphere3D(colPos_, colRadius_, 4, 0xffff00, 0xffff00, false);

	//�G���f���̕`��
	MV1DrawModel(trans_.modelId);

	//�U���̕`��
	if (atk_.IsAttack()) { DrawSphere3D(atk_.pos_, nowSkillColRadius_, 50.0f, 0xff0f0f, 0xff0f0f, true); }
	else if (atk_.IsBacklash()) { DrawSphere3D(atk_.pos_, nowSkillColRadius_, 5.0f, 0xff0f0f, 0xff0f0f, false); }
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

	//�����A�j���[�V����
	if(moveSpeed_ > 0.0f)ResetAnim(ANIM::WALK, DEFAULT_SPEED_ANIM);
}

void Enemy::ProcessMove(const float _moveSpeed, const float _deg)
{
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
	skills_.emplace_back(SKILL_1);
	skills_.emplace_back(SKILL_2);

	//�����ɃX�L���̐����A�j���[�V�������i�[������
	skillAnims_.emplace_back(ANIM::SKILL_1);
	skillAnims_.emplace_back(ANIM::SKILL_2);

	//�����ɃX�L���̓����蔻�蔼�a���i�[������
	skillColRadius_.emplace_back(SKILL_1_COL_RADIUS);
	skillColRadius_.emplace_back(SKILL_2_COL_RADIUS);

	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void Enemy::FinishAnim(void)
{
	switch (anim_)
	{
	case UnitBase::ANIM::IDLE:
	case UnitBase::ANIM::WALK:
	case UnitBase::ANIM::RUN:
		//���[�v�Đ�
		stepAnim_ = 0.0f;
		break;

	case UnitBase::ANIM::SKILL_1:
	case UnitBase::ANIM::SKILL_2:
	case UnitBase::ANIM::DAMAGE:
	case UnitBase::ANIM::DEATH:
	case UnitBase::ANIM::ENTRY:
		break;
	}
}

void Enemy::Attack(void)
{
	//�O����
	VECTOR dir = trans_.quaRot.GetForward();

	//���W�̐ݒ�
	atk_.pos_ = VAdd(colPos_, VScale(dir, nowSkillColRadius_));
}

void Enemy::RandSkill(void)
{
	//�X�L���̐�
	size_t size = skills_.size();

	//�X�L���̐������烉���_�������
	int rand = GetRand(size - 1);

	//�����_���łƂ��Ă����U���̎�ނ������甭������X�L���ɐݒ�
	atk_ = skills_[rand];
	nowSkillAnim_ = skillAnims_[rand];
	nowSkillColRadius_ = skillColRadius_[rand];
}
