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
	frameAtk_ = FRAME_ATK_MAX;
	frameDodge_ = FRAME_DODGE_MAX;
	color_ = 0xffffff;

	//���f���̏�����
	transform_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	float scale = 0.5f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transform_.Update();
}

void PlayerBase::Update(void)
{
	//���f���̍X�V
	transform_.Update();
	//�R���g���[���n
	KeyBoardControl();

	//�U���������t���[�����肷��
	Attack();

	//���
	Dodge();

	
}

void PlayerBase::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void PlayerBase::Move(float _deg, VECTOR _axis)
{
	if (!IsAtk())
	{
		Turn(_deg, _axis);
		VECTOR dir = transform_.GetForward();
		//�ړ�����
		VECTOR movePow = VScale(dir, SPEED_MOVE);
		//�ړ�����
		transform_.pos = VAdd(transform_.pos, movePow);
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
	if (ins.IsTrgDown(KEY_INPUT_E)&&!IsAtk()){frameAtk_ = 0;}
	
	//���
	if (ins.IsTrgDown(KEY_INPUT_N)&&!IsDodge()&&!IsAtk())
	{
		frameDodge_ = 0;
	}

}

void PlayerBase::DrawDebug(void)
{
	//����
	DrawSphere3D(transform_.pos, 20.0f, 8, 0x0, color_, true);
	//�l����p
	DrawFormatString(0, 0, 0xffffff, "FrameATK(%d)\nisAtk(%d)", frameAtk_, IsAtk());
}

void PlayerBase::Turn(float _deg, VECTOR _axis)
{
	//transform_.quaRot =
	//	transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));

	transform_.quaRot =
		transform_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}


void PlayerBase::Attack(void)
{
	if (IsAtk())
	{
		frameAtk_++;
		color_ = 0xff0000;
	}
	else
	{
		color_ = 0xffffff;
	}
	
}

void PlayerBase::Dodge(void)
{
	if (IsDodge())
	{
		frameDodge_++;
		if (frameDodge_ < FRAME_DODGE_MAX)
		{
			VECTOR dir = transform_.GetForward();
			//�ړ�����
			VECTOR movePow = VScale(dir, SPEED_DODGE);
			//�ړ�����
			transform_.pos = VAdd(transform_.pos, movePow);
		}
	}
	else
	{
		dodgeCdt_ = 0;
		color_ = 0xffffff;
	}
}
