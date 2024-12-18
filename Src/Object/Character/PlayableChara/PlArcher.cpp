#include"../Manager/ResourceManager.h"
#include"../../Arrow.h"
#include "PlArcher.h"
PlArcher::PlArcher(const SceneManager::PLAY_MODE _mode, const SceneManager::CNTL _cntl) :PlayerBase(_mode, _cntl)
{
	mode_ = _mode;
	cntl_ = _cntl;
}
PlArcher::PlArcher(const SceneManager::PLAY_MODE _mode, const InputManager::JOYPAD_NO _padNum) : PlayerBase(_mode, _padNum)
{
	mode_ = _mode;
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

	atkStartRange_ = ATK_START_RANGE;
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
	//�U���J�E���g������
	atk_.ResetCnt();

	//�X�L�����I�������N�[���^�C���̃J�E���g�J�n
	isCool_[static_cast<int>(act_)] = true;

	//�ړ��\�ɂ���
	moveAble_ = true;

	//��������
	atk_.isHit_ = false;

	//�U���̔���
	atkStartCnt_ = 0.0f;


	isShotArrow_ = false;

}

void PlArcher::CreateArrow(void)
{
	//��̐�������
	for (auto& arrow : arrow_)
	{
		if (arrow->GetState() == Arrow::STATE::DESTROY)
		{
			//��̏����㏑��
			arrow = nullptr;
			arrow = std::make_shared<Arrow>();
			arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);

			//�J�E���g����
			arrowCnt_++;

			return;
		}
	}

	//�V�����z���ǉ�
	std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
	arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);

	//�z��Ɋi�[
	arrow_.emplace_back(arrow);

	//�J�E���g����
	arrowCnt_++;
}

void PlArcher::Update(void)
{
	PlayerBase::Update();

	size_t arrowSize = arrow_.size();

	//��Ɩ�ɑΉ������U���̍X�V
	for (int a = 0; a < arrowSize; a++)
	{
		//�U����Ԃ��I���������j��
		if (atk_.IsFinishMotion())
		{
			arrow_[a].get()->Destroy();
		}
			
		//�X�V
		arrow_[a].get()->Update(atk_);
	}
}

void PlArcher::Draw(void)
{
	PlayerBase::Draw();

	size_t arrowSize = arrow_.size();
	for (int s = 0; s < arrowSize; s++)
	{
		arrow_[s].get()->Draw();
	}
}

void PlArcher::AtkFunc(void)
{
	//��������A�[�`���[�쐬����I�I�I


	if (IsFinishAtkStart()&&!isShotArrow_)
	{
		CreateArrow();
		//������
		arrow_.back().get()->ChangeState(Arrow::STATE::SHOT);

		//��������
		isShotArrow_ = true;
	}
	
}

void PlArcher::Skill1Func(void)
{
}

void PlArcher::Skill2Func(void)
{
}
