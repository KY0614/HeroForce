#include"../Manager/ResourceManager.h"
#include"../../Arrow.h"
#include "PlArcher.h"
PlArcher::PlArcher(const SceneManager::CNTL _cntl) :PlayerBase(_cntl)
{
	cntl_ = _cntl;
}
PlArcher::PlArcher(const InputManager::JOYPAD_NO _padNum) : PlayerBase(_padNum)
{
	padNum_ = _padNum;
}

void PlArcher::SetParam(void)
{
	InitAct();

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

	hp_ = MAX_HP;

	moveAble_ = true;

	//�����蔻��̐ݒ�
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	atkAbleCnt_ = 0;
}

void PlArcher::InitAct(void)
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

void PlArcher::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void PlArcher::InitAtk(void)
{
	//size_t size = arrowAtk_.size();
	////�U���̏�����
	//for (int a = 0; a < size; a++)
	//{
	//	InitArrowAtk(arrowAtk_[a]);
	//}
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

void PlArcher::CreateArrow(void)
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

void PlArcher::CreateAtk(void)
{
	for (auto& atk : arrowAtk_)
	{
		if (!atk.IsAttack())
		{
			//atk������
			atk = atkMax_[act_];
			return;
		}
	}
	//�V�������ꍇ
	arrowAtk_.emplace_back(atkMax_[act_]);
}

void PlArcher::Update(void)
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



void PlArcher::Draw(void)
{
	PlayerBase::Draw();
	size_t arrowSize = arrow_.size();
	for (auto& arrow:arrow_)
	{
		arrow.get()->Draw();
	}
	DrawFormatString(300, 100, 0xffffff, "arrowSize(%d)", arrowSize);
}

void PlArcher::AtkFunc(void)
{
	//��������A�[�`���[�쐬����I
	if (IsFinishAtkStart()&&!isShotArrow_)
	{
		CreateArrow();
		CreateAtk();
		//������
		//for (auto& arrow : arrow_)
		//{
		//	arrow->ChangeState(Arrow::STATE::SHOT);
		//}
		// 
		
		//�͈�for���̈Ӗ�
		//for (int i = 0; i < arrow_.size; ++i)
		//{
		//	arrow_[i]->ChangeState(Arrow::STATE::SHOT)
		//}
		//��������
		isShotArrow_ = true;
	}
	NmlActCommon();
}

void PlArcher::Skill1Func(void)
{
	
}

void PlArcher::Skill2Func(void)
{

}

void PlArcher::NmlAtkInit(void)
{
	if (isCool_[static_cast<int>(ATK_ACT::ATK)])return;
	ChangeAct(ATK_ACT::ATK);
	ResetParam(atk_);
	CntUp(atkStartCnt_);
}

void PlArcher::SkillOneInit(void)
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

void PlArcher::SkillTwoInit(void)
{
}

void PlArcher::NmlActCommon(void)
{
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
		}
	}
}




void PlArcher::InitArrowAtk(ATK& arrowAtk)
{
	//�U���J�E���g������
	arrowAtk.ResetCnt();

	SyncActPos(arrowAtk);

	//��������
	arrowAtk.isHit_ = false;
}
