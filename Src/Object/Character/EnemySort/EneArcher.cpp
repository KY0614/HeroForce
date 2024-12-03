#include"../../../Manager/Resource.h"
#include"../../../Manager/ResourceManager.h"
#include"../../Arrow.h"
#include"../Enemy.h"
#include "EneArcher.h"

void EneArcher::SetParam(void)
{
	//攻撃の遷移
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneArcher::Skill_One, this));

	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_ARCHER));
	//arrowMdlId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::);	※TODO矢のモデル
	arrowMdlId_ = 0;

	//※個々で設定する
	radius_ = MY_COL_RADIUS;
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	hp_ = HP_MAX;
	atkPow_ = ATK_POW;
	def_ = DEF;
	arrowCnt_ = 0;
	isShotArrow_ = false;
	walkSpeed_ = WALK_SPEED;
	localCenterPos_ = LOCAL_CENTER_POS;
	stunDefMax_ = STUN_DEF_MAX;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
}

void EneArcher::InitAnimNum(void)
{
	//共通アニメーション初期化
	Enemy::InitAnimNum();

	//固有アニメーション初期化
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_AIMING);
	animNum_.emplace(ANIM::UNIQUE_2, ANIM_RELOAD);
}

void EneArcher::InitSkill(void)
{
	//ここにスキルの数分格納させる
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);

	//ここにスキルの数分アニメーションを格納させる
	skillAnims_.emplace_back(ANIM::SKILL_1);

	//初期スキルを設定しておく
	RandSkill();
}

void EneArcher::Attack(void)
{
	//リロード中　又は　矢を放った判定なら攻撃しない
	if (IsReload() || isShotArrow_)return;

	//対応スキル発動
	processSkill_();
}

void EneArcher::Skill_One(void)
{
	//矢を生成
	CreateArrow();

	//矢を放つ
	arrow_.back().get()->ChangeState(Arrow::STATE::SHOT);

	//矢を放った
	isShotArrow_ = true;
}

void EneArcher::CreateArrow(void)
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

void EneArcher::ReloadArrow(void)
{
	//リロードが終わったらカウンタリセット
	if (reloadCnt_ >= RELOAD_TIME)
	{
		arrowCnt_ = 0;
		reloadCnt_ = 0.0f;
		return;
	}

	//リロード時間カウント
	CntUp(reloadCnt_);

	//リロードアニメーション
	ResetAnim(ANIM::UNIQUE_2, SPEED_ANIM);
}

void EneArcher::FinishAnim(void)
{
	//共通アニメーションの終了処理
	Enemy::FinishAnim();

	switch (anim_)
	{
	case UnitBase::ANIM::UNIQUE_1:
	case UnitBase::ANIM::UNIQUE_2:
		break;
	}
}

void EneArcher::ChangeStateAlert(void)
{
	//更新処理の中身初期化
	Enemy::ChangeStateAlert();

	//待機アニメーション
	ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM);
}

void EneArcher::ChangeStateBreak(void)
{
	//更新処理の中身初期化
	Enemy::ChangeStateBreak();

	//矢を放った判定を初期化
	isShotArrow_ = false;
}

void EneArcher::Update(void)
{
	Enemy::Update();

	size_t arrowSize = arrow_.size();

	//矢と矢に対応した攻撃の更新
	for (int a = 0 ; a < arrowSize ; a++)
	{
		//攻撃状態が終わったら矢を破壊
		if (!nowSkill_[a].IsAttack())arrow_[a].get()->Destroy();

		//更新
		arrow_[a].get()->Update(nowSkill_[a]);
	}
}

void EneArcher::UpdateBreak(void)
{
	//**********************************************************
	//終了処理
	//**********************************************************

	//休憩時間が終わったら
	if (!IsBreak())
	{
		//通常状態に遷移
		ChangeState(STATE::NORMAL);
		return;
	}

	//**********************************************************
	//動作処理
	//**********************************************************

	//待機アニメーション
	ResetAnim(ANIM::IDLE, SPEED_ANIM);

	//リロード
	if (IsReload())ReloadArrow();
	//攻撃休憩時間カウンタ
	else CntUp(breakCnt_);
}

void EneArcher::Draw(void)
{
	Enemy::Draw();

	size_t skillSize = nowSkill_.size();
	size_t arrowSize = arrow_.size();

	for (int s = 0; s < arrowSize; s++)
	{
		arrow_[s].get()->Draw();
	}
}
