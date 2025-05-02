#include "./USER/PlAxeMan.h"
#include "AxeMan.h"
#ifdef DEBUG_ON
void AxeMan::DrawDebug(void)
{
	DrawFormatString(0, 0, 0x000000, "atkPos(%f)", atkPow_);
}

#endif // DEBUG_ON


AxeMan::AxeMan(void)
{
	
}

void AxeMan::SetParam(void)
{
	InitAct();

	//攻撃の初期化
	InitAtk();

	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));
	float scale = CHARACTER_SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { -300.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	auto& effIns = EffectManager::GetInstance();
	auto& resIns = ResourceManager::GetInstance();
	using EFFECT = EffectManager::EFFECT;

	//溜め攻撃エフェクトロード
	EffectManager::GetInstance().Add(EFFECT::CHARGE_AXE_HIT, 
		ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARGE_AXE_HIT).handleId_);

	//スキルチャージ(ロードするとなぜかエフェクトが出なくなるから念のためコメントアウト)
	EffectManager::GetInstance().Add(EFFECT::CHARGE_SKILL,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARGE_SKILL).handleId_);


	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);

	//ステータス関係
	ParamLoad(CharacterParamData::UNIT_TYPE::AXEMAN);

	//役職の割り当て
	role_ = SceneManager::ROLE::AXEMAN;

	

	moveAble_ = true;

	//当たり判定の設定
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

}
void AxeMan::InitAct(void)
{
	//通常攻撃の最大値
	atkMax_.emplace(ATK_ACT::ATK, ATK_MAX);

	//スキル１の最大値
	atkMax_.emplace(ATK_ACT::SKILL1, SKILL_ONE_MAX);

	//スキル２の最大値
	atkMax_.emplace(ATK_ACT::SKILL2, SKILL_TWO_MAX);


	//クールタイム
	coolTimeMax_[static_cast<int>(ATK_ACT::ATK)] = ATK_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_COOLTIME;

	//攻撃発生時間
	atkStartTime_[static_cast<int>(ATK_ACT::ATK)] = ATK_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_START;


}

void AxeMan::InitSkill(ATK_ACT _act)
{
	if (isAtk_ || isSkill_)return;
	//スキルごとにアニメーションを決めて、カウント開始
	ChangeAct(static_cast<ATK_ACT>(_act));
	ResetParam(atk_);
	CntUp(atkStartCnt_);
	moveAble_ = false;
	isSkill_ = true;
}

void AxeMan::Draw(void)
{
	PlayerBase::Draw();
#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON

	
}

void AxeMan::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::UNIQUE_2, SPIN_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void AxeMan::NmlAtkInit(void)
{
}

void AxeMan::SkillOneInit(void)
{

}

void AxeMan::SkillTwoInit(void)
{
	
}

void AxeMan::AtkFunc(void)
{
	if (!isAtk_)return;

	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
		if (IsFinishAtkStart())
		{
			CntUp(atk_.cnt_);
		}
	}
	else if (IsFinishAtkStart())
	{
		if ((atk_.IsAttack() || atk_.IsBacklash()))
		{
			CntUp(atk_.cnt_);
			//クールタイムの初期化
			coolTime_[static_cast<int>(act_)] = 0.0f;
		}
		if(atk_.IsFinishMotion())/*これつけると通常連打の時にバグる*/
		{
			InitAtk();
			isAtk_ = false;
		}
	}
}

void AxeMan::Skill1Func(void)
{
	auto& efeIns = EffectManager::GetInstance();
	if (isCool_[static_cast<int>(skillNo_)])
	{
		return;
	}
	if (0.0f < atkStartCnt_ && atkStartCnt_ < atkStartTime_[static_cast<int>(act_)])
	{
		if (atkStartCnt_ <= DELTATIME)
		{
			EffectManager::GetInstance().Play(
				EffectManager::EFFECT::CHARGE_SKILL,
				trans_.pos,
				Quaternion(),
				CHARGE_SKILL_EFF_SIZE,
				SoundManager::SOUND::NONE);
		}

		CntUp(atkStartCnt_);
		if (stepAnim_ >= SKILL_CHARGE_STEPANIM)
		{
			stepAnim_ = SKILL_CHARGE_STEPANIM;
		}
	}
	else if (atkStartCnt_ >= atkStartTime_[static_cast<int>(skillNo_)])
	{
		if (atkStartCnt_ > PlAxe::SKILL_ONE_START_NOCHARGE) { atk_.pow_ = SKILL_ONE_CHARGE_POW; }
		
		CntUp(atk_.cnt_);
		if (atk_.cnt_ <=DELTATIME)
		{
			EffectManager::GetInstance().Play(
				EffectManager::EFFECT::CHARGE_AXE_HIT,
				atk_.pos_,
				Quaternion(),
				CHARGE_AXE_EFF_SIZE,
				SoundManager::SOUND::NONE);
		}
		if (atk_.IsFinishMotion())
		{
			coolTime_[static_cast<int>(ATK_ACT::SKILL1)] = 0.0f;

			//efeIns.Stop(EffectManager::EFFECT::CHARGE_SKILL);

			//スキル終わったら攻撃発生時間の最大時間をセットする
			atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;

			InitAtk();
			isSkill_ = false;
		}
	}
}

void AxeMan::Skill2Func(void)
{
	if (isAtk_)return;

	if (!isSkill_)return;
	//近接攻撃用(atk_変数と遠距離で分ける)
	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
		if (IsFinishAtkStart())
		{
			CntUp(atk_.cnt_);
		}
	}
	else if (IsFinishAtkStart())
	{
		if ((atk_.IsAttack() || atk_.IsBacklash()))
		{
			CntUp(atk_.cnt_);
			//クールタイムの初期化
			coolTime_[static_cast<int>(act_)] = 0.0f;
		}
		else //if(atk_.IsFinishMotion())/*これつけると通常連打の時にバグる*/
		{
			InitAtk();
			isSkill_ = false;
		}
	}


	if (atk_.cnt_ >= SKILL2_CHANGE_ANIM_TIME)
	{
		if (stepAnim_ > 14.0f)ResetAnim(ANIM::UNIQUE_2, SPEED_ANIM_ATK);
		//回転中移動できる
		moveAble_ = true;
		//攻撃座標を移動中も同期する
		SyncActPos(atk_);
		//持続回転切り
		if (atk_.isHit_)
		{
			CntUp(multiHitInterval_);
			//クールタイムの初期化
			coolTime_[static_cast<int>(act_)] = 0.0f;
			if (multiHitInterval_ >= MULTIHIT_INTERVAL)
			{
				atk_.isHit_ = false;
				multiHitInterval_ = 0.0f;
			}
		}
		else
		{
			multiHitInterval_ = 0.0f;
		}
	}
}