#include"../Manager/InputManager.h"
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
	
	frameDodge_ = FRAME_DODGE_MAX;
	dodgeCdt_ = DODGE_CDT_MAX;
	color_ = 0xffffff;

	atk_.cnt_ = FRAME_ATK_MAX;
	atk_.duration_ = FRAME_ATK_DURATION;
	atk_.pow_ = 0;




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
	trans_.Update();
}

void PlayerBase::Update(void)
{
	//���f���̍X�V
	trans_.Update();
	//�R���g���[���n
	KeyBoardControl();

	//�U���������t���[�����肷��
	Attack();

	//���
	Dodge();

	
}

void PlayerBase::Draw(void)
{
	MV1DrawModel(trans_.modelId);
	DrawDebug();
}

void PlayerBase::Move(float _deg, VECTOR _axis)
{
	if (!atk_.IsAttack()&&!IsDodge())
	{
		Turn(_deg, _axis);
		VECTOR dir = trans_.GetForward();
		//�ړ�����
		VECTOR movePow = VScale(dir, SPEED_MOVE);
		//�ړ�����
		trans_.pos = VAdd(trans_.pos, movePow);
	}
}

void PlayerBase::KeyBoardControl(void)
{
	auto& ins = InputManager::GetInstance();
	//�O
	if(ins.IsNew(KEY_INPUT_W))
	{
		Move(0.0f, AsoUtility::AXIS_Y);
	}

	//�E
	if (ins.IsNew(KEY_INPUT_D))
	{
		Move(90.0f, AsoUtility::AXIS_Y);
	}

	//��
	if(ins.IsNew(KEY_INPUT_S))
	{ 
		Move(180.0f, AsoUtility::AXIS_Y);
	}

	//��
	if(ins.IsNew(KEY_INPUT_A))
	{ 
		Move(270.0f, AsoUtility::AXIS_Y);
	}
	
	//�U���i�U���A�j���[�V�����̃t���[����0�ȉ���������t���[����ݒ�j
	if (ins.IsTrgDown(KEY_INPUT_E)/*&&!atk_.IsAttack()*/)
	{
		if (!atk_.IsAttack())
		{
			atk_.cnt_++;
		}
		
	}

	if (ins.IsNew(KEY_INPUT_Q))
	{ Skill_1(); }
	
	//���
	if (ins.IsTrgDown(KEY_INPUT_N)&&!IsDodge()&&!atk_.IsAttack()){ frameDodge_ = 0; }

}

void PlayerBase::DrawDebug(void)
{
	//����
	//DrawSphere3D(trans_.pos, 20.0f, 8, 0x0, color_, true);
	//�l����p
	DrawFormatString(0, 0, 0xffffff, "FrameATK(%f)\nisAtk(%d)", atk_.cnt_, atk_.IsAttack());
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
	if (atk_.IsAttack())
	{
		atk_.cnt_++;
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
		frameDodge_++;
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
			dodgeCdt_ = 0;
		}
	}
	else
	{
		dodgeCdt_ ++;
		color_ = 0xffffff;
	}
}

void PlayerBase::Skill_1(void)
{
	skillNum_ = "Skill_1";
}

void PlayerBase::Skill_2(void)
{
	skillNum_ = "Skill_2";
}

