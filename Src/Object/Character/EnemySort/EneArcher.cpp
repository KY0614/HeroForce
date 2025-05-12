#include"../../Arrow.h"
#include"Enemy.h"
#include "EneArcher.h"

EneArcher::EneArcher(const VECTOR& _spawnPos) : Enemy(_spawnPos)
{
	trans_.pos = _spawnPos;

	arrowMdlId_ = -1;
	isShotArrow_ = false;
	arrowCnt_ = 0;
	reloadCnt_ = 0.0f;
	lastArrow_ = nullptr;
}

void EneArcher::Destroy(void)
{
	//���ʉ��
	Enemy::Destroy();

	//�|�C���^�̉��
	lastArrow_ = nullptr;
	delete lastArrow_;
}

void EneArcher::SetParam(void)
{
	//�U���x��
	alertSkills_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneArcher::AlertSkill_One, this));

	//�U���̑J��
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneArcher::Skill_One, this));

	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_ARCHER));
	arrowMdlId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ARROW);	

	//���X�Őݒ肷��ϐ�
	trans_.scl = { CHARACTER_SCALE,CHARACTER_SCALE,CHARACTER_SCALE };
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	arrowCnt_ = 0;
	isShotArrow_ = false;
	localCenterPos_ = LOCAL_CENTER_POS;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;

	//�O������X�e�[�^�X���擾
	ParamLoad(CharacterParamData::UNIT_TYPE::E_ARCHER);
}

void EneArcher::InitAnim(void)
{
	//���ʃA�j���[�V����������
	Enemy::InitAnim();

#pragma region �ŗL�A�j���[�V����������

	//�X�L��1
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
	//�G�C��
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_AIMING);
	//�����[�h
	animNum_.emplace(ANIM::UNIQUE_2, ANIM_RELOAD);

#pragma endregion

#pragma region �A�j���[�V�������x�ݒ�

	//�X�L��1
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM);
	//�G�C��
	changeSpeedAnim_.emplace(ANIM::UNIQUE_1, SPEED_ANIM);
	//�����[�h
	changeSpeedAnim_.emplace(ANIM::UNIQUE_2, SPEED_ANIM_RELOAD);

#pragma endregion

	//�A�j���[�V�������Z�b�g
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneArcher::InitSkill(void)
{
#pragma region �X�L���̊i�[

	//�X�L��1
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);

#pragma endregion

#pragma region �X�L���̗\������A�j���[�V�����i�[

	//�X�L��1
	skillPreAnims_.emplace_back(ANIM::UNIQUE_1);

#pragma endregion

#pragma region �X�L���̓���A�j���[�V����

	//�X�L��1
	skillAnims_.emplace_back(ANIM::SKILL_1);

#pragma endregion

	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void EneArcher::AlertSkill_One(void)
{
	//�G�̑O��
	VECTOR pos = trans_.GetForward();

	//�U���͈�
	pos = VScale(pos, SKILL_ONE_COL_RADIUS);

	//���W���킹
	pos = VAdd(trans_.pos, VScale(pos, ARROW_SPEED * SKILL_ONE_DURATION));

	//�͈͍쐬
	CreateAlert(pos, SKILL_ONE_COL_RADIUS * 2, SKILL_ONE_COL_RADIUS * 2 * ARROW_SPEED * SKILL_ONE_DURATION);
}

void EneArcher::Skill_One(void)
{
	//��������
	if (isShotArrow_)
	{
		//�U���̏I������
		if (!lastArrow_->GetIsAlive() && lastAtk_->IsFinishMotion())
		{
			//�U���I��
			isEndAllAtk_ = true;

			//�����I��
			return;
		}
	}

	//�����[�h���@���́@������������Ȃ�U�����Ȃ�
	if (IsReload() || isShotArrow_)return;

	//��𐶐�
	Arrow& arrow = CreateArrow();

	//�Ō�ɐ������ꂽ����i�[
	lastArrow_ = &arrow;

	//�����ɍU��������
	lastAtk_ = &createSkill_();

	//������
	arrow.ChangeState(Arrow::STATE::SHOT);

	//��������
	isShotArrow_ = true;
}

Arrow& EneArcher::CreateArrow(void)
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

			return *arrow;
		}
	}

	//�V�����z���ǉ�
	std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
	arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);

	//�z��Ɋi�[
	arrow_.emplace_back(arrow);

	//�J�E���g����
	arrowCnt_++;

	return *arrow_.back();
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
	ResetAnim(ANIM::UNIQUE_2, changeSpeedAnim_[ANIM::UNIQUE_2]);
}

void EneArcher::FinishAnim(void)
{
	//���ʃA�j���[�V�����̏I������
	Enemy::FinishAnim();

	switch (anim_)
	{
	case UnitBase::ANIM::UNIQUE_1:
		break;
	case UnitBase::ANIM::UNIQUE_2:
		//���[�v�Đ�
		stepAnim_ = 0.0f;		
		break;
	}
}

void EneArcher::ResetAtkJudge(void)
{
	//����
	Enemy::ResetAtkJudge();

	//�������������������
	isShotArrow_ = false;

}

void EneArcher::ChangeStateAtk(void)
{
	//�X�V�����̒��g������
	stateUpdate_ = std::bind(&EneArcher::UpdateAtk, this);
}

void EneArcher::Update(void)
{
	Enemy::Update();

	size_t arrowSize = arrow_.size();

	//��Ɩ�ɑΉ������U���̍X�V
	for (int a = 0 ; a < arrowSize ; a++)
	{
		//�X�V
		arrow_[a].get()->Update(nowSkill_[a]);

		//�U����Ԃ��I���������j��
		if (!nowSkill_[a].IsAttack())arrow_[a].get()->Destroy();
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
		
	//�����[�h
	if (IsReload())
	{
		ReloadArrow();
		return;
	}
	//�U���x�e���ԃJ�E���^
	else CntUp(breakCnt_);

	//�ҋ@�A�j���[�V����
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
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
