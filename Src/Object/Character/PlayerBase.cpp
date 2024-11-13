#include"../Manager/InputManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/Collision.h"
#include "./PlayableChara/PlAxeMan.h"
#include "PlayerBase.h"

void PlayerBase::Destroy(void)
{

}

void PlayerBase::SetParam(void)
{
	
}

void PlayerBase::Init(void)
{
	//�A�j���[�V����������
	InitAnimNum();
	ResetAnim(ANIM::NONE, SPEED_ANIM);
	SetParam();
#ifdef DEBUG_ON
	InitDebug();
#endif // DEBUG_ON
	skillNo_ = SKILL_NUM::ONE;

	dodgeCdt_ = DODGE_CDT_MAX;
	moveSpeed_ = 0.0f;
	ChangeControll(SceneManager::CNTL::KEYBOARD);

	searchRange_ = SEARCH_RANGE;

	preAtk_ = ATK_ACT::MAX;

	isEnemySerch_ = false;
	
	//ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);

	//���ꂼ��̍U�����W�̓���
	//SyncActPos(acts_[ATK_ACT::ATK], ATK_COL_LOCAL_POS);
	//SyncActPos(acts_[ATK_ACT::SKILL1], SKILL1_COL_LOCAL_POS);
	//SyncActPos(acts_[ATK_ACT::SKILL2], SKILL2_COL_LOCAL_POS);

	userOnePos_ = { -400.0f,0.0f,0.0f };

	act_ = ATK_ACT::MAX;

	//���f���̏�����
	trans_.Update();
}

void PlayerBase::Update(void)
{
	//�A�j���[�V�����̍X�V
	Anim();

	//���f���̍X�V
	trans_.Update();

	colPos_ = VAdd(trans_.pos, VScale(PLAYER_COL_LOCAL_POS, CHARACTER_SCALE));
	//�����蔻��̓���
	//SyncActPos(atk_.pos_);
	//SyncActPos(acts_[ATK_ACT::SKILL1], SKILL1_COL_LOCAL_POS);
	//SyncActPos(acts_[ATK_ACT::SKILL2], SKILL2_COL_LOCAL_POS);

	switch (mode_)
	{
	case SceneManager::PLAY_MODE::USER:
		ModeUserUpdate();
		break;
	case SceneManager::PLAY_MODE::CPU:
		CpuUpdate();
		break;
	default:
		break;
	}

	

#ifdef DEBUG_ON
	auto& ins=InputManager::GetInstance();
	const float SPEED = 7.0f;
	if (ins.IsNew(KEY_INPUT_W)) { userOnePos_.z += SPEED; }
	if (ins.IsNew(KEY_INPUT_D)) { userOnePos_.x += SPEED; }
	if (ins.IsNew(KEY_INPUT_S)) { userOnePos_.z -= SPEED; }
	if (ins.IsNew(KEY_INPUT_A)) { userOnePos_.x -= SPEED; }
#endif // DEBUG_ON

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

void PlayerBase::UserUpdate(void)
{
	//��~��Ԃ̎��̃A�j���[�V����
	if (!IsMove() && !IsDodge() && !IsAtkAction() && !IsSkillAll())
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
		moveSpeed_ = 0.0f;
	}
	//���ꂼ��̑���X�V
	switch (cntl_)
	{
	case  SceneManager::CNTL::KEYBOARD:
		KeyBoardControl();
		break;
	case  SceneManager::CNTL::PAD:
		GamePad();
		break;
	}

	////���ʏ���
	//switch (skillNo_)
	//{
	//case PlayerBase::SKILL_NUM::ONE:
	//	ChangeAtk(ATK_ACT::SKILL1);
	//	break;
	//case PlayerBase::SKILL_NUM::TWO:
	//	ChangeAtk(ATK_ACT::SKILL2);
	//	break;
	//default:
	//	break;
	//}

	//switch (act_)
	//{
	//case PlayerBase::ATK_ACT::ATK:
	//	//�A�j���[�V����
	//	ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);
	//	//�J�E���^�J�n
	//	break;
	//case PlayerBase::ATK_ACT::SKILL1:
	//	//�A�j���[�V����
	//	ResetAnim(ANIM::SKILL_1, SPEED_ANIM_ATK);
	//	break;
	//case PlayerBase::ATK_ACT::SKILL2:
	//	//�A�j���[�V����
	//	ResetAnim(ANIM::SKILL_2, SPEED_ANIM_ATK);
	//	break;
	//}

	//���
	Dodge();

	Action();

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
	animNum_.emplace(ANIM::NONE, T_POSE_NUM);
	animNum_.emplace(ANIM::IDLE, IDLE_NUM);
	animNum_.emplace(ANIM::WALK, RUN_NUM);
	animNum_.emplace(ANIM::DODGE, DODGE_NUM);
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void PlayerBase::InitAct(ATK_ACT _act, float _dulation, float _backlash)
{
	//acts_[_act].ResetCnt();
	//acts_[_act].duration_ = _dulation;
	//acts_[_act].backlash_ = _backlash;
	//acts_[_act].pow_ = 0;
	//acts_[_act].isHit_ = false;
}

void PlayerBase::ChangeAtk(const ATK_ACT _act)
{
	act_ = _act;
	switch (act_)
	{
	case PlayerBase::ATK_ACT::ATK:
		//�A�j���[�V����
		ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);
		atk_.radius_ = COL_ATK;
		atk_.pos_ = ATK_COL_LOCAL_POS;
		atk_.duration_ = FRAME_ATK_DURATION;
		atk_.backlash_ = FRAME_ATK_BACKRASH;
		atk_.pow_ = 4;
		atk_.isHit_ = false;
		//�J�E���^�J�n
		break;
	case PlayerBase::ATK_ACT::SKILL1:
		//�A�j���[�V����
		ResetAnim(ANIM::SKILL_1, SPEED_ANIM_ATK);
		atk_.radius_ = COL_SKILL1;
		atk_.pos_ = SKILL1_COL_LOCAL_POS;
		atk_.duration_ = FRAME_SKILL1_DURATION;
		atk_.backlash_ = FRAME_SKILL1_BACKRASH;
		atk_.pow_ = 4;
		atk_.isHit_ = false;
		break;
	case PlayerBase::ATK_ACT::SKILL2:
		//�A�j���[�V����
		ResetAnim(ANIM::SKILL_2, SPEED_ANIM_ATK);
		atk_.radius_ = COL_SKILL2;
		atk_.pos_ = SKILL2_COL_LOCAL_POS;
		atk_.duration_ = FRAME_SKILL2_DURATION;
		atk_.backlash_ = FRAME_SKILL2_BACKRASH;
		atk_.pow_ = 4;
		atk_.isHit_ = false;
		break;

	}
	SyncActPos(atk_.pos_);
	Count(atk_.cnt_);
}

void PlayerBase::KeyBoardControl(void)
{
	auto& ins = InputManager::GetInstance();
	DINPUT_JOYSTATE input;
	if (GetJoypadInputState(DX_INPUT_PAD1)&&CheckHitKeyAll() < 0)
	{
		ChangeControll(SceneManager::CNTL::PAD);
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
		ChangeAtk(ATK_ACT::ATK);
	}

	//�X�L���؂�ւ�
	if (ins.IsTrgDown(KEY_INPUT_J && !IsSkillAll()))
	{
		skillNo_ = static_cast<SKILL_NUM>(static_cast<int>(skillNo_) + 1);
		if (skillNo_ == SKILL_NUM::MAX)
		{
			skillNo_ = SKILL_NUM::ONE;
		}
	}
		
	if (ins.IsTrgDown(KEY_INPUT_Q)&&IsSkillable())
	{
		ChangeAtk(static_cast<ATK_ACT>(skillNo_));
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
		ChangeControll(SceneManager::CNTL::KEYBOARD);
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
		ChangeAtk(ATK_ACT::ATK);
	}

	//�X�L��
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::TOP) 
		&& IsSkillable())
	{
		//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
		ChangeAtk(static_cast<ATK_ACT>(skillNo_));
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

void PlayerBase::ChangeControll(SceneManager::CNTL _cntl)
{
	cntl_ = _cntl;
	switch (cntl_)
	{
	case SceneManager::CNTL::KEYBOARD:
		break;
	case SceneManager::CNTL::PAD:
		break;
	}
}

void PlayerBase::Common(void)
{

	
}
#ifdef DEBUG_ON
void PlayerBase::DrawDebug(void)
{
	const unsigned int ATK_COLOR = 0xff0000;
	//����
	DrawSphere3D(trans_.pos, 20.0f, 8, 0x0, 0xff0000, true);
	//�l����p
	DrawFormatString(0, 0, 0xffffff
		, "FrameATK(%f)\nisAtk(%d)\nisBackSrash(%d)\nDodge(%f)\nSkill(%f)\nSkillNum(%d)\nStick(%f)\nHP(%d)"
		, atk_.cnt_, atk_.IsAttack(), atk_.IsBacklash()
		, frameDodge_, atk_.cnt_, skillNo_, stickDeg_, hp_);

	DrawSphere3D(colPos_, CHARACTER_SCALE * 100, 8, color_Col_, color_Col_, false);
	DrawSphere3D(atk_.pos_, atk_.radius_, 8, color_Atk_, color_Atk_, false);
	//DrawSphere3D(acts_[ATK_ACT::SKILL1].pos_, COL_SKILL1, 8, color_skl1_, color_skl1_, false);
	//DrawSphere3D(acts_[ATK_ACT::SKILL2].pos_, COL_SKILL2, 8, color_skl2_, color_skl2_, false);

	//�v���C���[�̓����蔻��
	DrawSphere3D(colPos_, radius_, 4, 0xffff00, 0xffff00, false);
	//�v���C���[�̍��G����
	DrawSphere3D(trans_.pos, searchRange_, 2, isMove_ ? 0xff0000 : 0xffffff, isMove_ ? 0xff0000 : 0xffffff, false);
	//�v���C���[�̍��G����
	DrawSphere3D(userOnePos_, 20, 2, 0x0000ff, 0xffffff, false);


	if (atk_.IsAttack())
	{
		color_Atk_ = ATK_COLOR;
	}
	else
	{
		color_Atk_ = 0x00ffff;
	}

	if (atk_.IsAttack())
	{
		color_skl1_ = ATK_COLOR;
	}
	else
	{
		color_skl1_ = 0x00ffff;
	}

	if (atk_.IsAttack())
	{
		color_skl2_ = ATK_COLOR;
	}
	else
	{
		color_skl2_ = 0x00ffff;
	}
}
#endif // DEBUG_ON

VECTOR PlayerBase::GetTargetVec(VECTOR _targetPos)
{
	//�W�I�ւ̕����x�N�g�����擾						��TODO:�x�N�g����SceneGame������炤
	VECTOR targetVec = VSub(_targetPos, trans_.pos);

	//���K��
	targetVec = VNorm(targetVec);

	//Y���W�͕K�v�Ȃ��̂ŗv�f������
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//�ړ��ʂ����߂�
	VECTOR ret = VScale(targetVec, moveSpeed_);

	return ret;
}



void PlayerBase::CpuMove(VECTOR _targetPos)
{
	//�ړ����x�̍X�V
	moveSpeed_ = SPEED_MOVE;
	isMove_ = true;
	//ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);

	//�����x�N�g���擾
	VECTOR targetVec = GetTargetVec(_targetPos);

	//��]
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);

	//�ړ�
	trans_.pos = VAdd(trans_.pos, targetVec);
}

void PlayerBase::Turn(float _deg, VECTOR _axis)
{
	//transform_.quaRot =
	//	transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));

	trans_.quaRot =
		trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}


void PlayerBase::Action(void)
{
	if (atk_.IsAttack() || atk_.IsBacklash())
	{
		Count(atk_.cnt_);
	}
	else
	{
		atk_.ResetCnt();

		//�U����Ԃ̃��Z�b�g
		act_ = ATK_ACT::MAX;
		//��������
		atk_.isHit_ = false;
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
		breakCnt_ = 0.0f;
		break;
	case STATE::MAX:
		break;
	default:
		break;
	}
}

void PlayerBase::SyncActPos(VECTOR _localPos)
{
	//�Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = trans_.pos;

	//�Ǐ]�Ώۂ̌���
	Quaternion followRot = trans_.quaRot;

	//�Ǐ]�Ώۂ̊p�x
	VECTOR relativeActPos = followRot.PosAxis(VScale(_localPos,CHARACTER_SCALE));
	atk_.pos_ = VAdd(trans_.pos, relativeActPos);
}


void PlayerBase::Dodge(void)
{
	//�h�b�W�t���O��true�ɂȂ�����
	if (IsDodge()&&!IsCoolDodge())
	{
		Count(frameDodge_);
		//�X�L�����ɉ�����o�����ɃX�L���̃J�E���g�����Z�b�g
		atk_.ResetCnt();
		atk_.ResetCnt();
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

void PlayerBase::Skill_One(void)
{
}

void PlayerBase::Skill_Two(void)
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
	Count(atk_.cnt_);
}

void PlayerBase::Damage(void)
{
	hp_--;
#ifdef DEBUG_ON
	const unsigned int  DMG_COLOR = 0x00ffff;
	color_Col_ = DMG_COLOR;
#endif // DEBUG_ON

	
}

#ifdef DEBUG_ON
void PlayerBase::InitDebug(void)
{
	color_Col_ = 0xffffff;
	color_Atk_ = 0x00ffff;
	color_skl1_ = 0x00ffff;
	color_skl2_ = 0x00ffff;


}
#endif // DEBUG_ON




//CPU
//------------------------------------------------
void PlayerBase::RandAct(void)
{
}

void PlayerBase::NmlUpdate(void)
{
	auto& col = Collision::GetInstance();


	//�Q�[���V�[����SetIsMove�ɂ���Ă�����false�ɂȂ��Ă�����A�Q�[���V�[���Ńv���C���[��Ǐ]���G�l�~�[��Ǐ]���邩�����߂�I�I���񂿁I



	//�ҋ@�A�j���[�V����
	//��~��Ԃ̎��̃A�j���[�V����
	if (!isMove_ && !IsDodge() && !IsAtkAction() && !IsSkillAll())
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
		moveSpeed_ = 0.0f;
	}
	//�����A�j���[�V����
	else
	{
		ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);
	}

	//�ړ��ʂ̏�����
	moveSpeed_ = 0.0f;

	preAtk_ = ATK_ACT::MAX;

	if (!isEnemySerch_)
	{
		if (AsoUtility::IsHitSpheres(trans_.pos, FOLLOW_PLAYER_RADIUS, userOnePos_, FOLLOW_PLAYER_RADIUS))
		{
			isMove_ = false;
		}
		else
		{
			targetPos_ = userOnePos_;
			CpuMove(targetPos_);
		}
	}
	else
	{
		//�G���@�m������A�Q�[���V�[������SetTargetPos�̒��ɓG�̍��W������
		//����Ɍ����ē�����
		CpuMove(targetPos_);
	}
	
	
}

void PlayerBase::AtkUpdate(void)
{
	//�U�����I����Ă���Ȃ��ԑJ��
	if (atk_.IsFinishMotion())
	{
		//�O�̍U���̏�����
		preAtk_ = ATK_ACT::MAX;

		//�x�e��ԂɑJ��
		ChangeState(STATE::BREAK);
		return;
	}
	if (preAtk_ == ATK_ACT::MAX)
	{
		ATK_ACT rand = static_cast<ATK_ACT>(GetRand(static_cast<int>(ATK_ACT::MAX) - 1));
		switch (rand)
		{
		case PlayerBase::ATK_ACT::ATK:
			if (IsAtkable())
			{
				ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);
				//Action(ATK_ACT::ATK);
			}
			break;
		case PlayerBase::ATK_ACT::SKILL1:
			if (IsSkillable())
			{
				ResetAnim(ANIM::SKILL_1, SPEED_ANIM_ATK);
				//Action(ATK_ACT::SKILL1);
			}
			break;
		case PlayerBase::ATK_ACT::SKILL2:
			if (IsSkillable())
			{
				ResetAnim(ANIM::SKILL_2, SPEED_ANIM_ATK);
				//Action(ATK_ACT::SKILL2);
			}
			break;
		default:
			break;
		}

		preAtk_ = rand;
		ChangeAtk(preAtk_);
	}
	ChangeAtk(preAtk_);

}

void PlayerBase::BreakUpdate(void)
{
	float deltaTIme= SceneManager::GetInstance().GetDeltaTime();
	const float BREAK_TIME = 2.0f;
	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
	if (breakCnt_ > BREAK_TIME)
	{
		ChangeState(STATE::NORMAL);
		return;
	}
	breakCnt_ += deltaTIme;
}