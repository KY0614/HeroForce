#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Generic/SceneManager.h"
//#include""
#include"../../Arrow.h"

#include "Archer.h"

#ifdef DEBUG_ON
void Archer::DrawDebug(void)
{
	DrawFormatString(0, 0, 0x000000, "arrowCnt(%d,%d)", arrowCnt_[0], arrowCnt_[1]);
	size_t arrowSize = arrow_.size();
	for (int i = 0; i < static_cast<int>(ATK_TYPE::MAX); i++)
	{
		for (auto& arrow : arrow_[static_cast<ATK_TYPE>(i)])
		{
			arrow.get()->Draw();
		}
	}
	//DrawFormatString(0, 0, 0x000000, "atkStartCnt(%f)", atkStartCnt_);
}
#endif // DEBUG_ON



Archer::Archer(void)
{

}

void Archer::SetParam(void)
{
	InitAct();
	arrowMdlId_ = -1;
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
	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);


	//ステータス関係
	hpMax_ = 0;
	atkPow_ = POW_ATK;
	def_ = MAX_DEF;
	speed_ = SPEED;

	moveAble_ = true;

	//当たり判定の設定
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

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

//void Archer::CreateArrow(void)
//{
//	//矢の生成処理
//	//使い終わった攻撃がある場合
//	for (auto& arrow : arrow_)
//	{
//		// 破壊状態のとき
//		if (arrow->GetState() == Arrow::STATE::DESTROY)
//		{
//			//矢の情報を上書き
//			arrow = nullptr;
//
//			// 生成
//			arrow = std::make_shared<Arrow>();
//
//			// 初期化
//			arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);
//
//			arrow->ChangeState(Arrow::STATE::SHOT);
//
//			//カウント増加
//			arrowCnt_++;
//
//			return;
//		}
//	}
//
//
//	//新しく作る場合
//	//新しく配列を追加
//	std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
//	arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);
//	arrow->ChangeState(Arrow::STATE::SHOT);
//
//	//配列に格納
//	arrow_.emplace_back(arrow);
//
//	//カウント増加
//	arrowCnt_++;
//}

//void Archer::CreateAtk(void)
//{
//	for (auto& atk : arrowAtk_)
//	{
//		if (atk.IsFinishMotion())
//		{
//			//atk初期化
//			ResetParam(atk);
//			return;
//		}
//	}
//	//新しく作る場合
//	arrowAtk_.emplace_back(atkMax_[act_]);
//}




void Archer::CreateArrow(ATK_TYPE _type)
{
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
		if (atk.IsFinishMotion())
		{
			//atk初期化
			ResetParam(atk);
			return;
		}
	}
	////新しく作る場合
	arrowAtk_[_type].emplace_back(atkMax_[act_]);
}


//const PlayerBase::ATK Archer::GetArrowAtk(const int i)
//{
//	return arrowAtk_[i];
//}
//
//void Archer::SetIsArrowHit(const bool _flg, int _num)
//{
//	arrowAtk_[_num].isHit_ = _flg;
//}



//const PlayerBase::ATK Archer::GetArrowAtk(const int i)
//{
//	return arrowAtk_[i];
//}

const PlayerBase::ATK Archer::GetArrowAtk(const ATK_TYPE _type, const int i)
{
	return arrowAtk_[_type][i];
}

void Archer::SetIsArrowHit(ATK_TYPE _type, const bool _flg, int _num)
{
	arrowAtk_[_type][_num].isHit_ = true;
}

void Archer::Buff(UnitBase& _target)
{
	_target.SetAttack(BUFF);
	_target.SetDefense(BUFF);
	_target.SetSpeed(BUFF);
}

//void Archer::SetIsArrowHit(const bool _flg, int _num)
//{
//	arrowAtk_[_num].isHit_ = _flg;
//}





void Archer::Update(void)
{
	PlayerBase::Update();

	size_t arrowSize = arrow_.size();

	//for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	//{
	//	ArrowUpdate(static_cast<ATK_ACT>(i));
	//}
	ArrowUpdate(ATK_TYPE::ATTACK);
	ArrowUpdate(ATK_TYPE::BUFF);
	//ArrowUpdate(ATK_ACT::SKILL2);
}



void Archer::Draw(void)
{
	PlayerBase::Draw();

#ifdef DEBUG_ON
	//DrawFormatString(300, 100, 0xffffff, "arrowSize(%d)", arrowSize);
	DrawDebug();
#endif // DEBUG_ON
}

void Archer::AtkFunc(void)
{
	if (isSkill_)return;
	//明日からアーチャー作成する！
	if (IsFinishAtkStart() && !isShotArrow_)
	{
		CreateArrow(ATK_TYPE::ATTACK);
		CreateAtk(ATK_TYPE::ATTACK);
		//isShotArrow_ = true;
	}

	size_t arrowSize = arrow_.size();
	//近接攻撃用(atk_変数と遠距離で分ける)
	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
	}
	else if (IsFinishAtkStart())
	{
		CntUp(atkAbleCnt_);
		//クールタイムの初期化
		coolTime_[static_cast<int>(act_)] = 0.0f;
		if (atkAbleCnt_ >= ATKABLE_TIME)
		{
			InitAtk();
			isAtk_ = false;
		}
	}
}

void Archer::Skill1Func(void)
{
	if (isAtk_)return;
	if (isCool_[static_cast<int>(skillNo_)])
	{
		return;
	}
	if (0.0f < atkStartCnt_ && atkStartCnt_ < atkStartTime_[static_cast<int>(act_)])
	{
		CntUp(atkStartCnt_);
		if (stepAnim_ >= SKILL_CHARGE_STEPANIM)
		{
			stepAnim_ = SKILL_CHARGE_STEPANIM;
		}
	}
	else if (atkStartCnt_ >= atkStartTime_[static_cast<int>(skillNo_)])
	{

		if (!isShotArrow_)
		{
			CreateArrow(ATK_TYPE::ATTACK);
			CreateAtk(ATK_TYPE::ATTACK);
		}
		isShotArrow_ = true;
		CntUp(backrashCnt_);
		if (backrashCnt_ >= BACKRASH_MAX)
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
	if (isAtk_)return;
	//明日からアーチャー作成する！
	if (IsFinishAtkStart() && !isShotArrow_)
	{
		CreateArrow(ATK_TYPE::BUFF);
		CreateAtk(ATK_TYPE::BUFF);
		//isShotArrow_ = true;
	}

	size_t arrowSize = arrow_.size();
	//近接攻撃用(atk_変数と遠距離で分ける)
	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
	}
	else if (IsFinishAtkStart())
	{
		CntUp(atkAbleCnt_);
		//クールタイムの初期化
		coolTime_[static_cast<int>(act_)] = 0.0f;
		if (atkAbleCnt_ >= ATKABLE_TIME)
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
			//ResetParam(arrowAtk_[s]);
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
			arrow_[_type][a].get()->Destroy();
			InitArrowAtk(arrowAtk_[_type][a]);

			//存在している矢を減らす
			arrowCnt_[static_cast<int>(_type)]--;
		}
		//更新
		arrow_[_type][a].get()->Update(arrowAtk_[_type][a]);
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

