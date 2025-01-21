#include "Knight.h"

Knight::Knight(const SceneManager::CNTL _cntl) :PlayerBase(_cntl)
{
	cntl_ = _cntl;
}

Knight::Knight(const InputManager::JOYPAD_NO _padNum) : PlayerBase(_padNum)
{
	padNum_ = _padNum;
}


void Knight::SetParam(void)
{
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	float scale = CHARACTER_SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { -300.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
}

void Knight::InitAct(void)
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
	atkTypes_[static_cast<int>(ATK_ACT::SKILL1)] = ATK_TYPE::NORMALATK;
	atkTypes_[static_cast<int>(ATK_ACT::SKILL2)] = ATK_TYPE::CHARGEATK;

}

void Knight::ChargeAct(void)
{
	//chargeActUpdate_();
}

void Knight::AtkFunc(void)
{
	if (isSkill_)return;
	auto& ins = InputManager::GetInstance();
	//�ߐڍU���p
	if (ins.IsTrgDown(ATK_KEY) && !isAtk_)
	{
		if (isCool_[static_cast<int>(ATK_ACT::ATK)])return;
		ChangeAct(ATK_ACT::ATK);
		ResetParam(atk_);
		CntUp(atkStartCnt_);
		isAtk_ = true;
	}
	if (!isAtk_)return;

	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
		if (IsFinishAtkStart())
		{
			CntUp(atk_.cnt_);
		}
	}
	else if (IsFinishAtkStart())
	{
		if ((atk_.IsAttack() || atk_.IsBacklash()))
		{
			CntUp(atk_.cnt_);
			//�N�[���^�C���̏�����
			coolTime_[static_cast<int>(act_)] = 0.0f;
		}
		else //if(atk_.IsFinishMotion())/*�������ƒʏ�A�ł̎��Ƀo�O��*/
		{
			InitAtk();
			isAtk_ = false;
		}
	}
}

//void PlKnight::ResetGuardCnt(void)
//{
//	if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] > GUARD_STARTABLE_COOL&&!IsAtkStart())
//	{
//		isCool_[static_cast<int>(SKILL_NUM::TWO)] = false;
//		ChangeAct(static_cast<ATK_ACT>(skillNo_));
//		ResetParam(atk_);
//		coolTime_[static_cast<int>(SKILL_NUM::TWO)] -= SKILL_TWO_START_COOLTIME;
//		//atkMax_[ATK_ACT::SKILL2].duration_ = coolTime_[static_cast<int>(ATK_ACT::SKILL2)];
//		atk_.duration_ = coolTime_[static_cast<int>(ATK_ACT::SKILL2)];
//		CntUp(atkStartCnt_);
//	}
//}

void Knight::SkillOneInit(void)
{
	//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
	ChangeAct(static_cast<ATK_ACT>(skillNo_));
	ResetParam(atk_);
	CntUp(atkStartCnt_);
}

void Knight::SkillTwoInit(void)
{
	if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] > GUARD_STARTABLE_COOL && !IsAtkStart())
	{
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = false;
		ChangeAct(static_cast<ATK_ACT>(skillNo_));
		ResetParam(atk_);
		coolTime_[static_cast<int>(SKILL_NUM::TWO)] -= SKILL_TWO_START_COOLTIME;
		atk_.duration_ = coolTime_[static_cast<int>(ATK_ACT::SKILL2)];
		CntUp(atkStartCnt_);
	}
}


void Knight::InitSkill(ATK_ACT _act)
{
	if (isAtk_ || isSkill_)return;
	//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
	ChangeAct(static_cast<ATK_ACT>(_act));
	ResetParam(atk_);
	CntUp(atkStartCnt_);
	moveAble_ = false;
	isSkill_ = true;
}

void Knight::Skill1Func(void)
{
	//�a����΂�
	auto& ins = InputManager::GetInstance();
	//����
	if (ins.IsTrgDown(SKILL_KEY))
	{
		InitSkill(skillNo_);
	}
	//�ߐڍU���p(atk_�ϐ��Ɖ������ŕ�����)
	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
		if (IsFinishAtkStart())
		{
			CntUp(atk_.cnt_);
		}
	}
	else if (IsFinishAtkStart())
	{
		if ((atk_.IsAttack() || atk_.IsBacklash()))
		{
			CntUp(atk_.cnt_);
			//�N�[���^�C���̏�����
			coolTime_[static_cast<int>(act_)] = 0.0f;
		}
		else //if(atk_.IsFinishMotion())/*�������ƒʏ�A�ł̎��Ƀo�O��*/
		{
			InitAtk();
			isSkill_ = false;
		}
	}

}

void Knight::Skill2Func(void)
{
	//����
	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(SKILL_KEY))
	{
		//�{�^���̉����͂��߂̎��ɒl������
		SkillTwoInit();
	}


	//�X�L��(�������ŃK�[�h��Ԉێ�)
	if (ins.IsNew(SKILL_KEY))
	{
		if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] > 0.0f)
		{
			moveAble_ = false;
			isCool_[static_cast<int>(SKILL_NUM::TWO)] = false;
			CntDown(coolTime_[static_cast<int>(SKILL_NUM::TWO)]);
			if (stepAnim_ >= 10.0f)
			{
				stepAnim_ = 10.0f;
			}
		}
	}
	else if (ins.IsTrgUp(SKILL_KEY))
	{
		isPush_ = false;
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
		InitAtk();
		actCntl_ = ACT_CNTL::NONE;
	}


	if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] <= 0.0f)
	{
		//�X�L���؂�ւ��o���Ȃ��悤�ɂ���
		return;
	}

	if (IsAtkStart())CntUp(atkStartCnt_);

	if (IsFinishAtkStart())
	{
		CntUp(atk_.cnt_);
	}
	if (atk_.IsAttack())
	{

	}

	//�{�^���������Ă��Ă��c��N�[���^�C����
	else if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] <= SKILL_TWO_START_COOLTIME)
	{
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
		InitAtk();
		return;
	}
}

void Knight::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::UNIQUE_2, BLOCKING_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}


void Knight::DrawDebug(void)
{

	DrawFormatString(0, 32, 0xffffff
		, "AtkCooltime(%.2f)\nSkill1Cool(%.2f)\nSkill2Cool(%.2f)\natkDulation(%f)\natkCnt(%f)"
		, coolTime_[static_cast<int>(ATK_ACT::ATK)]
		, coolTime_[static_cast<int>(ATK_ACT::SKILL1)]
		, coolTime_[static_cast<int>(ATK_ACT::SKILL2)]
		, atk_.duration_
		, atkStartCnt_);
}