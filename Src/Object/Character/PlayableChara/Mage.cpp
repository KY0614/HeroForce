#include "Mage.h"

Mage::Mage(void)
{
}

void Mage::SetParam(void)
{
	//ステータス関係
//------------------------------------------
	hpMax_ = HP_MAX;
	atkPow_ = POW_ATK;
	def_ = DEF_MAX;
	defSpeed_ = SPEED;

	//モデル
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGE));
	float scale = CHARACTER_SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { -300.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	//ステータス
	ParamLoad(CharacterParamData::UNIT_TYPE::MAGE);

	//役職の割り当て
	role_ = SceneManager::ROLE::MAGE;

	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
}

void Mage::AtkFunc(void)
{
}

void Mage::Skill1Func(void)
{
}

void Mage::Skill2Func(void)
{
}

void Mage::NmlAtkInit(void)
{
}

void Mage::SkillOneInit(void)
{
}

void Mage::SkillTwoInit(void)
{
}
