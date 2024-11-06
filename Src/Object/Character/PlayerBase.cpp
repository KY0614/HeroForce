#include"../Manager/InputManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include "./PlayableChara/AxeMan.h"
#include "PlayerBase.h"


#define DEBUG_ON

//PlayerBase::PlayerBase(PLAY_MODE _mode, ROLE _role)
//{
//
//	
//
//
//
//}

void PlayerBase::Destroy(void)
{

}

void PlayerBase::SetParam(void)
{
	//acts_[ATK_ACT::ATK].pos_ = VAdd(trans_.pos, ATK_COL_LOCAL_POS);
	//acts_[ATK_ACT::ATK].ResetCnt();
	//acts_[ATK_ACT::ATK].duration_ = FRAME_ATK_DURATION;
	//acts_[ATK_ACT::ATK].backlash_ = FRAME_ATK_BACKRASH;
	//acts_[ATK_ACT::ATK].pow_ = 0;

	//trans_.SetModel(
	//	ResourceManager::GetInstance()
	//	.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	//float scale = CHARACTER_SCALE;
	//trans_.scl = { scale, scale, scale };
	//trans_.pos = { 0.0f, 0.0f, 0.0f };
	//trans_.quaRot = Quaternion();
	//trans_.quaRotLocal = Quaternion::Euler(
	//	0.0f, AsoUtility::Deg2RadF(180.0f),
	//	0.0f
	//);

	//hp_ = MAX_HP;

	////�����蔻��̐ݒ�
	//radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	
}

void PlayerBase::Init(void)
{
	SetParam();
#ifdef DEBUG_ON
	InitDebug();
#endif // DEBUG_ON

	switch (role_)
	{
	case PlayerBase::ROLE::KNIGHT:

		break;
	case PlayerBase::ROLE::AXEMAN:
		break;
	case PlayerBase::ROLE::MAGE:
		break;
	case PlayerBase::ROLE::ARCHER:
		break;
	default:
		break;
	}

	colPos_ = VAdd(trans_.pos, VScale(PLAYER_COL_LOCAL_POS, CHARACTER_SCALE));

	InitAct(ATK_ACT::ATK,FRAME_ATK_DURATION,FRAME_ATK_BACKRASH);
	InitAct(ATK_ACT::SKILL1,FRAME_SKILL1_DURATION, FRAME_SKILL1_BACKRASH);
	InitAct(ATK_ACT::SKILL2, FRAME_SKILL2_DURATION, FRAME_SKILL2_BACKRASH);



	skillNo_ = SKILL_NUM::ONE;

	dodgeCdt_ = DODGE_CDT_MAX;
	moveSpeed_ = 0.0f;
	ChangeControll(CNTL::KEYBOARD);

	//�A�j���[�V����������
	InitAnimNum();
	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);

	//���ꂼ��̍U�����W�̓���
	SyncActPos(acts_[ATK_ACT::ATK], ATK_COL_LOCAL_POS);
	SyncActPos(acts_[ATK_ACT::SKILL1], SKILL1_COL_LOCAL_POS);
	SyncActPos(acts_[ATK_ACT::SKILL2], SKILL2_COL_LOCAL_POS);

	//���f���̏�����
	trans_.Update();
}

void PlayerBase::Update(void)
{
	//���ʏ���
	switch (skillNo_)
	{
	case PlayerBase::SKILL_NUM::ONE:
		Action(ATK_ACT::SKILL1);
		break;
	case PlayerBase::SKILL_NUM::TWO:
		Action(ATK_ACT::SKILL2);
		break;
	default:
		break;
	}

	switch (mode_)
	{
	case PlayerBase::PLAY_MODE::USER:
		ModeUserUpdate();
		break;
	case PlayerBase::PLAY_MODE::CPU:
		CpuUpdate();
		break;
	default:
		break;
	}



	//�A�j���[�V�����̍X�V
	Anim();

	//���f���̍X�V
	trans_.Update();

	//��~��Ԃ̎��̃A�j���[�V����
	if (!IsMove() && !IsDodge() && !IsAtkAction() && !IsSkillAll())
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
		moveSpeed_ = 0.0f;
	}

	colPos_ = VAdd(trans_.pos, VScale(PLAYER_COL_LOCAL_POS, CHARACTER_SCALE));

	//���
	Dodge();

	Action(ATK_ACT::ATK);

	//�����蔻��̓���
	SyncActPos(acts_[ATK_ACT::ATK], ATK_COL_LOCAL_POS);
	SyncActPos(acts_[ATK_ACT::SKILL1], SKILL1_COL_LOCAL_POS);
	SyncActPos(acts_[ATK_ACT::SKILL2], SKILL2_COL_LOCAL_POS);
}

void PlayerBase::UserUpdate(void)
{
}

void PlayerBase::CpuUpdate(void)
{
	switch (state_)
	{
	case PlayerBase::STATE::NORMAL:
		NmlUpdate();
		break;
	case PlayerBase::STATE::ATTACK:
		AtkUpdate();
		break;
	case PlayerBase::STATE::BREAK:
		BreakUpdate();
		break;
	}
}

void PlayerBase::Draw(void)
{
	MV1DrawModel(trans_.modelId);
#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
	
}



void PlayerBase::Move(float _deg, VECTOR _axis)
{
	moveSpeed_ = SPEED_MOVE;
	if (!IsDodge() && !IsAtkAction()&&!IsSkillAll())
	{
		ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);
		Turn(_deg, _axis);
		VECTOR dir = trans_.GetForward();
		//�ړ�����
		VECTOR movePow = VScale(dir, moveSpeed_);
		//�ړ�����
		trans_.pos = VAdd(trans_.pos, movePow);
	}
}

void PlayerBase::ModeUserUpdate(void)
{
	//���ꂼ��̑���X�V
	switch (cntl_)
	{
	case PlayerBase::CNTL::KEYBOARD:
		KeyBoardControl();
		break;
	case PlayerBase::CNTL::PAD:
		GamePad();
		break;
	}
}

void PlayerBase::ModeCpuUpdate(void)
{

}

void PlayerBase::ActUpdate(ATK_ACT _act)
{

}

void PlayerBase::AtkFunc(void)
{

}

void PlayerBase::Skill1Func(void)
{

}

void PlayerBase::Skill2Func(void)
{

}

void PlayerBase::InitAnimNum(void)
{
	animNum_.emplace(ANIM::IDLE, IDLE_NUM);
	animNum_.emplace(ANIM::WALK, RUN_NUM);
	animNum_.emplace(ANIM::DODGE, DODGE_NUM);
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void PlayerBase::InitAct(ATK_ACT _act, float _dulation, float _backlash)
{
	acts_[_act].ResetCnt();
	acts_[_act].duration_ = _dulation;
	acts_[_act].backlash_ = _backlash;
	acts_[_act].pow_ = 0;
	acts_[_act].isHit_ = false;
}

void PlayerBase::KeyBoardControl(void)
{
	auto& ins = InputManager::GetInstance();
	DINPUT_JOYSTATE input;
	if (GetJoypadInputState(DX_INPUT_PAD1)&&CheckHitKeyAll() < 0)
	{
		ChangeControll(CNTL::PAD);
	}
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
		moveSpeed_ = 0.0f;
	}
	

	
	

	//�U���i�U���A�j���[�V�����̃t���[����0�ȉ���������t���[����ݒ�j
	if (ins.IsTrgDown(KEY_INPUT_E) &&IsAtkable())
	{
		//�A�j���[�V����
		ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);
		//�J�E���^�J�n
		Count(acts_[ATK_ACT::ATK].cnt_);
	}

	if (ins.IsTrgDown(KEY_INPUT_Q)&&IsSkillable())
	{
		Count(acts_[static_cast<ATK_ACT>(skillNo_)].cnt_);
		ResetAnim(ANIM::SKILL_1,SPEED_ANIM);
	}

	//���
	if (ins.IsTrgDown(KEY_INPUT_N) &&IsSkillable())
	{
		ResetAnim(ANIM::DODGE,SPEED_ANIM_DODGE);
		Count(frameDodge_);
	}


}

void PlayerBase::GamePad(void)
{
	auto& ins = InputManager::GetInstance();
	if (GetJoypadNum() == 0)
	{
		ChangeControll(CNTL::KEYBOARD);
	}

	// ���X�e�B�b�N�̉���
	leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;

	//�c��
	leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;

	//��������
	auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));
	stickDeg_ = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(stickRad) + 90.0f));
	//�O
	if (leftStickY_ < -1)
	{
		Move(stickDeg_, AsoUtility::AXIS_Y);
	}
	//�E
	else if (leftStickX_ > 1)
	{
		Move(stickDeg_, AsoUtility::AXIS_Y);
	}
	//��
	else if (leftStickY_ > 1)
	{
		Move(stickDeg_, AsoUtility::AXIS_Y);
	}
	//��
	else if (leftStickX_ < -1)
	{
		Move(stickDeg_, AsoUtility::AXIS_Y);
	}
	else
	{
		moveSpeed_ = 0.0f;
	}

	//�U���i�U���A�j���[�V�����̃t���[����0�ȉ���������t���[����ݒ�j
	if ( ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1,InputManager::JOYPAD_BTN::RIGHT) 
		&& IsAtkable())
	{
		//�A�j���[�V����
		ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);
		//�J�E���^�J�n
		Count(acts_[ATK_ACT::ATK].cnt_);
	}

	//�X�L��
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::TOP) 
		&& IsSkillable())
	{
		//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
		SkillAnim();
	}

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_BUTTON)
		&&!IsSkillAll())
	{
		skillNo_ = static_cast<SKILL_NUM>(static_cast<int>(skillNo_)+1);
		if (skillNo_ == SKILL_NUM::MAX)
		{
			skillNo_ = SKILL_NUM::ONE;
		}
	}

	//���
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::LEFT) 
		&&IsDodgeable())
	{
		ResetAnim(ANIM::DODGE, SPEED_ANIM_DODGE);
		Count(frameDodge_);
	}
}

void PlayerBase::ChangeControll(const CNTL _cntl)
{
	//if (cntl_ == _cntl) { return; }
	cntl_ = _cntl;
	switch (cntl_)
	{
	case PlayerBase::CNTL::KEYBOARD:
		break;
	case PlayerBase::CNTL::PAD:
		break;
	}
}

void PlayerBase::Common(void)
{

	
}

void PlayerBase::DrawDebug(void)
{
	const unsigned int ATK_COLOR = 0xff0000;
	//����
	//DrawSphere3D(trans_.pos, 20.0f, 8, 0x0, color_, true);
	//�l����p
	DrawFormatString(0, 0, 0xffffff
		, "FrameATK(%f)\nisAtk(%d)\nisBackSrash(%d)\nDodge(%f)\nSkill(%f)\nSkillNum(%d)\nStick(%f)\nHP(%d)"
		, acts_[ATK_ACT::ATK].cnt_, acts_[ATK_ACT::ATK].IsAttack(), acts_[ATK_ACT::ATK].IsBacklash()
		, frameDodge_,acts_[ATK_ACT::SKILL1].cnt_, skillNo_, stickDeg_, hp_);

	DrawSphere3D(colPos_, CHARACTER_SCALE * 100, 8, color_Col_, color_Col_, false);
	DrawSphere3D(acts_[ATK_ACT::ATK].pos_, COL_ATK, 8, color_Atk_, color_Atk_, false);
	DrawSphere3D(acts_[ATK_ACT::SKILL1].pos_, COL_SKILL1, 8, color_skl1_, color_skl1_, false);
	DrawSphere3D(acts_[ATK_ACT::SKILL2].pos_, COL_SKILL2, 8, color_skl2_, color_skl2_, false);

	if (acts_[ATK_ACT::ATK].IsAttack())
	{
		color_Atk_ = ATK_COLOR;
	}
	else
	{
		color_Atk_ = 0x00ffff;
	}

	if (acts_[ATK_ACT::SKILL1].IsAttack())
	{
		color_skl1_ = ATK_COLOR;
	}
	else
	{
		color_skl1_ = 0x00ffff;
	}

	if (acts_[ATK_ACT::SKILL2].IsAttack())
	{
		color_skl2_ = ATK_COLOR;
	}
	else
	{
		color_skl2_ = 0x00ffff;
	}
}



void PlayerBase::Turn(float _deg, VECTOR _axis)
{
	//transform_.quaRot =
	//	transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));

	trans_.quaRot =
		trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}


void PlayerBase::Action(ATK_ACT _act)
{
	if (acts_[_act].IsAttack() || acts_[_act].IsBacklash())
	{
		Count(acts_[_act].cnt_);
	}
	else
	{
		acts_[_act].ResetCnt();

		//��������
		acts_[_act].isHit_ = false;
	}
}


void PlayerBase::ChangeState(STATE _state)
{
	state_ = _state;
	switch (state_)
	{
	case STATE::NORMAL:
		break;
	case STATE::ATTACK:
		break;
	case STATE::BREAK:
		break;
	case STATE::MAX:
		break;
	default:
		break;
	}
}

void PlayerBase::SyncActPos(ATK& _act,const VECTOR _colPos)
{
	//�Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = trans_.pos;

	//�Ǐ]�Ώۂ̌���
	Quaternion followRot = trans_.rot;

	//�Ǐ]�Ώۂ̊p�x
	VECTOR relativeActPos = followRot.PosAxis(VScale(_colPos,CHARACTER_SCALE));
	_act.pos_ = VAdd(trans_.pos, relativeActPos);
}


void PlayerBase::Dodge(void)
{
	//�h�b�W�t���O��true�ɂȂ�����
	if (IsDodge()&&!IsCoolDodge())
	{
		Count(frameDodge_);
		//�X�L�����ɉ�����o�����ɃX�L���̃J�E���g�����Z�b�g
		acts_[ATK_ACT::SKILL1].ResetCnt();
		acts_[ATK_ACT::SKILL2].ResetCnt();
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
		Count(dodgeCdt_);
		ResetDodgeFrame();
#ifdef DEBUG_ON
		color_Col_ = 0xffffff;
#endif // DEBUG_ON

		
	}
}

void PlayerBase::Skill_1(void)
{
}

void PlayerBase::Skill_2(void)
{
}

void PlayerBase::SkillAnim(void)
{
	switch (skillNo_)
	{
	case PlayerBase::SKILL_NUM::ONE:
		ResetAnim(ANIM::SKILL_1, SPEED_ANIM);
		break;
	case PlayerBase::SKILL_NUM::TWO:
		ResetAnim(ANIM::SKILL_2, SPEED_ANIM_ATK);
		break;
	case PlayerBase::SKILL_NUM::MAX:
		break;
	default:
		break;
	}
	Count(acts_[static_cast<ATK_ACT>(skillNo_)].cnt_);
}

void PlayerBase::Damage(void)
{
	hp_--;
#ifdef DEBUG_ON
	const unsigned int  DMG_COLOR = 0x00ffff;
	color_Col_ = DMG_COLOR;
#endif // DEBUG_ON

	
}

void PlayerBase::InitDebug(void)
{
	color_Col_ = 0xffffff;
	color_Atk_ = 0x00ffff;
	color_skl1_ = 0x00ffff;
	color_skl2_ = 0x00ffff;
}





//CPU
//------------------------------------------------
void PlayerBase::RandAct(void)
{
}

void PlayerBase::NmlUpdate(void)
{
	//�ҋ@�A�j���[�V����
	if (moveSpeed_ == 0.0)ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
	//�����A�j���[�V����
	else if (moveSpeed_ > 0.0f)ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);

	//�ړ��ʂ̏�����
	moveSpeed_ = 0.0f;
}

void PlayerBase::AtkUpdate(void)
{
	//�U�����I����Ă���Ȃ��ԑJ��
	if (atk_.IsFinishMotion())
	{
		//�x�e��ԂɑJ��
		ChangeState(STATE::BREAK);
		return;
	}
	ATK_ACT rand = static_cast<ATK_ACT>(GetRand(static_cast<int>(ATK_ACT::MAX)));

	switch (rand)
	{
	case PlayerBase::ATK_ACT::ATK:
		Action(ATK_ACT::ATK);
		break;
	case PlayerBase::ATK_ACT::SKILL1:
		Action(ATK_ACT::SKILL1);
		break;
	case PlayerBase::ATK_ACT::SKILL2:
		Action(ATK_ACT::SKILL2);
		break;
	default:
		break;
	}


	//�U���A�j���[�V����
	ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);


	//�U���̃J�E���^
//nowSkill.cnt_++;

//�U������
//Attack();
}

void PlayerBase::BreakUpdate(void)
{
}