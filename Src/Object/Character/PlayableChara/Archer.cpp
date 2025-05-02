#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Generic/SceneManager.h"
//#include""
#include"../../Arrow.h"

#include "Archer.h"

#ifdef DEBUG_ON
void Archer::DrawDebug(void)
{
	DrawFormatString(200, 32, 0x000000, "arrowCnt(%d,%d)", arrow_[ATK_TYPE::ATTACK].size(), arrow_[ATK_TYPE::BUFF].size());
	DrawFormatString(200, 64, 0x000000, "atkCnt(%d)", arrowAtk_[ATK_TYPE::ATTACK].size());
	DrawSphere3D(trans_.pos, ARCHER_SEARCH_RANGE, 4, 0x0, 0xff0000, false);
	size_t arrowSize = arrow_.size();
	//DrawFormatString(0, 0, 0x000000, "atkStartCnt(%f)", atkStartCnt_);
}
#endif // DEBUG_ON



Archer::Archer(void)
{

}
void Archer::Destroy(void)
{
	arrow_.clear();
	arrowAtk_.clear();
	PlayerBase::Destroy();
}
void Archer::SetParam(void)
{
	InitAct();
	//プレイヤーモデル
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_ARCHER));
	float scale = CHARACTER_SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { -300.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);

	//エフェクト
	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::ARROW_RED,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::ARROW_RED).handleId_);

	//スキルチャージ
	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::CHARGE_SKILL, 
		ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARGE_SKILL).handleId_);

	//スキル2の発動エフェクト
	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::ARCHER_SKILL2,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::ARCHER_SKILL2).handleId_);


	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);

	//ステータス関係
	ParamLoad(CharacterParamData::UNIT_TYPE::ARCHER);

	//移動可能である
	moveAble_ = true;

	//役職の割り当て
	role_ = SceneManager::ROLE::ARCHER;

	//当たり判定の設定
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	auto& effIns = EffectManager::GetInstance();
	auto& resIns = ResourceManager::GetInstance();
	using EFFECT = EffectManager::EFFECT;

	//スキルチャージ(ロードするとなぜかエフェクトが出なくなるから念のためコメントアウト)
	EffectManager::GetInstance().Add(EFFECT::CHARGE_SKILL,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARGE_SKILL).handleId_);

	atkAbleCnt_ = 0;


	for (int i = 0; i < static_cast<int>(ATK_TYPE::MAX); i++)
	{
		arrowCnt_[i] = 0;
	}
}

void Archer::InitAct(void)
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

void Archer::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void Archer::InitAtk(void)
{
	//スキルが終わったらクールタイムのカウント開始
	isCool_[static_cast<int>(act_)] = true;

	//移動可能にする
	moveAble_ = true;

	//攻撃の発生
	atkStartCnt_ = 0.0f;

	isShotArrow_ = false;

	//攻撃可能時間
	atkAbleCnt_ = 0.0f;

}





void Archer::CreateArrow(ATK_TYPE _type)
{
	auto& efeIns = EffectManager::GetInstance();
	//矢の生成処理
	//使い終わった攻撃がある場合
	for (auto& arrow : arrow_[_type])
	{
		// 破壊状態のとき
		if (arrow->GetState() == Arrow::STATE::DESTROY)
		{
			//矢の情報を上書き
			arrow = nullptr;

			// 生成
			arrow = std::make_shared<Arrow>();

			//Idを指定(生成してからIdを指定する)
			arrowMdlId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ARROW);    //矢のモデル

			// 初期化
			arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);

			arrow->ChangeState(Arrow::STATE::SHOT);

			//カウント増加
			arrowCnt_[static_cast<int>(_type)]++;
			return;
		}
	}


	//新しく作る場合
	//新しく配列を追加
	std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();

	//Idを指定
	arrowMdlId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ARROW);    //矢のモデル
	arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);
	arrow->ChangeState(Arrow::STATE::SHOT);

	//配列に格納
	arrow_[_type].emplace_back(arrow);

	//カウント増加
	arrowCnt_[static_cast<int>(_type)]++;
}





void Archer::CreateAtk(ATK_TYPE _type)
{
	for (auto& atk : arrowAtk_[_type])
	{
		//当たり判定が初期化されてない問題。ここを!atk.IsAttack()にしてみる
		if (atk.IsFinishMotion())
		{
			//atk初期化
			ResetParam(atk);
			return;
		}
	}
	//新しく作る場合
	arrowAtk_[_type].emplace_back(atkMax_[act_]);
}


const PlayerBase::ATK Archer::GetArrowAtk(const ATK_TYPE _type, const int i)
{
	return arrowAtk_[_type][i];
}

void Archer::SetIsArrowHit(ATK_TYPE _type, const bool _flg, int _num)
{
	arrowAtk_[_type][_num].isHit_ = true;
}

void Archer::Buff(PlayerBase& _target)
{
	//前の情報を格納する
	_target.SetPreStatus();

	//バフ情報をセット
	_target.SetBuff(STATUSBUFF_TYPE::ATK_BUFF,SKILL_BUFF::BUFF_ARROW, BUFF, SKILL2_BUFF_TIME);
	_target.SetBuff(STATUSBUFF_TYPE::SPD_BUFF, SKILL_BUFF::BUFF_ARROW, BUFF, SKILL2_BUFF_TIME);
	_target.SetBuff(STATUSBUFF_TYPE::DEF_BUFF, SKILL_BUFF::BUFF_ARROW, BUFF, SKILL2_BUFF_TIME);
}




void Archer::Update(void)
{
	PlayerBase::Update();

	size_t arrowSize = arrow_.size();

	ArrowUpdate(ATK_TYPE::ATTACK);
	ArrowUpdate(ATK_TYPE::BUFF);
}



void Archer::Draw(void)
{
	PlayerBase::Draw();
	for (int i = 0; i < static_cast<int>(ATK_TYPE::MAX); i++)
	{
		for (auto& arrow : arrow_[static_cast<ATK_TYPE>(i)])
		{
			arrow.get()->Draw();
		}
	}


#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
}

void Archer::AtkFunc(void)
{
	if (isSkill_)return;
	//明日からアーチャー作成する！
	if (IsFinishAtkStart() && !isShotArrow_)
	{
		CreateAtk(ATK_TYPE::ATTACK);
		CreateArrow(ATK_TYPE::ATTACK);
		isShotArrow_ = true;
	}

	size_t arrowSize = arrow_.size();
	//近接攻撃用(atk_変数と遠距離で分ける)
	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
		if (isSerchArcher_)
		{
			VECTOR targetVec = GetTargetVec(targetPos_, false);
			//回転
			trans_.quaRot = trans_.quaRot.LookRotation(targetVec);
		}
	}
	else if (IsFinishAtkStart())
	{
		CntUp(backrashCnt_);
		//クールタイムの初期化
		coolTime_[static_cast<int>(act_)] = 0.0f;
		if (backrashCnt_ >= NMLATK_BACKRASH_MAX)
		{
			InitAtk();
			isAtk_ = false;
			backrashCnt_ = 0.0f;
		}
	}
}

void Archer::Skill1Func(void)
{
	if (!isSkill_)return;
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
		if (isSerchArcher_)
		{
			VECTOR targetVec = GetTargetVec(targetPos_, false);
			//回転
			trans_.quaRot = trans_.quaRot.LookRotation(targetVec);
		}

		if (stepAnim_ >= SKILL_CHARGE_STEPANIM)
		{
			stepAnim_ = SKILL_CHARGE_STEPANIM;
		}
	}
	else if (atkStartCnt_ >= atkStartTime_[static_cast<int>(skillNo_)])
	{
		if (!isShotArrow_)
		{
			CreateAtk(ATK_TYPE::ATTACK);
			CreateArrow(ATK_TYPE::ATTACK);
		}
		if (atkStartCnt_ > SKILL_ONE_CHARE_STARTCNT)
			arrowAtk_[ATK_TYPE::ATTACK].back().pow_ = SKILL_ONE_POW_MAX;

		isShotArrow_ = true;
		CntUp(backrashCnt_);
		if (backrashCnt_ >= SKILL1_BACKRASH_MAX)
		{
			coolTime_[static_cast<int>(ATK_ACT::SKILL1)] = 0.0f;

			//スキル終わったら攻撃発生時間の最大時間をセットする
			atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;

			backrashCnt_ = 0.0f;
			InitAtk();
			isSkill_ = false;
		}
	}
}

void Archer::Skill2Func(void)
{
	if (!isSkill_)return;
	//明日からアーチャー作成する！
	if (IsFinishAtkStart() && !isShotArrow_)
	{
		CreateAtk(ATK_TYPE::BUFF);
		CreateArrow(ATK_TYPE::BUFF);
		isShotArrow_ = true;
	}

	size_t arrowSize = arrow_.size();
	//近接攻撃用(atk_変数と遠距離で分ける)
	if (IsAtkStart())
	{
		if (atkStartCnt_ <= DELTATIME)
		{
			EffectManager::GetInstance().Play(
				EffectManager::EFFECT::ARCHER_SKILL2,
				trans_.pos,
				Quaternion(),
				CHARGE_SKILL_EFF_SIZE,
				SoundManager::SOUND::NONE);
		}
		moveAble_ = false;
		CntUp(atkStartCnt_);
	}
	else if (IsFinishAtkStart())
	{
		CntUp(backrashCnt_);
		//クールタイムの初期化
		coolTime_[static_cast<int>(act_)] = 0.0f;
		if (backrashCnt_ >= SKILL2_BACKRASH_MAX)
		{
			InitAtk();
			isSkill_ = false;
		}
	}
}

void Archer::NmlAtkInit(void)
{
	if (isCool_[static_cast<int>(ATK_ACT::ATK)])return;
	ChangeAct(ATK_ACT::ATK);
	CntUp(atkStartCnt_);
}

void Archer::SkillOneInit(void)
{
	size_t arrowSize = arrow_.size();
	for (int s = 0; s < arrowSize; s++)
	{
		if (IsSkillable() && !isCool_[static_cast<int>(skillNo_)])
		{
			//スキルごとにアニメーションを決めて、カウント開始
			ChangeAct(static_cast<ATK_ACT>(skillNo_));
			ResetParam(arrowAtk_[ATK_TYPE::ATTACK][s]);
			CntUp(atkStartCnt_);
		}
	}
}

void Archer::SkillTwoInit(void)
{

}
void Archer::ArrowUpdate(ATK_TYPE _type)
{
	size_t arrowSize = arrow_[_type].size();
	for (int a = 0; a < arrowSize; a++)
	{
		if (arrow_[_type][a].get()->GetIsAlive())
		{
			CntUp(arrowAtk_[_type][a].cnt_);
		}
		//攻撃状態が終わったら矢を破壊
		if (arrowAtk_[_type][a].IsFinishMotion())
		{
			if (arrow_[_type][a].get()->GetState() == Arrow::STATE::SHOT)
			{
				//存在している矢を減らす
				arrowCnt_[static_cast<int>(_type)]--;
			}

			arrow_[_type][a].get()->Destroy();

		}
		//更新
		arrow_[_type][a].get()->Update(arrowAtk_[_type][a]);

		////エフェクト設定
		//EffectManager::GetInstance().SyncEffect(
		//	EffectManager::EFFECT::ARROW_RED,
		//	arrowAtk_[_type][a].pos_,
		//	trans_.GetForward(),
		//	ARROW_EFFECT_SIZE);
	}
}
void Archer::InitArrowAtk(ATK& arrowAtk)
{
	//攻撃カウント初期化
	arrowAtk.ResetCnt();

	SyncActPos(arrowAtk);

	//消すかも
	arrowAtk.isHit_ = false;
}

