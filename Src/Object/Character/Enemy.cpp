#include<DxLib.h>
#include<cmath>
#include"../../Application.h"
#include"../../Manager/Resource.h"
#include"../../Manager/InputManager.h"
#include"../../Manager/ResourceManager.h"
#include"../../Manager/Collision.h"
#include"../../Utility/AsoUtility.h"
#include "Enemy.h"

Enemy::Enemy()
{
	exp_ = 1.0f;
	hp_ = 5;
	moveSpeed_ = 0.0f;
	state_ = STATE::NORMAL;
	anim_ = ANIM::NONE;
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

#ifdef DEBUG_ENEMY
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_1);
	animNum_.emplace(ANIM::SKILL_2, ANIM_SKILL_2);
	targetPos_ = { -30.0f, 0.0f,-30.0f };
#endif // DEBUG_ENEMY

	//�A�j���[�V�������Z�b�g
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);

	//���ʂ̕ϐ��̏�����
	trans_.scl = { CHARACTER_SCALE,CHARACTER_SCALE,CHARACTER_SCALE };
	trans_.pos = AsoUtility::VECTOR_ZERO;
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
	state_ = STATE::NORMAL;
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
	radius_ = MY_COL_RADIUS;
	exp_ = 1.0f;
	hp_ = 5;
	stunDefMax_ = 100;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
}

void Enemy::Update(void)
{
	//�A�j���[�V����
	Anim();

#ifdef DEBUG_ENEMY
	//���͗p
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_W)) { targetPos_.z+= 3.0f; }
	if (ins.IsNew(KEY_INPUT_D)) { targetPos_.x+= 3.0f; }
	if (ins.IsNew(KEY_INPUT_S)) { targetPos_.z-= 3.0f; }
	if (ins.IsNew(KEY_INPUT_A)) { targetPos_.x-= 3.0f; }

	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Q)) { Damage(1, 10); }
#endif // DEBUG_ENEMY


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

void Enemy::ChangeState(const STATE _state)
{
	//��ԑJ��
	state_ = _state;

	//��ԑJ�ڂɂ����鏉����
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		break;
	
	case Enemy::STATE::ALERT:
		//�x���J�E���^������
		alertCnt_ = 0.0f;
		break;

	case Enemy::STATE::ATTACK:
		break;
	
	case Enemy::STATE::BREAK:
		//�U���x�e���Ԃ̏�����
		breakCnt_ = 0;
		break;
	}
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
	//**********************************************************
	//�I������
	//**********************************************************


	/*�Q�[���V�[���ɂ���܂�*/


	//**********************************************************
	//���쏈��
	//**********************************************************

	//�ҋ@�A�j���[�V����
	if (moveSpeed_ == 0.0)ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
	//�����A�j���[�V����
	else if (moveSpeed_ > 0.0f)ResetAnim(ANIM::WALK, DEFAULT_SPEED_ANIM);

	//�ړ��ʂ̏�����
	moveSpeed_ = 0.0f;
	
	//���G
	if (isMove_)
	{
		//�ړ�����
		Move();
	}
}

void Enemy::UpdateAlert(void)
{
	//**********************************************************
	//�I������
	//**********************************************************

	//�x���J�E���^���I������Ȃ�U���J�n
	if (!IsAlertTime())
	{
		//�U����ԂɑJ��
		ChangeState(STATE::ATTACK);
		return;
	}

	//**********************************************************
	//���쏈��
	//**********************************************************

	//�N�[���_�E���J�E���^
	alertCnt_++;

	//�������I����ĂȂ��Ȃ琶������
	if (nowSkill_.front().IsFinishMotion())
	{
		//�����_���ōU������
		RandSkill();
	}
}

void Enemy::UpdateAtk(void)
{
	//�U�����I����Ă���Ȃ��ԑJ��
	if (nowSkill_.back().IsFinishMotion())
	{
		//�x�e��ԂɑJ��
		ChangeState(STATE::BREAK);
		return;
	}

	//**********************************************************
	//���쏈��
	//**********************************************************

	//�U���A�j���[�V����
	ResetAnim(nowSkillAnim_, DEFAULT_SPEED_ANIM);

	for (auto& nowSkill : nowSkill_)
	{
		//�U���̃J�E���^
		nowSkill.cnt_++;
	}

	//�U������
	Attack();
}

void Enemy::UpdateBreak(void)
{
	//**********************************************************
	//�I������
	//**********************************************************

	//�x�e���Ԃ��I�������
	if (!IsBreak())
	{
		//�ʏ��ԂɑJ��
		ChangeState(STATE::NORMAL);
		return;
	}

	//**********************************************************
	//���쏈��
	//**********************************************************

	//�ҋ@�A�j���[�V����
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);

	//�U���x�e���ԃJ�E���^
	breakCnt_++;
}

void Enemy::Draw(void)
{
#ifdef DEBUG_ENEMY
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
	DrawSphere3D(colPos_, radius_, 4, 0xffff00, 0xffff00, false);
	//�G�̍��G����
	DrawSphere3D(trans_.pos, searchRange_, 2, isMove_ ? 0xff0000 : 0xffffff, isMove_ ? 0xff0000 : 0xffffff, false);
	//�G�̍��G����
	DrawSphere3D(trans_.pos, atkStartRange_, 2, state_ == STATE::ALERT ? 0xff0000 : 0xffffff, state_ == STATE::ALERT ? 0x0000ff : 0xffffff, false);
	//�^�[�Q�b�g�̍��W
	DrawSphere3D(targetPos_, 3.0f, 10, 0x0000ff, 0x0000ff, true);
#endif // DEBUG_ENEMY

	//�G���f���̕`��
	MV1DrawModel(trans_.modelId);

	for (auto& nowSkill : nowSkill_)
	{
		//�U���̕`��
		if (nowSkill.IsAttack()) { DrawSphere3D(nowSkill.pos_, nowSkill.radius_, 50.0f, 0xff0f0f, 0xff0f0f, true); }
		else if (nowSkill.IsBacklash()) { DrawSphere3D(nowSkill.pos_, nowSkill.radius_, 5.0f, 0xff0f0f, 0xff0f0f, false); }
	}
	//�U���̕`��
	//if (atk_.IsAttack()) { DrawSphere3D(atk_.pos_, nowSkillColRadius_, 50.0f, 0xff0f0f, 0xff0f0f, true); }
	//else if (atk_.IsBacklash()) { DrawSphere3D(atk_.pos_, nowSkillColRadius_, 5.0f, 0xff0f0f, 0xff0f0f, false); }
}

const VECTOR Enemy::GetTargetVec(void)const
{
	//�W�I�ւ̕����x�N�g�����擾
	VECTOR targetVec = VSub(targetPos_, trans_.pos);

	//���K��
	targetVec = VNorm(targetVec);

	//Y���W�͕K�v�Ȃ��̂ŗv�f������
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//�ړ��ʂ����߂�
	VECTOR ret = VScale(targetVec, moveSpeed_);

	return ret;
}

void Enemy::Move(void)
{
	//�ړ����x�̍X�V
	moveSpeed_ = WALK_SPEED;

	//�����x�N�g���擾
	VECTOR targetVec = GetTargetVec();

	//��]
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);

	//�ړ�
	trans_.pos = VAdd(trans_.pos, targetVec);
}

void Enemy::InitSkill(void)
{
	//�����ɃX�L���̐����i�[������
	skills_.emplace_back(SKILL_1);
	skills_.emplace_back(SKILL_2);

	//�����ɃX�L���̐����A�j���[�V�������i�[������
	skillAnims_.emplace_back(ANIM::SKILL_1);
	skillAnims_.emplace_back(ANIM::SKILL_2);

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

	for (auto& nowSkill : nowSkill_)
	{
		//���W�̐ݒ�
		nowSkill.pos_ = VAdd(colPos_, VScale(dir, nowSkill.radius_));
	}
	//���W�̐ݒ�
	//atk_.pos_ = VAdd(colPos_, VScale(dir, nowSkillColRadius_));
}

void Enemy::RandSkill(void)
{
	//�X�L���̐�
	size_t size = skills_.size();

	//�X�L���̐������烉���_�������
	int rand = GetRand(size - 1);

	//**********************************************************
	//�g���I������U��������ꍇ
	//**********************************************************
	
	//�g���I������U���ɏ㏑��
	for (auto& nowSkill : nowSkill_)
	{
		if (nowSkill.IsFinishMotion())
		{
			//�㏑��
			nowSkill = skills_[rand];
			
			//�J�E���^�̏�����
			nowSkill.ResetCnt();

			//�����I��
			return;
		}
	}

	//**********************************************************
	//�Ȃ��ꍇ
	//**********************************************************

	//�����_���łƂ��Ă����U���̎�ނ������甭������X�L���ɐݒ�
	nowSkill_.emplace_back(skills_[rand]);
	//atk_ = skills_[rand];
	nowSkillAnim_ = skillAnims_[rand];

	//�J�E���^�̏�����
	nowSkill_.back().ResetCnt();
}