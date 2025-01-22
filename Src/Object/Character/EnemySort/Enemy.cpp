#include<DxLib.h>
#include<cmath>
#include"../../../Application.h"
#include"../../../Utility/AsoUtility.h"
#include "Enemy.h"

Enemy::Enemy(const VECTOR& _spawnPos)
{
	trans_.pos = _spawnPos;
}

void Enemy::Destroy(void)
{
	animNum_.clear();

	lastAtk_ = nullptr;
	delete lastAtk_;
}

void Enemy::Init(void)
{
	//��ԊǗ�
	stateChanges_.emplace(STATE::NORMAL, std::bind(&Enemy::ChangeStateNormal, this));
	stateChanges_.emplace(STATE::ALERT, std::bind(&Enemy::ChangeStateAlert, this));
	stateChanges_.emplace(STATE::ATTACK, std::bind(&Enemy::ChangeStateAtk, this));
	stateChanges_.emplace(STATE::BREAK, std::bind(&Enemy::ChangeStateBreak, this));

	//�L�����ŗL�ݒ�
	SetParam();

	//�A�j���[�V�����ԍ��̏�����
	InitAnim();

	//���ʂ̕ϐ��̏�����
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
	ChangeState(STATE::NORMAL);
	alertCnt_ = 0.0f;
	breakCnt_ = 0.0f;
	stunDef_ = 0;
	atkAct_ = ATK_ACT::MAX;
	isEndAllAtk_ = false;

	//�U�����̏�����
	InitSkill();
	atk_.ResetCnt();

	trans_.Update();
}

void Enemy::Update(void)
{
	//�A�j���[�V����
	Anim();

	//���W�o�b�N�A�b�v
	prePos_ = trans_.pos;

#ifdef DEBUG_ENEMY
	//���͗p
	InputManager& ins = InputManager::GetInstance();

	//if (ins.IsNew(KEY_INPUT_W)) { targetPos_.z+= 3.0f; }
	//if (ins.IsNew(KEY_INPUT_D)) { targetPos_.x+= 3.0f; }
	//if (ins.IsNew(KEY_INPUT_S)) { targetPos_.z-= 3.0f; }
	//if (ins.IsNew(KEY_INPUT_A)) { targetPos_.x-= 3.0f; }

	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Q)) { Damage(1, 10); }
#endif // DEBUG_ENEMY


	//����Ă���Ȃ牽�����Ȃ�
	if (!IsAlive()) { return; }

	//�����蔻����W�̍X�V
	colPos_ = VAdd(trans_.pos, localCenterPos_);

	//��Ԃ��Ƃ�Update
	stateUpdate_();

	//���f������
	trans_.Update();
}

void Enemy::Damage(const int _damage, const int _stunPow)
{
	//���ɂ���Ă���Ȃ珈�����Ȃ�
	if (!IsAlive()) { return; }

	//�_���[�W�J�E���g
	hp_ -= _damage;

	//�X�^���l�J�E���g
	stunDef_ += _stunPow;

	//���ꂽ�玀�S�A�j���[�V����
	if (!IsAlive()){ ResetAnim(ANIM::DEATH, changeSpeedAnim_[ANIM::DEATH]); }
}

void Enemy::ChangeState(const STATE _state)
{
	//��ԑJ��
	state_ = _state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();
}

void Enemy::ChangeStateNormal(void)
{
	//�X�V�����̒��g������
	stateUpdate_ = std::bind(&Enemy::UpdateNml, this);
}

void Enemy::ChangeStateAlert(void)
{
	//�X�V�����̒��g������
	stateUpdate_ = std::bind(&Enemy::UpdateAlert, this);

	//���������߂Đݒ�
	trans_.quaRot = trans_.quaRot.LookRotation(GetTargetVec());

	//�����_���ōU�����𐶐�
	RandSkill();

	//�\������A�j���[�V����
	ResetAnim(nowSkillPreAnim_, changeSpeedAnim_[nowSkillPreAnim_]);

	//�x���J�E���^������
	alertCnt_ = 0.0f;
}

void Enemy::ChangeStateAtk(void)
{
	//�X�V�����̒��g������
	stateUpdate_ = std::bind(&Enemy::UpdateAtk, this);

	//�U������
	lastAtk_ = &createSkill_();
}

void Enemy::ChangeStateBreak(void)
{
	//�X�V�����̒��g������
	stateUpdate_ = std::bind(&Enemy::UpdateBreak, this);

	//�U���x�e���Ԃ̏�����
	breakCnt_ = 0;
}

void Enemy::InitAnim()
{
	//���ʃA�j���[�V����
	animNum_.emplace(ANIM::IDLE, ANIM_IDLE);
	animNum_.emplace(ANIM::WALK, ANIM_WALK);
	animNum_.emplace(ANIM::RUN, ANIM_RUN);
	animNum_.emplace(ANIM::DAMAGE, ANIM_DAMAGE);
	animNum_.emplace(ANIM::DEATH, ANIM_DEATH);
	animNum_.emplace(ANIM::ENTRY, ANIM_ENTRY);

	//�A�j���[�V�������x�ݒ�
	changeSpeedAnim_.emplace(ANIM::IDLE, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::WALK, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::RUN, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::DAMAGE, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::DEATH, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::ENTRY, SPEED_ANIM);
}

void Enemy::Alert(void)
{
	//�x��
	alertNowSkill_();
}

void Enemy::Attack(void)
{
	//�Ή��X�L������
	processSkill_();

	//�e�U���̃J�E���g
	for (auto& nowSkill : nowSkill_)
	{
		//�U���̃J�E���g
		CntUp(nowSkill.cnt_);
	}
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
	if (moveSpeed_ == 0.0)ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
	//�����A�j���[�V����
	else if (moveSpeed_ > 0.0f)ResetAnim(ANIM::WALK, changeSpeedAnim_[ANIM::WALK]);

	//�ړ��ʂ̏�����
	moveSpeed_ = 0.0f;
	
	//���G
	if (!isMove_)return;

	//�ړ�����
	Move();
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

	//�x��
	Alert();

	//�N�[���_�E���J�E���^
	CntUp(alertCnt_);
}

void Enemy::UpdateAtk(void)
{
	//**********************************************************
	//�I������
	//**********************************************************

	//�U�����I����Ă���Ȃ��ԑJ��
	if (isEndAllAtk_)
	{
		//�U���I������̏�����
		ResetAtkJudge();

		//�x�e��ԂɑJ��
		ChangeState(STATE::BREAK);
		return;
	}

	//**********************************************************
	//���쏈��
	//**********************************************************

	//�U������
	Attack();

	//�U���A�j���[�V����
	ResetAnim(nowSkillAnim_, changeSpeedAnim_[nowSkillAnim_]);
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
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);

	//�U���x�e���ԃJ�E���^
	CntUp(breakCnt_);
}

void Enemy::DrawDebug(void)
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
	DrawSphere3D(colPos_, radius_, 4, 0xffff00, 0xffff00, false);
	//�G�̍��G����
	DrawSphere3D(trans_.pos, searchRange_, 2, isMove_ ? 0xff0000 : 0xffffff, isMove_ ? 0xff0000 : 0xffffff, false);
	//�G�̍��G����
	DrawSphere3D(trans_.pos, atkStartRange_, 2, state_ == STATE::ALERT ? 0xff0000 : 0xffffff, state_ == STATE::ALERT ? 0x0000ff : 0xffffff, false);
}

void Enemy::Draw(void)
{
#ifdef DEBUG_ENEMY
	
	//�f�o�b�O
	DrawDebug();

#endif // DEBUG_ENEMY

	//�G���f���̕`��
	MV1DrawModel(trans_.modelId);

	for (auto& nowSkill : nowSkill_)
	{
		//�U���̕`��
		if (nowSkill.IsAttack()) { DrawSphere3D(nowSkill.pos_, nowSkill.radius_, 50.0f, 0xff0f0f, 0xff0f0f, true); }
		else if (nowSkill.IsBacklash()) { DrawSphere3D(nowSkill.pos_, nowSkill.radius_, 5.0f, 0xff0f0f, 0xff0f0f, false); }
	}
}

const VECTOR Enemy::GetTargetVec(const float _speed) const
{
	//�W�I�ւ̕����x�N�g�����擾						��targetPos��SceneGame������炤
	VECTOR targetVec = VSub(targetPos_, trans_.pos);

	//���K��
	targetVec = VNorm(targetVec);

	//Y���W�͕K�v�Ȃ��̂ŗv�f������
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//�ړ��ʂ����߂�
	VECTOR ret = VScale(targetVec, _speed);

	return ret;
}

const VECTOR Enemy::GetTargetVec(const VECTOR _pos, const float _speed) const
{
	//�W�I�ւ̕����x�N�g�����擾						��targetPos��SceneGame������炤
	VECTOR targetVec = VSub(targetPos_, _pos);

	//���K��
	targetVec = VNorm(targetVec);

	//Y���W�͕K�v�Ȃ��̂ŗv�f������
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//�ړ��ʂ����߂�
	VECTOR ret = VScale(targetVec, _speed);

	return ret;
}

void Enemy::Move(void)
{
	//�ړ����x�̍X�V
	moveSpeed_ = walkSpeed_;

	//�ړ��x�N�g���擾
	VECTOR targetVec = GetTargetVec(moveSpeed_);

	//������]
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);

	//�ړ�
	trans_.pos = VAdd(trans_.pos, targetVec);
}

Enemy::ATK& Enemy::CreateSkill(ATK_ACT _atkAct)
{
	//**********************************************************
	//�g���I������U��������ꍇ
	//**********************************************************

	//�g���I������U���ɏ㏑��
	for (auto& nowSkill : nowSkill_)
	{
		if (nowSkill.IsFinishMotion())
		{
			//�X�L���㏑��
			nowSkill = skills_[_atkAct];
			
			//�J�E���^�̏�����
			nowSkill.ResetCnt();

			//�q�b�g����̏�����
			nowSkill.isHit_ = false;

			//�����I��
			return nowSkill;
		}
	}

	//**********************************************************
	//�Ȃ��ꍇ
	//**********************************************************

	//�����_���łƂ��Ă����U���̎�ނ������甭������X�L���ɐݒ�
	nowSkill_.emplace_back(skills_[_atkAct]);

	//�J�E���^�̏�����
	nowSkill_.back().ResetCnt();

	//�q�b�g����̏�����
	nowSkill_.back().isHit_ = false;

	//�����I��
	return nowSkill_.back();
}

void Enemy::FinishAnim(void)
{
	//�A�j���[�V��������
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

void Enemy::ResetAtkJudge(void)
{
	//�U���I������̏�����
	isEndAllAtk_ = false;
}

void Enemy::RandSkill(void)
{
	//�X�L���̐�
	size_t size = skills_.size();

	//�X�L���̐������烉���_�������
	int rand = GetRand(size - 1);

	//�X�L��
	atkAct_ = static_cast<ATK_ACT>(rand);

	//�X�L����������
	SetUpSkill(atkAct_);

	//�X�L���ɑΉ������\������A�j���[�V�����̋L�^
	nowSkillPreAnim_ = skillPreAnims_[static_cast<int>(atkAct_)];

	//�X�L���ɑΉ������A�j���[�V�����̋L�^
	nowSkillAnim_ = skillAnims_[static_cast<int>(atkAct_)];

	//�X�L������
	createSkill_ = std::bind(&Enemy::CreateSkill, this, atkAct_);
}

void Enemy::SetUpSkill(ATK_ACT _atkAct)
{
	//�x�������Z�b�g
	alertNowSkill_ = alertSkills_[_atkAct];

	//�U�������Z�b�g
	processSkill_ = changeSkill_[_atkAct];
}
