#include"../../Arrow.h"
#include"Enemy.h"
#include "EneArcher.h"

EneArcher::EneArcher(const VECTOR& _spawnPos) : Enemy(_spawnPos)
{
	trans_.pos = _spawnPos;

	arrowMdlId_ = -1;
	isShotArrow_ = false;
	arrowCnt_ = 0;
	reloadCnt_ = 0.0f;
	lastArrow_ = nullptr;
}

void EneArcher::Destroy(void)
{
	//共通解放
	Enemy::Destroy();

	//ポインタの解放
	lastArrow_ = nullptr;
	delete lastArrow_;
}

void EneArcher::SetParam(void)
{
	//攻撃警告
	alertSkills_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneArcher::AlertSkill_One, this));

	//攻撃の遷移
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneArcher::Skill_One, this));

	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_ARCHER));
	arrowMdlId_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ARROW);	

	//※個々で設定する変数
	trans_.scl = { CHARACTER_SCALE,CHARACTER_SCALE,CHARACTER_SCALE };
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	arrowCnt_ = 0;
	isShotArrow_ = false;
	localCenterPos_ = LOCAL_CENTER_POS;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;

	//外部からステータスを取得
	ParamLoad(CharacterParamData::UNIT_TYPE::E_ARCHER);
}

void EneArcher::InitAnim(void)
{
	//共通アニメーション初期化
	Enemy::InitAnim();

#pragma region 固有アニメーション初期化

	//スキル1
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
	//エイム
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_AIMING);
	//リロード
	animNum_.emplace(ANIM::UNIQUE_2, ANIM_RELOAD);

#pragma endregion

#pragma region アニメーション速度設定

	//スキル1
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM);
	//エイム
	changeSpeedAnim_.emplace(ANIM::UNIQUE_1, SPEED_ANIM);
	//リロード
	changeSpeedAnim_.emplace(ANIM::UNIQUE_2, SPEED_ANIM_RELOAD);

#pragma endregion

	//アニメーションリセット
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneArcher::InitSkill(void)
{
#pragma region スキルの格納

	//スキル1
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);

#pragma endregion

#pragma region スキルの予備動作アニメーション格納

	//スキル1
	skillPreAnims_.emplace_back(ANIM::UNIQUE_1);

#pragma endregion

#pragma region スキルの動作アニメーション

	//スキル1
	skillAnims_.emplace_back(ANIM::SKILL_1);

#pragma endregion

	//初期スキルを設定しておく
	RandSkill();
}

void EneArcher::AlertSkill_One(void)
{
	//敵の前方
	VECTOR pos = trans_.GetForward();

	//攻撃範囲
	pos = VScale(pos, SKILL_ONE_COL_RADIUS);

	//座標合わせ
	pos = VAdd(trans_.pos, VScale(pos, ARROW_SPEED * SKILL_ONE_DURATION));

	//範囲作成
	CreateAlert(pos, SKILL_ONE_COL_RADIUS * 2, SKILL_ONE_COL_RADIUS * 2 * ARROW_SPEED * SKILL_ONE_DURATION);
}

void EneArcher::Skill_One(void)
{
	//矢を放った
	if (isShotArrow_)
	{
		//攻撃の終了判定
		if (!lastArrow_->GetIsAlive() && lastAtk_->IsFinishMotion())
		{
			//攻撃終了
			isEndAllAtk_ = true;

			//処理終了
			return;
		}
	}

	//リロード中　又は　矢を放った判定なら攻撃しない
	if (IsReload() || isShotArrow_)return;

	//矢を生成
	Arrow& arrow = CreateArrow();

	//最後に生成された矢を格納
	lastArrow_ = &arrow;

	//同時に攻撃も生成
	lastAtk_ = &createSkill_();

	//矢を放つ
	arrow.ChangeState(Arrow::STATE::SHOT);

	//矢を放った
	isShotArrow_ = true;
}

Arrow& EneArcher::CreateArrow(void)
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

			return *arrow;
		}
	}

	//新しく配列を追加
	std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
	arrow->Init(arrowMdlId_, trans_, ARROW_SPEED);

	//配列に格納
	arrow_.emplace_back(arrow);

	//カウント増加
	arrowCnt_++;

	return *arrow_.back();
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
	ResetAnim(ANIM::UNIQUE_2, changeSpeedAnim_[ANIM::UNIQUE_2]);
}

void EneArcher::FinishAnim(void)
{
	//共通アニメーションの終了処理
	Enemy::FinishAnim();

	switch (anim_)
	{
	case UnitBase::ANIM::UNIQUE_1:
		break;
	case UnitBase::ANIM::UNIQUE_2:
		//ループ再生
		stepAnim_ = 0.0f;		
		break;
	}
}

void EneArcher::ResetAtkJudge(void)
{
	//共通
	Enemy::ResetAtkJudge();

	//矢を放った判定を初期化
	isShotArrow_ = false;

}

void EneArcher::ChangeStateAtk(void)
{
	//更新処理の中身初期化
	stateUpdate_ = std::bind(&EneArcher::UpdateAtk, this);
}

void EneArcher::Update(void)
{
	Enemy::Update();

	size_t arrowSize = arrow_.size();

	//矢と矢に対応した攻撃の更新
	for (int a = 0 ; a < arrowSize ; a++)
	{
		//更新
		arrow_[a].get()->Update(nowSkill_[a]);

		//攻撃状態が終わったら矢を破壊
		if (!nowSkill_[a].IsAttack())arrow_[a].get()->Destroy();
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
		
	//リロード
	if (IsReload())
	{
		ReloadArrow();
		return;
	}
	//攻撃休憩時間カウンタ
	else CntUp(breakCnt_);

	//待機アニメーション
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
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
