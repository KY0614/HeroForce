#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Generic/SceneManager.h"
//#include""
#include"../../Arrow.h"

#include "Archer.h"

#ifdef DEBUG_ON
void Archer::DrawDebug(void)
{
	DrawFormatString(0, 32, 0x000000, "arrowCnt(%d,%d)", arrowCnt_[0], arrowCnt_[1]);
	DrawSphere3D(trans_.pos, ARCHER_SEARCH_RANGE, 4, 0x0, 0xff0000, false);
	size_t arrowSize = arrow_.size();
	for (int i = 0; i < static_cast<int>(ATK_TYPE::MAX); i++)
	{
		for (auto& arrow : arrow_[static_cast<ATK_TYPE>(i)])
		{
			arrow.get()->Draw();
		}
	}
	//DrawFormatString(0, 0, 0x000000, "atkStartCnt(%f)", atkStartCnt_);
}
#endif // DEBUG_ON



Archer::Archer(void)
{

}

void Archer::SetParam(void)
{
	InitAct();
	//�v���C���[���f��
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
	ParamLoad(CharacterParamData::UNIT_TYPE::ARCHER);

	moveAble_ = true;

	//�����蔻��̐ݒ�
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	auto& effIns = EffectManager::GetInstance();
	auto& resIns = ResourceManager::GetInstance();
	using EFFECT = EffectManager::EFFECT;

	atkAbleCnt_ = 0;

	for (int i = 0; i < static_cast<int>(ATK_TYPE::MAX); i++)
	{
		arrowCnt_[i] = 0;
	}
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





void Archer::CreateArrow(ATK_TYPE _type)
{
	auto& efeIns = EffectManager::GetInstance();
	//��̐�������
	//�g���I������U��������ꍇ
	for (auto& arrow : arrow_[_type])
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
			arrowCnt_[static_cast<int>(_type)]++;
			return;
		}
	}


	//�V�������ꍇ
	//�V�����z���ǉ�
	std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
	arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);
	arrow->ChangeState(Arrow::STATE::SHOT);

	//�z��Ɋi�[
	arrow_[_type].emplace_back(arrow);

	//�J�E���g����
	arrowCnt_[static_cast<int>(_type)]++;
}





void Archer::CreateAtk(ATK_TYPE _type)
{
	
	for (auto& atk : arrowAtk_[_type])
	{
		if (atk.IsFinishMotion())
		{
			//atk������
			ResetParam(atk);
			return;
		}
	}
	//�V�������ꍇ
	arrowAtk_[_type].emplace_back(atkMax_[act_]);
}


const PlayerBase::ATK Archer::GetArrowAtk(const ATK_TYPE _type, const int i)
{
	return arrowAtk_[_type][i];
}

void Archer::SetIsArrowHit(ATK_TYPE _type, const bool _flg, int _num)
{
	arrowAtk_[_type][_num].isHit_ = true;
}

void Archer::Buff(PlayerBase& _target)
{
	//�O�̏����i�[����
	_target.SetPreStatus();

	//�o�t�����Z�b�g
	_target.SetBuff(BUFF_TYPE::ATK_BUFF, 0.2f, SKILL2_BUFF_TIME);
	_target.SetBuff(BUFF_TYPE::SPD_BUFF, 0.2f, SKILL2_BUFF_TIME);
	_target.SetBuff(BUFF_TYPE::DEF_BUFF, 0.2f, SKILL2_BUFF_TIME);
}


void Archer::Update(void)
{
	PlayerBase::Update();

	size_t arrowSize = arrow_.size();

	//for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	//{
	//	ArrowUpdate(static_cast<ATK_ACT>(i));
	//}
	ArrowUpdate(ATK_TYPE::ATTACK);
	ArrowUpdate(ATK_TYPE::BUFF);
	//ArrowUpdate(ATK_ACT::SKILL2);
}



void Archer::Draw(void)
{
	PlayerBase::Draw();

#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
}

void Archer::AtkFunc(void)
{
	if (isSkill_)return;
	//��������A�[�`���[�쐬����I
	if (IsFinishAtkStart() && !isShotArrow_)
	{
		CreateArrow(ATK_TYPE::ATTACK);
		CreateAtk(ATK_TYPE::ATTACK);
		//isShotArrow_ = true;
	}

	size_t arrowSize = arrow_.size();
	//�ߐڍU���p(atk_�ϐ��Ɖ������ŕ�����)
	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
		if (isSerchArcher_)
		{
			VECTOR targetVec = GetTargetVec(targetPos_, false);
			//��]
			trans_.quaRot = trans_.quaRot.LookRotation(targetVec);
		}
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

		if (!isShotArrow_)
		{
			CreateArrow(ATK_TYPE::ATTACK);
			CreateAtk(ATK_TYPE::ATTACK);
		}
		isShotArrow_ = true;
		CntUp(backrashCnt_);
		if (backrashCnt_ >= BACKRASH_MAX)
		{
			coolTime_[static_cast<int>(ATK_ACT::SKILL1)] = 0.0f;

			//�X�L���I�������U���������Ԃ̍ő厞�Ԃ��Z�b�g����
			atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;

			backrashCnt_ = 0.0f;
			InitAtk();
			isSkill_ = false;
		}
	}
}

void Archer::Skill2Func(void)
{
	if (isAtk_)return;
	//��������A�[�`���[�쐬����I
	if (IsFinishAtkStart() && !isShotArrow_)
	{
		CreateArrow(ATK_TYPE::BUFF);
		CreateAtk(ATK_TYPE::BUFF);
		//isShotArrow_ = true;
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
			isSkill_ = false;
		}
	}
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
			//ResetParam(arrowAtk_[s]);
			ResetParam(arrowAtk_[ATK_TYPE::ATTACK][s]);
			CntUp(atkStartCnt_);
		}
	}
}

void Archer::SkillTwoInit(void)
{

}
void Archer::ArrowUpdate(ATK_TYPE _type)
{
	size_t arrowSize = arrow_[_type].size();
	for (int a = 0; a < arrowSize; a++)
	{
		if (arrow_[_type][a].get()->GetIsAlive())
		{
			CntUp(arrowAtk_[_type][a].cnt_);
		}
		//�U����Ԃ��I���������j��
		if (arrowAtk_[_type][a].IsFinishMotion())
		{
			arrow_[_type][a].get()->Destroy();
			InitArrowAtk(arrowAtk_[_type][a]);

			//���݂��Ă��������炷
			arrowCnt_[static_cast<int>(_type)]--;
		}
		//�X�V
		arrow_[_type][a].get()->Update(arrowAtk_[_type][a]);
	}
}
void Archer::InitArrowAtk(ATK& arrowAtk)
{
	//�U���J�E���g������
	arrowAtk.ResetCnt();

	SyncActPos(arrowAtk);

	//��������
	arrowAtk.isHit_ = false;
}

