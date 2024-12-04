#include"../Manager/ResourceManager.h"
#include "PlAxeMan.h"

PlAxe::PlAxe(SceneManager::PLAY_MODE _mode, InputManager::JOYPAD_NO _padNum) : PlayerBase(_mode, _padNum)
{
	mode_ = _mode;
	padNum_ = _padNum;
}

void PlAxe::SetParam(void)
{
	InitAct();

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

	////�͈�
	//colRadius_.emplace(ATK_ACT::ATK, COL_ATK);
	//colRadius_.emplace(ATK_ACT::SKILL1, COL_SKILL1);
	//colRadius_.emplace(ATK_ACT::SKILL2, COL_SKILL2);

	////���W
	//colLocalPos_.emplace(ATK_ACT::ATK, ATK_COL_LOCAL_POS);
	//colLocalPos_.emplace(ATK_ACT::SKILL1, SKILL1_COL_LOCAL_POS);
	//colLocalPos_.emplace(ATK_ACT::SKILL2, SKILL2_COL_LOCAL_POS);

	////��������
	//dulationMax_.emplace(ATK_ACT::ATK, FRAME_ATK_DURATION);
	//dulationMax_.emplace(ATK_ACT::SKILL1, FRAME_SKILL1_DURATION);
	//dulationMax_.emplace(ATK_ACT::SKILL2, FRAME_SKILL2_DURATION);

	////�㌄
	//backLashMax_.emplace(ATK_ACT::ATK, FRAME_ATK_BACKRASH);
	//backLashMax_.emplace(ATK_ACT::SKILL1, FRAME_SKILL1_BACKRASH);
	//backLashMax_.emplace(ATK_ACT::SKILL2, FRAME_SKILL2_BACKRASH);

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
	atkTypes_[static_cast<int>(ATK_ACT::SKILL2)] = ATK_TYPE::NORMALATK;
}

void PlAxe::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::UNIQUE_2, SPIN_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void PlAxe::AtkFunc(void)
{
	moveAble_ = false;
}

void PlAxe::Skill1Func(void)
{
	//�͗��߂đł����ނ��
	moveAble_ = false;
	//�N�[���^�C���̏�����
	//coolTime_[static_cast<int>(act_)] = 0.0f;
	if (IsAtkStart())
	{
		if (stepAnim_ >= 16.9f)
		{
			stepAnim_ = 16.9f;
		}
	}
}

void PlAxe::Skill2Func(void)
{
	if (atk_.cnt_ >= SKILL2_CHANGE_ANIM_TIME)
	{
		if (stepAnim_ > 14.0f)ResetAnim(ANIM::UNIQUE_2, SPEED_ANIM_ATK);
		//��]���ړ��ł���
		moveAble_ = true;
		//�U�����W���ړ�������������
		SyncActPos(atk_.pos_);
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
	else
	{
		//��]���ړ��ł���
		moveAble_ = false;
	}

}