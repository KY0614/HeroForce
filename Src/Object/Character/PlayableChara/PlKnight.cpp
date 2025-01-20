#include"../Manager/ResourceManager.h"
#include"../Manager/SceneManager.h"
#include "PlKnight.h"


PlKnight::PlKnight(const SceneManager::CNTL _cntl) :PlayerBase(_cntl)
{
	cntl_ = _cntl;
}

PlKnight::PlKnight(const InputManager::JOYPAD_NO _padNum) :PlayerBase(_padNum)
{
	padNum_ = _padNum;
}


void PlKnight::SetParam(void)
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

void PlKnight::InitAct(void)
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


void PlKnight::AtkFunc(void)
{
	if (isSkill_)return;
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	//�ߐڍU���p
	if (ins.CheckAct(ACT_CNTL::NMLATK) && !isAtk_)
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


void PlKnight::SkillOneInit(void)
{
	//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
	ChangeAct(static_cast<ATK_ACT>(skillNo_));
	ResetParam(atk_);
	CntUp(atkStartCnt_);
}

void PlKnight::SkillTwoInit(void)
{
	if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] > GUARD_STARTABLE_COOL && !IsAtkStart())
	{
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = false;
		ChangeAct(static_cast<ATK_ACT>(skillNo_));
		ResetParam(atk_);
		coolTime_[static_cast<int>(SKILL_NUM::TWO)] -= SKILL_TWO_START_COOLTIME;
		atk_.duration_ = coolTime_[static_cast<int>(ATK_ACT::SKILL2)];
		//CntUp(atkStartCnt_);
		isSkill_ = true;
	}
}


void PlKnight::InitSkill(ATK_ACT _act)
{
	//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
	if (isAtk_ || isSkill_ || isCool_[static_cast<int>(_act)])return;
	ChangeAct(static_cast<ATK_ACT>(_act));
	ResetParam(atk_);
	CntUp(atkStartCnt_);
	moveAble_ = false;
	isSkill_ = true;
}

void PlKnight::Skill1Func(void)
{
	if (isAtk_)return;
	//�a����΂�
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	//����
	if (ins.CheckAct(ACT_CNTL::SKILL_DOWN))
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

void PlKnight::Skill2Func(void)
{
	if (isAtk_)return;
	//����
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	if (ins.CheckAct(ACT_CNTL::SKILL_DOWN))
	{
		//�{�^���̉����͂��߂̎��ɒl������
		SkillTwoInit();
	}
	//�X�L��(�������ŃK�[�h��Ԉێ�)
	if (ins.CheckAct(ACT_CNTL::SKILL_KEEP)&&isSkill_)
	{
		if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] > 0.0f)
		{
			moveAble_ = false;
			isCool_[static_cast<int>(SKILL_NUM::TWO)] = false;
			if (stepAnim_ >= 10.0f)
			{
				stepAnim_ = 10.0f;
			}
		}
	}
	else if (ins.CheckAct(ACT_CNTL::SKILL_UP) && isSkill_)
	{
		isPush_ = false;
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
		isSkill_ = false;
		InitAtk();
	}



	//�{�^�����������Ă���Ƃ��ɃN�[���^�C����0�b�ȉ��ɂȂ�����
	if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] <= 0.0f)
	{
		//�X�L���؂�ւ��o���Ȃ��悤�ɂ���
		isSkill_ = false;
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
		InitAtk();
		return;
	}

	if (isSkill_&&!IsFinishAtkStart())CntUp(atkStartCnt_);

	else if (isSkill_ && IsFinishAtkStart())
	{
		CntUp(atk_.cnt_);
		CntDown(coolTime_[static_cast<int>(SKILL_NUM::TWO)]);
	}
	//�{�^���������Ă��Ă��c��N�[���^�C����
	else if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] <= SKILL_TWO_START_COOLTIME)
	{
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
		InitAtk();
		isSkill_ = false;
		return;
	}
}

void PlKnight::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::UNIQUE_2, BLOCKING_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}


void PlKnight::DrawDebug(void)
{
	PlayerBase::DrawDebug();
	DrawFormatString(0, 200, 0xffffff
		, "AtkCooltime(%.2f)\nSkill1Cool(%.2f)\nSkill2Cool(%.2f)\natkDulation(%f)\natkCnt(%f)"
		, coolTime_[static_cast<int>(ATK_ACT::ATK)]
		, coolTime_[static_cast<int>(ATK_ACT::SKILL1)]
		, coolTime_[static_cast<int>(ATK_ACT::SKILL2)]
		, atk_.duration_
	,atkStartCnt_);
}