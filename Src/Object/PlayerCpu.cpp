#include"../Manager/ResourceManager.h"
#include "PlayerCpu.h"

void PlayerCpu::SetParam(void)
{
	atk_.pos_ = VAdd(trans_.pos, ATK_COL_LOCAL_POS);
	atk_.ResetCnt();
	atk_.duration_ = FRAME_ATK_DURATION;
	atk_.backlash_ = FRAME_ATK_BACKRASH;
	atk_.pow_ = 0;

	hp_ = MAX_HP;

	//ƒ‚ƒfƒ‹‚Ì‰Šú‰»
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));
	float scale = SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { 0.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);

	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
}

void PlayerCpu::Update(void)
{
	//‹¤’Ê
	Common();

	//’âŽ~ó‘Ô‚ÌŽž‚ÌƒAƒjƒ[ƒVƒ‡ƒ“
	if (!IsMove() && !IsDodge() && !IsAtkAction() && !IsSkillAction(SKILL_NUM::ONE))
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
		speedMove_ = 0.0f;
	}

	Action(atk_);

	//“G‚ð’Tõ‚·‚é
	Search();

	
}

void PlayerCpu::Search(void)
{
	//player_->GetPos();
	//Move(90.0f, AsoUtility::AXIS_Y);
}

