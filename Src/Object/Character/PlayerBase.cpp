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
	
	isMove2CallPlayer_ = false;

	userOnePos_ = { -400.0f,0.0f,0.0f };

	act_ = ATK_ACT::MAX;

	calledMoveSpeed_ = 1.0f;

	atk_.isHit_ = false;

	isCall_ = false;

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






#ifdef DEBUG_ON
//���̏����͌�ŃL�[�{�[�h�̂Ƃ���ɒu���Ă���
	{
		auto& ins = InputManager::GetInstance();
		//CPU�������I�ɌĂяo��
		if (ins.IsTrgDown(KEY_INPUT_X))
		{
			isCall_ = true;
		}
		if (!isMove2CallPlayer_
			&& isCall_
			&& !AsoUtility::IsHitSpheres(trans_.pos, SEARCH_RANGE, userOnePos_, MY_COL_RADIUS)
			&& cpuState_ == CPU_STATE::BREAK)
		{
			isMove2CallPlayer_ = true;
			ChangeState(CPU_STATE::NORMAL);

			//�O�̍U���̏�����
			preAtk_ = ATK_ACT::MAX;

			//�Ăяo����Ĉړ�����Ƃ��̉����x
			calledMoveSpeed_ = CALLED_MOVE_SPEED_SCALE;

			//�v���C���[�ɗD��I�ɂ��Ă�����������G���@�m���Ȃ��悤�ɂ���
			isEnemySerch_ = false;

			//�^�[�Q�b�g���Ăяo���ꂽ�v���C���[�ɐݒ肷��
			SetTargetPos(userOnePos_);
		}
	}
	
#endif // DEBUG_ON

	//���ꂼ��̍X�V
	switch (mode_)
	{
	case SceneManager::PLAY_MODE::USER:
		UserUpdate();
		break;
	case SceneManager::PLAY_MODE::CPU:
		CpuUpdate();
		break;
	default:
		break;
	}
	//�A�N�V��������
	Action();

	//�X�L�����I�������N�[���^�C���̃J�E���g�J�n
	CoolTimeCnt();
	

#ifdef DEBUG_ON
	{
		auto& ins = InputManager::GetInstance();
		const float SPEED = 5.0f;
		if (ins.IsNew(KEY_INPUT_W)) { userOnePos_.z += SPEED; }
		if (ins.IsNew(KEY_INPUT_D)) { userOnePos_.x += SPEED; }
		if (ins.IsNew(KEY_INPUT_S)) { userOnePos_.z -= SPEED; }
		if (ins.IsNew(KEY_INPUT_A)) { userOnePos_.x -= SPEED; }



		// ���X�e�B�b�N�̉���
		leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;

		//�c��
		leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;
		auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));
		stickDeg_ = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(stickRad) + 90.0f));
		//�O
		if (leftStickY_ < -1){ userOnePos_.z += SPEED; }
		//�E
		else if (leftStickX_ > 1){ userOnePos_.x += SPEED; }
		//��
		else if (leftStickY_ > 1) { userOnePos_.z -= SPEED; }
		//��
		else if (leftStickX_ < -1){ userOnePos_.x -= SPEED; }

	}
	
#endif // DEBUG_ON

}
void PlayerBase::CpuUpdate(void)
{
	switch (cpuState_)
	{
	case PlayerBase::CPU_STATE::NORMAL:
		NmlUpdate();
		break;
	case PlayerBase::CPU_STATE::ATTACK:
		AtkUpdate();
		break;
	case PlayerBase::CPU_STATE::BREAK:
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
	if (!IsDodge() && !IsAtkAction())
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
	if (!IsMove() && !IsDodge() && !IsAtkAction())
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

	//���
	Dodge();

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
	animNum_.emplace(ANIM::DAMAGE, DAMAGE_NUM);
	animNum_.emplace(ANIM::DEATH, DEATH_NUM);
}

void PlayerBase::InitAct(void)
{
	
}

void PlayerBase::ChangeAtk(const ATK_ACT _act)
{
	act_ = _act;
	switch (act_)
	{
	case PlayerBase::ATK_ACT::ATK:
		//�A�j���[�V����
		ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);
		//�J�E���^�J�n
		break;
	case PlayerBase::ATK_ACT::SKILL1:
		//�A�j���[�V����
		ResetAnim(ANIM::SKILL_1, SPEED_ANIM_ATK);
		break;
	case PlayerBase::ATK_ACT::SKILL2:
		//�A�j���[�V����
		ResetAnim(ANIM::SKILL_2, SPEED_ANIM_ATK);
		break;

	}
	ResetParam(_act);
	SyncActPos(atk_.pos_);
	CntUp(atk_.cnt_);
}

void PlayerBase::ResetParam(ATK_ACT _act)
{
	atk_.radius_ = colRadius_[_act];
	atk_.pos_ = colLocalPos_[_act];
	atk_.backlash_ = backLashMax_[_act];
	atk_.duration_ = dulationMax_[_act];
	atk_.isHit_ = false;
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
	if (ins.IsTrgDown(KEY_INPUT_J) && !IsAtkAction())
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
		CntUp(dodgeCnt_);
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
		&&!IsAtkAction())
	{
		skillNo_ = static_cast<SKILL_NUM>(static_cast<int>(skillNo_) + 1);
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
		CntUp(dodgeCnt_);
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
	//DrawFormatString(0, 0, 0xffffff
	//	, "FrameATK(%f)\nisAtk(%d)\nisBackSrash(%d)\nDodge(%f)\nSkill(%f)\nSkillNum(%d)\nStick(%f)\nHP(%d)\nIsHit(%d)\nIsfollowPlayer(%d)\nAct(%d)\nCpuState(%d)"
	//	, atk_.cnt_, atk_.IsAttack(), atk_.IsBacklash()
	//	, dodgeCnt_, atk_.cnt_, skillNo_, stickDeg_, hp_,atk_.isHit_,isMove2CallPlayer_,act_,cpuState_);
	DrawFormatString(0, 32, 0xffffff
		, "AtkCooltime(%.2f)\nSkill1Cool(%.2f)\nSkill2Cool(%.2f)"
		, coolTime_[static_cast<int>(ATK_ACT::ATK)]
		, coolTime_[static_cast<int>(ATK_ACT::SKILL1)]
		, coolTime_[static_cast<int>(ATK_ACT::SKILL2)]);

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
	moveSpeed_ = SPEED_MOVE * calledMoveSpeed_;
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
	if ((atk_.IsAttack() || atk_.IsBacklash())&&!isCool_[static_cast<int>(act_)])
	{
		CntUp(atk_.cnt_);
		// �N�[���^�C���̏�����
		coolTime_[static_cast<int>(act_)] = 0.0f;
	}
	else /*if(atk_.IsFinishMotion())*/
	{
		//�U���J�E���g������
		atk_.ResetCnt();

		//�X�L�����I�������N�[���^�C���̃J�E���g�J�n
		isCool_[static_cast<int>(act_)] = true;

		//��������
		atk_.isHit_ = false;
	}
}


void PlayerBase::ChangeState(CPU_STATE _state)
{
	cpuState_ = _state;

	switch (cpuState_)
	{
	case CPU_STATE::NORMAL:
		break;
	case CPU_STATE::ATTACK:
		isCall_ = false;
		break;
	case CPU_STATE::BREAK:
		breakCnt_ = 0.0f;
		break;
	case CPU_STATE::MAX:
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
		CntUp(dodgeCnt_);
		//�X�L�����ɉ�����o�����ɃX�L���̃J�E���g�����Z�b�g
		atk_.ResetCnt();
		if (dodgeCnt_ < FRAME_DODGE_MAX)
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
		CntUp(dodgeCdt_);
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
	CntUp(atk_.cnt_);
}

void PlayerBase::Damage(void)
{
	//�Ƃ肠����1�_���[�W���炷
	hp_--;

	//�A�j���[�V�����ύX
	ResetAnim(ANIM::DAMAGE, SPEED_ANIM_DODGE);
#ifdef DEBUG_ON
	const unsigned int  DMG_COLOR = 0x00ffff;
	color_Col_ = DMG_COLOR;
#endif // DEBUG_ON
	if (!IsAlive()) { ResetAnim(ANIM::DEATH, SPEED_ANIM); }
	
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

void PlayerBase::CoolTimeCnt(void)
{
	if (act_ == ATK_ACT::MAX)return;
	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		if (isCool_[i])
		{
			CntUp(coolTime_[i]);
		}
		if (coolTime_[i] >= coolTimeMax_[i])
		{
			isCool_[i] = false;
			coolTime_[i] = coolTimeMax_[i];
		}
	}
}

void PlayerBase::NmlUpdate(void)
{
	auto& col = Collision::GetInstance();
	const float MOVE_DELAY_MAX=0.3f;

	//�ҋ@�A�j���[�V����
	//��~��Ԃ̎��̃A�j���[�V����
	if (!isMove_ && !IsDodge() && !IsAtkAction())
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

	//preAtk_ = ATK_ACT::MAX;

	if (!isEnemySerch_)
	{
		if (AsoUtility::IsHitSpheres(trans_.pos, FOLLOW_PLAYER_RADIUS, userOnePos_, FOLLOW_PLAYER_RADIUS))
		{
			//�Ăяo�����I������炻�ꂼ��̏�����
			isMove_ = false;
			isMove2CallPlayer_ = false;
			calledMoveSpeed_ = 1.0f;
			isCall_ = false;
			moveStartDelay_ = 0.0f;
		}
		else
		{
			//�v���C���[�̂Ƃ���܂œ���
			targetPos_ = userOnePos_;
			CntUp(moveStartDelay_);
			if (moveStartDelay_ >= MOVE_DELAY_MAX)
			{
				moveStartDelay_ = MOVE_DELAY_MAX;
				CpuMove(targetPos_);
			}
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
		ChangeState(CPU_STATE::BREAK);
		return;
	}
	//�O�̍U���������Ȃ�������A�U����ނ����߂�
	if (preAtk_ == ATK_ACT::MAX)
	{
		for (int i = ATK_TOTAL-1; i > -1; i--)
		{
			if (!isCool_[i])
			{
				ChangeAtk(static_cast<ATK_ACT>(i));
				preAtk_ = static_cast<ATK_ACT>(i);
				return;
			}
			else
			{
				continue;
			}
		}
		//ATK_ACT rand = static_cast<ATK_ACT>(GetRand(static_cast<int>(ATK_ACT::MAX) - 1));
		//preAtk_ = rand;
		
	}

}

void PlayerBase::BreakUpdate(void)
{
	const float BREAK_TIME = 2.0f;
	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
	if (breakCnt_ > BREAK_TIME)
	{
		//�u���C�N���I�������ʏ��Ԃֈڍs
		ChangeState(CPU_STATE::NORMAL);
		return;
	}
	CntUp(breakCnt_);
}