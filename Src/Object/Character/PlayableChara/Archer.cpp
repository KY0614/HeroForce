#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Generic/SceneManager.h"
//#include""
#include"../../Arrow.h"

#include "Archer.h"

Archer::Archer(void)
{
	
}

void Archer::SetParam(void)
{
	InitAct();
	arrowMdlId_ = -1;
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_ARCHER));
	float scale = CHARACTER_SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { -300.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);


	//�X�e�[�^�X�֌W
	hpMax_ = MAX_HP;
	atkPow_ = POW_ATK;
	def_ = MAX_DEF;

	moveAble_ = true;

	//�����蔻��̐ݒ�
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	atkAbleCnt_ = 0;
}

void Archer::InitAct(void)
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
	atkTypes_[static_cast<int>(ATK_ACT::SKILL2)] = ATK_TYPE::NORMALATK;
}

void Archer::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void Archer::InitAtk(void)
{
	//�X�L�����I�������N�[���^�C���̃J�E���g�J�n
	isCool_[static_cast<int>(act_)] = true;

	//�ړ��\�ɂ���
	moveAble_ = true;

	//�U���̔���
	atkStartCnt_ = 0.0f;

	isShotArrow_ = false;

	//�U���\����
	atkAbleCnt_ = 0.0f;

}

void Archer::CreateArrow(void)
{
	//��̐�������
	//�g���I������U��������ꍇ
	for (auto& arrow : arrow_)
	{
		// �j���Ԃ̂Ƃ�
		if (arrow->GetState() == Arrow::STATE::DESTROY)
		{
			//��̏����㏑��
			arrow = nullptr;

			// ����
			arrow = std::make_shared<Arrow>();

			// ������
			arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);

			arrow->ChangeState(Arrow::STATE::SHOT);

			//�J�E���g����
			arrowCnt_++;

			return;
		}
	}


	//�V�������ꍇ
	//�V�����z���ǉ�
	std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
	arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);
	arrow->ChangeState(Arrow::STATE::SHOT);

	//�z��Ɋi�[
	arrow_.emplace_back(arrow);

	//�J�E���g����
	arrowCnt_++;
}

void Archer::CreateAtk(void)
{
	for (auto& atk : arrowAtk_)
	{
		if (!atk.IsAttack())
		{
			//atk������
			ResetParam(atk);
			return;
		}
	}
	//�V�������ꍇ
	arrowAtk_.emplace_back(atkMax_[act_]);
}

void Archer::Update(void)
{
	PlayerBase::Update();

	size_t arrowSize = arrow_.size();
	//��Ɩ�ɑΉ������U���̍X�V
	for (int a = 0; a < arrowSize; a++)
	{
		if (arrow_[a].get()->GetIsAlive())
		{
			CntUp(arrowAtk_[a].cnt_);
		}
		//�U����Ԃ��I���������j��
		if (!arrowAtk_[a].IsAttack())
		{
			arrow_[a].get()->Destroy();
			InitArrowAtk(arrowAtk_[a]);
		}
		//�X�V
		arrow_[a].get()->Update(arrowAtk_[a]);
	}
}



void Archer::Draw(void)
{
	PlayerBase::Draw();
	size_t arrowSize = arrow_.size();
	for (auto& arrow : arrow_)
	{
		arrow.get()->Draw();
	}
	DrawFormatString(300, 100, 0xffffff, "arrowSize(%d)", arrowSize);
}

void Archer::AtkFunc(void)
{
	if (!isAtk_||isSkill_)return;
	//��������A�[�`���[�쐬����I
	if (IsFinishAtkStart() && !isShotArrow_)
	{
		CreateArrow();
		CreateAtk();
		isShotArrow_ = true;
	}

	size_t arrowSize = arrow_.size();
	//�ߐڍU���p(atk_�ϐ��Ɖ������ŕ�����)
	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
	}
	else if (IsFinishAtkStart())
	{
		CntUp(atkAbleCnt_);
		//�N�[���^�C���̏�����
		coolTime_[static_cast<int>(act_)] = 0.0f;
		if (atkAbleCnt_ >= ATKABLE_TIME)
		{
			InitAtk();
			isAtk_ = false;
		}
	}
}

void Archer::Skill1Func(void)
{
	if (isAtk_)return;
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

		CreateArrow();
		CreateAtk();
		coolTime_[static_cast<int>(
			ATK_ACT::SKILL1)] = 0.0f;

		//�X�L���I�������U���������Ԃ̍ő厞�Ԃ��Z�b�g����
		atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;

		InitAtk();
		isSkill_ = false;
		if (atk_.IsFinishMotion())
		{


		}
	}
}

void Archer::Skill2Func(void)
{

}

void Archer::NmlAtkInit(void)
{
	if (isCool_[static_cast<int>(ATK_ACT::ATK)])return;
	ChangeAct(ATK_ACT::ATK);
	CntUp(atkStartCnt_);
}

void Archer::SkillOneInit(void)
{
	size_t arrowSize = arrow_.size();
	for (int s = 0; s < arrowSize; s++)
	{
		if (IsSkillable() && !isCool_[static_cast<int>(skillNo_)])
		{
			//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
			ChangeAct(static_cast<ATK_ACT>(skillNo_));
			ResetParam(arrowAtk_[s]);
			CntUp(atkStartCnt_);
		}
	}
}

void Archer::SkillTwoInit(void)
{
}

void Archer::InitArrowAtk(ATK& arrowAtk)
{
	//�U���J�E���g������
	arrowAtk.ResetCnt();

	SyncActPos(arrowAtk);

	//��������
	arrowAtk.isHit_ = false;
}

