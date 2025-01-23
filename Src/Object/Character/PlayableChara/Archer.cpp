#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Generic/SceneManager.h"
//#include""
#include"../../Arrow.h"

#include "Archer.h"

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
	hpMax_ = MAX_HP;
	atkPow_ = POW_ATK;
	def_ = MAX_DEF;

	moveAble_ = true;

	//当たり判定の設定
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	atkAbleCnt_ = 0;
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

	//攻撃タイプ
	atkTypes_[static_cast<int>(ATK_ACT::ATK)] = ATK_TYPE::NORMALATK;
	atkTypes_[static_cast<int>(ATK_ACT::SKILL1)] = ATK_TYPE::NORMALATK;
	atkTypes_[static_cast<int>(ATK_ACT::SKILL2)] = ATK_TYPE::NORMALATK;
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

void Archer::CreateArrow(void)
{
	//矢の生成処理
	//使い終わった攻撃がある場合
	for (auto& arrow : arrow_)
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
			arrowCnt_++;

			return;
		}
	}


	//新しく作る場合
	//新しく配列を追加
	std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
	arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);
	arrow->ChangeState(Arrow::STATE::SHOT);

	//配列に格納
	arrow_.emplace_back(arrow);

	//カウント増加
	arrowCnt_++;
}

void Archer::CreateAtk(void)
{
	for (auto& atk : arrowAtk_)
	{
		if (!atk.IsAttack())
		{
			//atk初期化
			ResetParam(atk);
			return;
		}
	}
	//新しく作る場合
	arrowAtk_.emplace_back(atkMax_[act_]);
}

void Archer::Update(void)
{
	PlayerBase::Update();

	size_t arrowSize = arrow_.size();
	//矢と矢に対応した攻撃の更新
	for (int a = 0; a < arrowSize; a++)
	{
		if (arrow_[a].get()->GetIsAlive())
		{
			CntUp(arrowAtk_[a].cnt_);
		}
		//攻撃状態が終わったら矢を破壊
		if (!arrowAtk_[a].IsAttack())
		{
			arrow_[a].get()->Destroy();
			InitArrowAtk(arrowAtk_[a]);
		}
		//更新
		arrow_[a].get()->Update(arrowAtk_[a]);
	}
}



void Archer::Draw(void)
{
	PlayerBase::Draw();
	size_t arrowSize = arrow_.size();
	for (auto& arrow : arrow_)
	{
		arrow.get()->Draw();
	}
	DrawFormatString(300, 100, 0xffffff, "arrowSize(%d)", arrowSize);
}

void Archer::AtkFunc(void)
{
	if (!isAtk_||isSkill_)return;
	//明日からアーチャー作成する！
	if (IsFinishAtkStart() && !isShotArrow_)
	{
		CreateArrow();
		CreateAtk();
		isShotArrow_ = true;
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

		CreateArrow();
		CreateAtk();
		coolTime_[static_cast<int>(
			ATK_ACT::SKILL1)] = 0.0f;

		//スキル終わったら攻撃発生時間の最大時間をセットする
		atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;

		InitAtk();
		isSkill_ = false;
		if (atk_.IsFinishMotion())
		{


		}
	}
}

void Archer::Skill2Func(void)
{

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
			ResetParam(arrowAtk_[s]);
			CntUp(atkStartCnt_);
		}
	}
}

void Archer::SkillTwoInit(void)
{
}

void Archer::InitArrowAtk(ATK& arrowAtk)
{
	//攻撃カウント初期化
	arrowAtk.ResetCnt();

	SyncActPos(arrowAtk);

	//消すかも
	arrowAtk.isHit_ = false;
}

