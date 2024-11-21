#include"../../../Manager/Resource.h"
#include"../../../Manager/ResourceManager.h"
#include"../../Arrow.h"
#include"../Enemy.h"
#include "EneArcher.h"

void EneArcher::SetParam(void)
{
	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_ARCHER));
	//arrowMdlId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::);	��TODO��̃��f��
	arrowMdlId_ = 0;

	//���X�Őݒ肷��
	radius_ = MY_COL_RADIUS;
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	hp_ = HP_MAX;
	atkPow_ = ATK_POW;
	def_ = DEF;
	arrowCnt_ = 0;
	isShotArrow_ = false;
	walkSpeed_ = WALK_SPEED;
	localCenterPos_ = LOCAL_CENTER_POS;
	stunDefMax_ = STUN_DEF_MAX;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
}

void EneArcher::InitAnimNum(void)
{
	//���ʃA�j���[�V����������
	Enemy::InitAnimNum();

	//�ŗL�A�j���[�V����������
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_AIMING);
	animNum_.emplace(ANIM::UNIQUE_2, ANIM_RELOAD);
}

void EneArcher::InitSkill(void)
{
	//�����ɃX�L���̐����i�[������
	skills_.emplace_back(SKILL_ONE);

	//�����ɃX�L���̐����A�j���[�V�������i�[������
	skillAnims_.emplace_back(ANIM::SKILL_1);

	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void EneArcher::Attack(void)
{
	//�����[�h���@���́@������������Ȃ�U�����Ȃ�
	if (IsReload() || isShotArrow_)return;

	//���݂̃X�L���̊m�F
	if (nowSkill_.front().radius_ == SKILL_ONE.radius_
		&& nowSkill_.front().backlash_ == SKILL_ONE.backlash_
		&& nowSkill_.front().duration_ == SKILL_ONE.duration_
		&& nowSkill_.front().pow_ == SKILL_ONE.pow_)
	{
		//�X�L��1����
		Skill_One();
	}


void EneArcher::Skill_One(void)
{
	//��𐶐�
	CreateArrow();

	//������
	arrow_.back().get()->ChangeState(Arrow::STATE::SHOT);

	//��������
	isShotArrow_ = true;
}

void EneArcher::CreateArrow(void)
{
	//��̐�������
	for (auto& arrow : arrow_)
	{
		if (arrow->GetState() == Arrow::STATE::DESTROY)
		{
			//��̏����㏑��
			arrow = nullptr;
			arrow = std::make_shared<Arrow>();
			arrow->Init(arrowMdlId_, trans_, 10.0f);

			//�J�E���g����
			arrowCnt_++;

			return;
		}
	}

	//�V�����z���ǉ�
	std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
	arrow->Init(arrowMdlId_, trans_, 10.0f);

	//�z��Ɋi�[
	arrow_.emplace_back(arrow);

	//�J�E���g����
	arrowCnt_++;
}

void EneArcher::ReloadArrow(void)
{
	//�����[�h���I�������J�E���^���Z�b�g
	if (reloadCnt_ >= RELOAD_TIME)
	{
		arrowCnt_ = 0;
		reloadCnt_ = 0.0f;
		return;
	}

	//�����[�h���ԃJ�E���g
	CntUp(reloadCnt_);

	//�����[�h�A�j���[�V����
	ResetAnim(ANIM::UNIQUE_2, SPEED_ANIM);
}

void EneArcher::FinishAnim(void)
{
	//���ʃA�j���[�V�����̏I������
	Enemy::FinishAnim();

	switch (anim_)
	{
	case UnitBase::ANIM::UNIQUE_1:
	case UnitBase::ANIM::UNIQUE_2:
		break;
	}
}

void EneArcher::InitChangeState(void)
{
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		break;

	case Enemy::STATE::ALERT:
		//���������߂Đݒ�
		trans_.quaRot = trans_.quaRot.LookRotation(GetMovePow2Target());

		//�ҋ@�A�j���[�V����
		ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM);

		//�x���J�E���^������
		alertCnt_ = 0.0f;
		break;

	case Enemy::STATE::ATTACK:
		break;

	case Enemy::STATE::BREAK:
		//�������������������
		isShotArrow_ = false;

		//�U���x�e���Ԃ̏�����
		breakCnt_ = 0;
		break;
	}
}

void EneArcher::Update(void)
{
	Enemy::Update();

	size_t arrowSize = arrow_.size();

	//��Ɩ�ɑΉ������U���̍X�V
	for (int a = 0 ; a < arrowSize ; a++)
	{
		//�U����Ԃ��I���������j��
		if (!nowSkill_[a].IsAttack())arrow_[a].get()->Destroy();

		//�X�V
		arrow_[a].get()->Update(nowSkill_[a]);
	}
}

void EneArcher::UpdateBreak(void)
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
	ResetAnim(ANIM::IDLE, SPEED_ANIM);

	//�����[�h
	if (IsReload())ReloadArrow();
	//�U���x�e���ԃJ�E���^
	else CntUp(breakCnt_);
}

void EneArcher::Draw(void)
{
	Enemy::Draw();

	size_t skillSize = nowSkill_.size();
	size_t arrowSize = arrow_.size();

	for (int s = 0; s < arrowSize; s++)
	{
		arrow_[s].get()->Draw();
	}
}
