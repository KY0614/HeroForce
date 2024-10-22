#include"../Manager/InputManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(void)
{
}

void PlayerBase::Destroy(void)
{
}

void PlayerBase::Init(void)
{
	dodgeCdt_ = DODGE_CDT_MAX;
	color_ = 0xffffff;

	
	
	speedMove_ = 0.0f;


	//���f���̏�����
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	float scale = 0.5f;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { 0.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	//�A�j���[�V����������
	animNum_[ANIM::IDLE] = IDLE_NUM;
	animNum_[ANIM::WALK] = WALK_NUM;
	animNum_[ANIM::DODGE] = DODGE_NUM;
	animNum_[ANIM::UNIQUE_1] = ATK_NUM;
	animNum_[ANIM::SKILL_1] = SKILL_NUM;
	ResetAnim(ANIM::IDLE);

	atk_.cnt_ = 0;
	atk_.duration_ = FRAME_ATK_DURATION;
	atk_.backlash_ = FRAME_ATK_BACKRASH;
	atk_.pow_ = 0;

	//�X�L��
	

	trans_.Update();

}

void PlayerBase::Update(void)
{
	//�A�j���[�V�����̍X�V
	Anim();
	//���f���̍X�V
	trans_.Update();

	//��~��Ԃ̎��̃A�j���[�V����
	if (!IsMove() && !IsDodge() && !IsAtkAction() && anim_ != ANIM::IDLE)
	{
		ResetAnim(ANIM::IDLE);
		speedMove_ = 0.0f;
	}

	//���
	Dodge();


	//�R���g���[���n
	KeyBoardControl();

	//�U���������t���[�����肷��
	Attack();





}

void PlayerBase::Draw(void)
{
	MV1DrawModel(trans_.modelId);
	DrawDebug();
}

void PlayerBase::Move(float _deg, VECTOR _axis)
{
	if (anim_!=ANIM::WALK&&!IsDodge()&&!IsAtkAction())
	{
		ResetAnim(ANIM::WALK);
		speedMove_ = SPEED_MOVE;
	}
	
	if (!atk_.IsAttack()&&!IsDodge() && !IsAtkAction())
	{
		Turn(_deg, _axis);
		VECTOR dir = trans_.GetForward();
		//�ړ�����
		VECTOR movePow = VScale(dir, speedMove_);
		//�ړ�����
		trans_.pos = VAdd(trans_.pos, movePow);
	}
}

void PlayerBase::KeyBoardControl(void)
{
	auto& ins = InputManager::GetInstance();
	//�O
	if (ins.IsNew(KEY_INPUT_W))
	{
		Move(0.0f, AsoUtility::AXIS_Y);
	}
	//�E
	else if (ins.IsNew(KEY_INPUT_D))
	{
		Move(90.0f, AsoUtility::AXIS_Y);
	}
	//��
	else if (ins.IsNew(KEY_INPUT_S))
	{
		Move(180.0f, AsoUtility::AXIS_Y);
	}
	//��
	else if (ins.IsNew(KEY_INPUT_A))
	{
		Move(270.0f, AsoUtility::AXIS_Y);
	}
	else
	{
		speedMove_ = 0.0f;
	}

	//�U���i�U���A�j���[�V�����̃t���[����0�ȉ���������t���[����ݒ�j
	if (ins.IsTrgDown(KEY_INPUT_E)/*&&!atk_.IsAttack()*/)
	{
		if (!atk_.IsAttack()&&!IsDodge()&&anim_!=ANIM::UNIQUE_1)
		{
			//�A�j���[�V����
			ResetAnim(ANIM::UNIQUE_1);
			//�J�E���^�J�n
			atk_.cnt_+=SceneManager::GetInstance().GetDeltaTime();
		}

	}

	if (ins.IsTrgDown(KEY_INPUT_Q)&&!IsAtkAction()&&!IsSkill(SKILL::TWO))
	{
		Skill_1();
		ResetAnim(ANIM::SKILL_1);
	}

	//���
	if (ins.IsTrgDown(KEY_INPUT_N) && !IsDodge() && !IsAtkAction()&&!IsCoolDodge())
	{
		ResetAnim(ANIM::DODGE);
		frameDodge_+=SceneManager::GetInstance().GetDeltaTime();
	}

}

void PlayerBase::DrawDebug(void)
{
	//����
	//DrawSphere3D(trans_.pos, 20.0f, 8, 0x0, color_, true);
	//�l����p
	DrawFormatString(0, 0, 0xffffff, "FrameATK(%f)\nisAtk(%d)\nisBackSrash(%d)\nDodge(%d)", atk_.cnt_, atk_.IsAttack(),atk_.IsBacklash_(), frameDodge_);
}



void PlayerBase::Turn(float _deg, VECTOR _axis)
{
	//transform_.quaRot =
	//	transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));

	trans_.quaRot =
		trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}


void PlayerBase::Attack(void)
{
	if (atk_.IsAttack()||atk_.IsBacklash_())
	{
		atk_.cnt_+=SceneManager::GetInstance().GetDeltaTime();
		color_ = 0xff0000;
	}
	else 
	{
		atk_.ResetCnt();
		color_ = 0xffffff;
	}
	
}

void PlayerBase::Dodge(void)
{
	//�h�b�W�t���O��true�ɂȂ�����
	if (IsDodge()&&!IsCoolDodge())
	{
		frameDodge_+=SceneManager::GetInstance().GetDeltaTime();
		if (frameDodge_ < FRAME_DODGE_MAX)
		{
			VECTOR dir = trans_.GetForward();
			//�ړ�����
			VECTOR movePow = VScale(dir, SPEED_DODGE);
			//�ړ�����
			trans_.pos = VAdd(trans_.pos, movePow);
		}
		else
		{
			dodgeCdt_ = 0.0f;
		}
	}
	else
	{
		dodgeCdt_ +=SceneManager::GetInstance().GetDeltaTime();
		ResetDodgeFrame();
		color_ = 0xffffff;
	}
}

void PlayerBase::Skill_1(void)
{
	skillNum_ = "Skill_1";
	skillCnt_[SKILL::ONE] += SceneManager::GetInstance().GetDeltaTime();
}

void PlayerBase::Skill_2(void)
{
	skillNum_ = "Skill_2";
}

