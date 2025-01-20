#include"../Manager/ResourceManager.h"
#include"../../Arrow.h"
#include "PlArcher.h"
PlArcher::PlArcher(const SceneManager::CNTL _cntl) :PlayerBase(_cntl)
{
	cntl_ = _cntl;
}
PlArcher::PlArcher(const InputManager::JOYPAD_NO _padNum) : PlayerBase(_padNum)
{
	padNum_ = _padNum;
}

void PlArcher::SetParam(void)
{
	InitAct();

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

	hp_ = MAX_HP;

	moveAble_ = true;

	//当たり判定の設定
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

	atkAbleCnt_ = 0;
}

void PlArcher::InitAct(void)
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

void PlArcher::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void PlArcher::InitAtk(void)
{
	//size_t size = arrowAtk_.size();
	////攻撃の初期化
	//for (int a = 0; a < size; a++)
	//{
	//	InitArrowAtk(arrowAtk_[a]);
	//}
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

void PlArcher::CreateArrow(void)
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

void PlArcher::CreateAtk(void)
{
	for (auto& atk : arrowAtk_)
	{
		if (!atk.IsAttack())
		{
			//atk初期化
			atk = atkMax_[act_];
			return;
		}
	}
	//新しく作る場合
	arrowAtk_.emplace_back(atkMax_[act_]);
}

void PlArcher::Update(void)
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



void PlArcher::Draw(void)
{
	PlayerBase::Draw();
	size_t arrowSize = arrow_.size();
	for (auto& arrow:arrow_)
	{
		arrow.get()->Draw();
	}
	DrawFormatString(300, 100, 0xffffff, "arrowSize(%d)", arrowSize);
}

void PlArcher::AtkFunc(void)
{
	//明日からアーチャー作成する！
	if (IsFinishAtkStart()&&!isShotArrow_)
	{
		CreateArrow();
		CreateAtk();
		//矢を放つ
		//for (auto& arrow : arrow_)
		//{
		//	arrow->ChangeState(Arrow::STATE::SHOT);
		//}
		// 
		
		//範囲for文の意味
		//for (int i = 0; i < arrow_.size; ++i)
		//{
		//	arrow_[i]->ChangeState(Arrow::STATE::SHOT)
		//}
		//矢を放った
		isShotArrow_ = true;
	}
	NmlActCommon();
}

void PlArcher::Skill1Func(void)
{
	
}

void PlArcher::Skill2Func(void)
{

}

void PlArcher::NmlAtkInit(void)
{
	if (isCool_[static_cast<int>(ATK_ACT::ATK)])return;
	ChangeAct(ATK_ACT::ATK);
	ResetParam(atk_);
	CntUp(atkStartCnt_);
}

void PlArcher::SkillOneInit(void)
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

void PlArcher::SkillTwoInit(void)
{
}

void PlArcher::NmlActCommon(void)
{
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
		}
	}
}




void PlArcher::InitArrowAtk(ATK& arrowAtk)
{
	//攻撃カウント初期化
	arrowAtk.ResetCnt();

	SyncActPos(arrowAtk);

	//消すかも
	arrowAtk.isHit_ = false;
}
