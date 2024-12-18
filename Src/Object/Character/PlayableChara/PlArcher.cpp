#include"../Manager/ResourceManager.h"
#include"../../Arrow.h"
#include "PlArcher.h"
PlArcher::PlArcher(const SceneManager::PLAY_MODE _mode, const SceneManager::CNTL _cntl) :PlayerBase(_mode, _cntl)
{
	mode_ = _mode;
	cntl_ = _cntl;
}
PlArcher::PlArcher(const SceneManager::PLAY_MODE _mode, const InputManager::JOYPAD_NO _padNum) : PlayerBase(_mode, _padNum)
{
	mode_ = _mode;
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

	atkStartRange_ = ATK_START_RANGE;
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


	isShotArrow_ = false;

}

void PlArcher::CreateArrow(void)
{
	//矢の生成処理
	for (auto& arrow : arrow_)
	{
		if (arrow->GetState() == Arrow::STATE::DESTROY)
		{
			//矢の情報を上書き
			arrow = nullptr;
			arrow = std::make_shared<Arrow>();
			arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);

			//カウント増加
			arrowCnt_++;

			return;
		}
	}

	//新しく配列を追加
	std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
	arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);

	//配列に格納
	arrow_.emplace_back(arrow);

	//カウント増加
	arrowCnt_++;
}

void PlArcher::Update(void)
{
	PlayerBase::Update();

	size_t arrowSize = arrow_.size();

	//矢と矢に対応した攻撃の更新
	for (int a = 0; a < arrowSize; a++)
	{
		//攻撃状態が終わったら矢を破壊
		if (atk_.IsFinishMotion())
		{
			arrow_[a].get()->Destroy();
		}
			
		//更新
		arrow_[a].get()->Update(atk_);
	}
}

void PlArcher::Draw(void)
{
	PlayerBase::Draw();

	size_t arrowSize = arrow_.size();
	for (int s = 0; s < arrowSize; s++)
	{
		arrow_[s].get()->Draw();
	}
}

void PlArcher::AtkFunc(void)
{
	//明日からアーチャー作成する！！！


	if (IsFinishAtkStart()&&!isShotArrow_)
	{
		CreateArrow();
		//矢を放つ
		arrow_.back().get()->ChangeState(Arrow::STATE::SHOT);

		//矢を放った
		isShotArrow_ = true;
	}
	
}

void PlArcher::Skill1Func(void)
{
}

void PlArcher::Skill2Func(void)
{
}
