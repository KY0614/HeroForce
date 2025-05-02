#include"../PlayerInput.h"
#include"../PlayerDodge.h"
#include"../../../Manager/Generic/InputManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(void)
{
	skillNo_ = ATK_ACT::SKILL1;

	moveSpeed_ = 0.0f;

	userOnePos_ = { -400.0f,0.0f,0.0f };

	atk_.isHit_ = false;

	multiHitInterval_ = 0.0f;

	atkStartCnt_ = 0.0f;

	isPush_ = false;

	dodge_ = nullptr;

	moveAble_ = true;

	stickDeg_ = -1;

	isSerchArcher_ = false;

	isBuffing_ = false;

	bufAtk_ = atkPow_;
	bufDef_ = defDef_;
	bufSpd_ = defSpeed_;

	speed_ = 0.0f;

	initPos_[0] = PLAYER1_POS;
	initPos_[1] = PLAYER2_POS;
	initPos_[2] = PLAYER3_POS;
	initPos_[3] = PLAYER4_POS;

	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		coolTime_[i] = coolTimeMax_[i];
		coolTimePer_[i] = 1.0f;
	}
}

void PlayerBase::Destroy(void)
{
	delete dodge_;
	dodge_ = nullptr;

	EffectManager::GetInstance().Stop(EffectManager::EFFECT::GUARD);
	EffectManager::GetInstance().Stop(EffectManager::EFFECT::CHARGE_SKILL);
	EffectManager::GetInstance().Stop(EffectManager::EFFECT::CHARGE_AXE_HIT);
	EffectManager::GetInstance().Stop(EffectManager::EFFECT::ARCHER_SKILL2);
	
	SoundManager::GetInstance().Stop(SoundManager::SOUND::SKILL_CHANGE);
}



void PlayerBase::Init(void)
{
	//アニメーションNo初期化
	InitAnimNum();


	InitCharaAnim();

	SetParam();

	InitAct();

#ifdef DEBUG_ON
	InitDebug();
#endif // DEBUG_ON
	//関数ポインタ初期化
	changeAct_.emplace(ATK_ACT::ATK, std::bind(&PlayerBase::ChangeNmlAtk, this));
	changeAct_.emplace(ATK_ACT::SKILL1, std::bind(&PlayerBase::ChangeSkillOne, this));
	changeAct_.emplace(ATK_ACT::SKILL2, std::bind(&PlayerBase::ChangeSkillTwo, this));

	radius_ = MY_COL_RADIUS;

	skillNo_ = ATK_ACT::SKILL1;

	

	//dodgeCdt_ = DODGE_CDT_MAX;
	dodge_ = new PlayerDodge();
	dodge_->Init();

	//攻撃の初期化(とりあえず通常攻撃で初期化しておく)
	ChangeAct(ATK_ACT::ATK);

	ChangeSkillControll(ATK_ACT::SKILL1);

	speed_ = 0.0f;

	userOnePos_ = { -400.0f,0.0f,0.0f };


	atk_.isHit_ = false;

	multiHitInterval_ = 0.0f;

	atkStartCnt_ = 0.0f;

	isPush_ = false;

	hp_ = hpMax_;

	preAtkPow_ = atkPow_;
	preDef_ = def_;
	preSpd_ = moveSpeed_;

	//バフ加算
	buffpers_.emplace(STATUSBUFF_TYPE::ATK_BUFF, 0.0f);
	buffpers_.emplace(STATUSBUFF_TYPE::DEF_BUFF, 0.0f);
	buffpers_.emplace(STATUSBUFF_TYPE::SPD_BUFF, 0.0f);

	//各バフの初期化
	buffs_.emplace(SKILL_BUFF::BUFF_ARROW, (0.0f, 0.0f, 0.0f));
	buffs_.emplace(SKILL_BUFF::GUARD, (0.0f, 0.0f, 0.0f));
	
	auto& snd = SoundManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::SKILL_CHANGE
		, res.Load(ResourceManager::SRC::SKILL_CHANGE).handleId_);

	//プレイヤー入力のインスタンスを作る
	PlayerInput::CreateInstance();

	//モデルの初期化
	trans_.Update();

	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		coolTime_[i] = coolTimeMax_[i];
	}

	for (int i = 0; i < static_cast<int>(STATUSBUFF_TYPE::MAX); i++)
	{
		buffPercent_[i] = 1.0f;
	}
}

void PlayerBase::Update(void)
{
	//アニメーションの更新
	Anim();
	if (!IsAlive())
	{
		ResetAnim(ANIM::DEATH, SPEED_ANIM);
		if (stepAnim_ >= DEATH_STEP_ANIM) { stepAnim_ = DEATH_STEP_ANIM; }
		return;
	}
#ifdef INPUT_DEBUG_ON
	InputUpdate();
#endif // INPUT_DEBUG_ON


	//座標のバックアップ
	prePos_ = trans_.pos;

	//モデルの更新
	trans_.Update();

	SyncActPos(atk_);

	colPos_ = VAdd(trans_.pos, VScale(PLAYER_COL_LOCAL_POS, CHARACTER_SCALE));

	UserUpdate();

	//クールタイム割合の計算
	CoolTimePerCalc();

	//HPを減らす処理
	SubHp();

	BuffUpdate();


#ifdef DEBUG_ON


#endif // DEBUG_ON
	//スキルが終わったらクールタイムのカウント開始
	CoolTimeCnt();
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
	if (!isAtk_&&!isSkill_)
	{
		ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);
	}
	if (!dodge_->IsDodge() && moveAble_)
	{
		speed_ = moveSpeed_;
		Turn(_deg, _axis);
		VECTOR dir = trans_.GetForward();
		//移動方向
		VECTOR movePow = VScale(dir, speed_);
		//移動処理
		trans_.pos = VAdd(trans_.pos, movePow);
	}
}

void PlayerBase::UserUpdate(void)
{

	//停止アニメーションになる条件
	if (!IsMove() && !dodge_->IsDodge() && 0.0f >= atkStartCnt_ &&!isAtk_&&!isSkill_)
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
		speed_ = 0.0f;
	}

	//操作関係
	ProcessAct();

	auto& inpMng = InputManager::GetInstance();
	auto& pInp = PlayerInput::GetInstance();

	//回避
	dodge_->Update(trans_);
	if (dodge_->IsDodge() && !dodge_->IsCoolDodge()) {
		atk_.ResetCnt();
		atkStartCnt_ = 0.0f;
		isAtk_ = false;
		isSkill_ = false;
		moveAble_ = true;
	}
}


void PlayerBase::InitAnimNum(void)
{
	animNum_.emplace(ANIM::NONE, T_POSE_NUM);
	animNum_.emplace(ANIM::IDLE, IDLE_NUM);
	animNum_.emplace(ANIM::WALK, RUN_NUM);
	animNum_.emplace(ANIM::DODGE, DODGE_NUM);
	animNum_.emplace(ANIM::DAMAGE, DAMAGE_NUM);
	animNum_.emplace(ANIM::DEATH, DEATH_NUM);
}

void PlayerBase::InitCharaAnim(void)
{

}

void PlayerBase::InitAct(void)
{

}

void PlayerBase::ChangeAct(const ATK_ACT _act)
{
	//クールタイム中なら処理しない
	if (isCool_[static_cast<int>(_act)] && !IsAtkable())return;
	act_ = _act;


	//変更点
	changeAct_[_act]();
}

void PlayerBase::ChangeNmlAtk(void)
{
	ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);
	//actUpdate_ = std::bind(&PlayerBase::AtkFunc, this);
}

void PlayerBase::ChangeSkillOne(void)
{
	ResetAnim(ANIM::SKILL_1, SPEED_ANIM_ATK);
	isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
	//actUpdate_ = std::bind(&PlayerBase::Skill1Func, this);
}

void PlayerBase::ChangeSkillTwo(void)
{
	ResetAnim(ANIM::SKILL_2, SPEED_ANIM_ATK);
	//actUpdate_ = std::bind(&PlayerBase::Skill2Func, this);
}

void PlayerBase::ResetParam(ATK& _atk)
{
	_atk = atkMax_[act_];
}

void PlayerBase::ResetParam(void)
{
	atk_ = atkMax_[act_];
}



#ifdef DEBUG_ON
void PlayerBase::DrawDebug(void)
{
	const unsigned int ATK_COLOR = 0xff0000;
	//球体
	DrawSphere3D(trans_.pos, 20.0f, 8, 0x0, 0xff0000, true);
	//値見る用
	DrawFormatString(0, 32, 0xffffff
		, "FrameATK(%f)\nisAtk(%d)\nisBackSrash(%d)\nDodge(%f)\nSkill(%f)\natkStartTime(%f)\natkStartCnt(%f)\nskillType(%d)"
		, atk_.cnt_, atk_.IsAttack(), atk_.IsBacklash()
		, dodge_->GetDodgeCnt(), atk_.cnt_, atkStartTime_[static_cast<int>(SKILL_NUM::ONE)], atkStartCnt_, atkType_);


	//DrawFormatString(0, 200, 0xffffff
	//	, "AtkCooltime(%.2f)\nSkill1Cool(%.2f)\nSkill2Cool(%.2f)\natkDulation(%f)\natkCnt(%f)"
	//	, coolTimePer_[static_cast<int>(ATK_ACT::ATK)]
	//	, coolTimePer_[static_cast<int>(ATK_ACT::SKILL1)]
	//	, coolTimePer_[static_cast<int>(ATK_ACT::SKILL2)]
	//	, atk_.duration_
	//	, atkStartCnt_);

	//DrawFormatString(0, 32, 0xffffff, "atkPos(%f,%f,%f)", atk_.pos_.x, atk_.pos_.y, atk_.pos_.z);
	DrawSphere3D(colPos_, CHARACTER_SCALE * 100, 8, color_Col_, color_Col_, false);
	DrawSphere3D(atk_.pos_, atk_.radius_, 8, color_Atk_, color_Atk_, false);

	DrawFormatString(0, 400, 0xffffff, "actCntl(%d)", PlayerInput::GetInstance().GetAct());

	//プレイヤーの当たり判定
	//DrawSphere3D(colPos_, radius_, 4, 0xffff00, 0xffff00, false);
	//プレイヤーの索敵判定
	//DrawSphere3D(trans_.pos, searchRange_, 2, isMove_ ? 0xff0000 : 0xffffff, isMove_ ? 0xff0000 : 0xffffff, false);
	//プレイヤーの索敵判定
	//DrawSphere3D(userOnePos_, 20, 2, 0x0000ff, 0xffffff, false);

	//攻撃状態の時、球体の色変更
	if (atk_.IsAttack()) { color_Atk_ = ATK_COLOR; }
	else { color_Atk_ = 0x00ffff; }

}
#endif // DEBUG_ON

VECTOR PlayerBase::GetTargetVec(VECTOR _targetPos,bool _isMove)
{
	//標的への方向ベクトルを取得						※TODO:ベクトルはSceneGameからもらう
	VECTOR targetVec = VSub(_targetPos, trans_.pos);

	//正規化
	targetVec = VNorm(targetVec);

	//Y座標は必要ないので要素を消す
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//移動量を求める
	VECTOR ret = VScale(targetVec, moveSpeed_);

	if (_isMove) { return ret; }
	else { return targetVec; }
}




void PlayerBase::Turn(float _deg, VECTOR _axis)
{
	//transform_.quaRot =
	//	transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));

	trans_.quaRot =
		trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}



void PlayerBase::InitAtk(void)
{
	//攻撃カウント初期化
	atk_.ResetCnt();

	//スキルが終わったらクールタイムのカウント開始
	isCool_[static_cast<int>(act_)] = true;

	//移動可能にする
	moveAble_ = true;

	//消すかも
	atk_.isHit_ = false;

	//攻撃の発生
	atkStartCnt_ = 0.0f;


	EffectManager::GetInstance().Stop(EffectManager::EFFECT::GUARD);



	
}

void PlayerBase::SetBuff(STATUSBUFF_TYPE _type, SKILL_BUFF _skill, float _per, float _second)
{
	buffs_[_skill].cnt = _second;
	buffs_[_skill].percent[static_cast<int>(_type)] = _per;
}

void PlayerBase::SetPreStatus(void)
{
	preAtkPow_ = atkPow_;
	preDef_ = def_;
	preSpd_ = moveSpeed_;
}

void PlayerBase::BuffUpdate(void)
{
	for (auto& buff : buffs_)
	{
		CntDown(buff.second.cnt);
	
		if (buff.second.cnt>0.0f&&buff.second.isBuff)
		{
			for (int i = 0; i < static_cast<int>(SKILL_BUFF::MAX); i++)
			{
				if (buff.second.isBuffing == true)continue;

				//それぞれのバフ(スキルごとに設定されたバフ)をステータスのバフに足す
				buffpers_[STATUSBUFF_TYPE::ATK_BUFF] += buff.second.percent[static_cast<int>(STATUSBUFF_TYPE::ATK_BUFF)]/ DEFAULT_PERCENT;
				buffpers_[STATUSBUFF_TYPE::DEF_BUFF] += buff.second.percent[static_cast<int>(STATUSBUFF_TYPE::DEF_BUFF)]/ DEFAULT_PERCENT;
				buffpers_[STATUSBUFF_TYPE::SPD_BUFF] += buff.second.percent[static_cast<int>(STATUSBUFF_TYPE::SPD_BUFF)]/ DEFAULT_PERCENT;
				buff.second.isBuffing = true;
			}
		}
		else if(buff.second.cnt <= 0.0f)
		{
			buff.second.cnt = 0.0f;
			for (int i = 0; i < static_cast<int>(SKILL_BUFF::MAX); i++)
			{
				if (buff.second.isBuffing == false)continue;

				//制限時間が終わったらスキルで増やした増加量分引く
				buffpers_[STATUSBUFF_TYPE::ATK_BUFF] -= buff.second.percent[static_cast<int>(STATUSBUFF_TYPE::ATK_BUFF)] / DEFAULT_PERCENT;
				buffpers_[STATUSBUFF_TYPE::DEF_BUFF] -= buff.second.percent[static_cast<int>(STATUSBUFF_TYPE::DEF_BUFF)] / DEFAULT_PERCENT;
				buffpers_[STATUSBUFF_TYPE::SPD_BUFF] -= buff.second.percent[static_cast<int>(STATUSBUFF_TYPE::SPD_BUFF)] / DEFAULT_PERCENT;
				buff.second.isBuffing = false;
				buff.second.isBuff = false;
			}
		}
		atkPow_ = preAtkPow_ * (1.0f + buffpers_[STATUSBUFF_TYPE::ATK_BUFF]);
		def_ = preDef_ * (1.0f + buffpers_[STATUSBUFF_TYPE::DEF_BUFF]);
		moveSpeed_ = preSpd_ * (1.0f + buffpers_[STATUSBUFF_TYPE::SPD_BUFF]);
	}
}

void PlayerBase::Reset(void)
{
	//アニメーション初期化
	ResetAnim(ANIM::IDLE, SPEED_ANIM);

	skillNo_ = ATK_ACT::SKILL1;

	dodge_->Init();

	userOnePos_ = { -400.0f,0.0f,0.0f };

	ResetParam();

	hp_ = hpMax_;

	isSkill_ = false;
	isAtk_ = false;

	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		coolTime_[i] = coolTimeMax_[i];
	}

	////モデルの初期化
	trans_.Update();

	auto& effIns = EffectManager::GetInstance();
	effIns.Stop(EffectManager::EFFECT::GUARD);
	effIns.Stop(EffectManager::EFFECT::CHARGE_AXE_HIT);

	//変更箇所
	//---------------------------------------------
	effIns.Stop(EffectManager::EFFECT::CHARGE_AXE_HIT);
	effIns.Stop(EffectManager::EFFECT::ARCHER_SKILL2);

	SoundManager::GetInstance().Stop(SoundManager::SOUND::SKILL_CHANGE);
	//---------------------------------------------

}


void PlayerBase::BuffPerAdd(STATUSBUFF_TYPE _type, float _per)
{
	buffPercent_[static_cast<int>(_type)] += _per;
}

void PlayerBase::SyncActPos(ATK& _atk)
{
	//追従対象の位置
	VECTOR followPos = trans_.pos;

	//追従対象の向き
	Quaternion followRot = trans_.quaRot;

	//追従対象の角度
	VECTOR relativeActPos = atkMax_[act_].pos_;

	VECTOR addPos = followRot.PosAxis(VScale(relativeActPos, CHARACTER_SCALE));

	_atk.pos_ = VAdd(trans_.pos, addPos);
}

void PlayerBase::ChangeSkillControll(ATK_ACT _skill)
{
	atkType_ = atkTypes_[static_cast<int>(_skill)];
	isPush_ = false;
	moveAble_ = true;

}
const bool PlayerBase::IsAtkable(void) const
{
	 return!IsAtkAction() && !dodge_->IsDodge();
}
const bool PlayerBase::IsDodgeable(void) const
{
	 return !dodge_->IsDodge() && !IsAtkAction() && !dodge_->IsCoolDodge(); 
}
void PlayerBase::Damage(void)
{
	//とりあえず1ダメージ減らす
	hp_--;

	//アニメーション変更
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
		else if (coolTime_[i] <= 0.0f)
		{
			coolTime_[i] = 0.0f;
		}
	}
}


void PlayerBase::ProcessAct(void)
{
	AtkFunc();
	switch (skillNo_)
	{
	case PlayerBase::ATK_ACT::SKILL1:
		Skill1Func();
		break;
	case PlayerBase::ATK_ACT::SKILL2:
		Skill2Func();
		break;
	default:
		break;
	}

}

void PlayerBase::CoolTimePerCalc(void)
{
	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		coolTimePer_[i] = coolTime_[i] / coolTimeMax_[i];
	}
}

bool PlayerBase::IsSkillable(void)
{
	{ return !IsAtkAction() && !dodge_->IsDodge(); }
}

void PlayerBase::SkillChange(void)
{
	skillNo_ = static_cast<ATK_ACT>(static_cast<int>(skillNo_) + 1);
	auto& snd = SoundManager::GetInstance();
	snd.Play(SoundManager::SOUND::SKILL_CHANGE);
	snd.AdjustVolume(SoundManager::SOUND::SKILL_CHANGE, SKILL_CHANGE_SE_VOL);
	//MAXになったらスキル１に戻る
	skillNo_ == ATK_ACT::MAX ? skillNo_ = ATK_ACT::SKILL1 : skillNo_ = skillNo_;
	//変更点
	ChangeSkillControll(skillNo_);
}