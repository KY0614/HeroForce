#include"../Manager/ResourceManager.h"
#include "PlAxeMan.h"

PlAxe::PlAxe(const SceneManager::PLAY_MODE _mode, const SceneManager::CNTL _cntl) : PlayerBase(_mode, _cntl)
{
	mode_ = _mode;
	cntl_ = _cntl;
	hp_ = MAX_HP;

	//�ϐ�������
	moveAble_ = true;

	//�����蔻��̐ݒ�
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	atkStartRange_ = ATK_START_RANGE;



}

PlAxe::PlAxe(const SceneManager::PLAY_MODE _mode, const InputManager::JOYPAD_NO _padNum) : PlayerBase(_mode, _padNum)
{
	mode_ = _mode;
	padNum_ = _padNum;

	//�ϐ��̏�����
	hp_ = MAX_HP;

	moveAble_ = true;

	//�����蔻��̐ݒ�
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	atkStartRange_ = ATK_START_RANGE;
}

void PlAxe::SetParam(void)
{
	InitAct();

	//�U���̏�����
	InitAtk();

	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));
	float scale = CHARACTER_SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { -300.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);

	hp_ = MAX_HP;

	moveAble_ = true;

	//�����蔻��̐ݒ�
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	atkStartRange_ = ATK_START_RANGE;
}
void PlAxe::InitAct(void)
{
	//�ʏ�U���̍ő�l
	atkMax_.emplace(ATK_ACT::ATK, ATK_MAX);

	//�X�L���P�̍ő�l
	atkMax_.emplace(ATK_ACT::SKILL1, SKILL_ONE_MAX);

	//�X�L���Q�̍ő�l
	atkMax_.emplace(ATK_ACT::SKILL2, SKILL_TWO_MAX);


	//�N�[���^�C��
	coolTimeMax_[static_cast<int>(ATK_ACT::ATK)] = ATK_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_COOLTIME;

	//�U����������
	atkStartTime_[static_cast<int>(ATK_ACT::ATK)] = ATK_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_START;

	//�U���^�C�v
	atkTypes_[static_cast<int>(ATK_ACT::ATK)] = ATK_TYPE::NORMALATK;
	atkTypes_[static_cast<int>(ATK_ACT::SKILL1)] = ATK_TYPE::CHARGEATK;
	atkTypes_[static_cast<int>(ATK_ACT::SKILL2)] = ATK_TYPE::NORMALATK;
}

void PlAxe::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::UNIQUE_2, SPIN_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void PlAxe::ChargeAct(void)
{
	//chargeActUpdate_();
	auto& ins = InputManager::GetInstance();
	int skillOne = static_cast<int>(ATK_ACT::SKILL1);
	if (!isCool_[static_cast<int>(ATK_ACT::SKILL1)])
	{
		//�X�L��(�������ŃK�[�h��Ԉێ�)
		if (ins.IsNew(SKILL_KEY)&&IsAtkStart())
		{
			//�����Ă��锽��
			//CntUp(atkStartCnt_);
		}
		else if (ins.IsTrgUp(SKILL_KEY) && IsAtkStart())
		{
			if (atkStartCnt_ <= SKILL_ONE_START_NOCHARGE)
			{
				atkStartTime_[skillOne] = SKILL_ONE_START_NOCHARGE;
			}
			else
			{
				atkStartTime_[skillOne] = atkStartCnt_;
			}
		}
	}
}

void PlAxe::SkillOneInit(void)
{
	if (!IsAtkAction() && !isCool_[static_cast<int>(skillNo_)])
	{
		//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
		ChangeAct(static_cast<ATK_ACT>(skillNo_));
		ResetParam(atk_);
		CntUp(atkStartCnt_);
		moveAble_ = false;
	}

}

void PlAxe::SkillTwoInit(void)
{
	if (!IsAtkAction() && !isCool_[static_cast<int>(skillNo_)])
	{
		//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
		ChangeAct(static_cast<ATK_ACT>(skillNo_));
		ResetParam(atk_);
		CntUp(atkStartCnt_);
		moveAble_ = false;
	}
}

void PlAxe::AtkFunc(void)
{
	NmlAtkUpdate();
}

void PlAxe::Skill1Func(void)
{
	//�͗��߂đł����ނ��
	//moveAble_ = false;
	//�N�[���^�C���̏�����
	if (isCool_[static_cast<int>(skillNo_)])
	{
		return;
	}
	if (0.0f < atkStartCnt_ && atkStartCnt_ < atkStartTime_[static_cast<int>(act_)])
	{
		CntUp(atkStartCnt_);
		if (stepAnim_ >= SKILL_CHARGE_STEPANIM)
		{
			stepAnim_ = SKILL_CHARGE_STEPANIM;
		}
	}
	else if (atkStartCnt_ >= atkStartTime_[static_cast<int>(skillNo_)])
	{
		CntUp(atk_.cnt_);
		if (atk_.IsFinishMotion())
		{
			coolTime_[static_cast<int>(ATK_ACT::SKILL1)] = 0.0f;

			//�X�L���I�������U���������Ԃ̍ő厞�Ԃ��Z�b�g����
			atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;

			InitAtk();
		}
	}
}

void PlAxe::Skill2Func(void)
{
	NmlAtkUpdate();
	if (atk_.cnt_ >= SKILL2_CHANGE_ANIM_TIME)
	{
		if (stepAnim_ > 14.0f)ResetAnim(ANIM::UNIQUE_2, SPEED_ANIM_ATK);
		//��]���ړ��ł���
		moveAble_ = true;
		//�U�����W���ړ�������������
		SyncActPos(atk_);
		//������]�؂�
		if (atk_.isHit_)
		{
			CntUp(multiHitInterval_);
			//�N�[���^�C���̏�����
			coolTime_[static_cast<int>(act_)] = 0.0f;
			if (multiHitInterval_ >= MULTIHIT_INTERVAL)
			{
				atk_.isHit_ = false;
				multiHitInterval_ = 0.0f;
			}
		}
		else
		{
			multiHitInterval_ = 0.0f;
		}
	}
}