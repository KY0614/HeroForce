#include"../../Arrow.h"
#include "Knight.h"

Knight::Knight(void)
{
	
}
void Knight::SetParam(void)
{
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	float scale = CHARACTER_SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { -300.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);

	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);

	//ステータス
	ParamLoad(CharacterParamData::UNIT_TYPE::KNIGHT);

	//役職の割り当て
	role_ = SceneManager::ROLE::KNIGHT;
	
	//溜め攻撃エフェクトロード
	auto& effIns = EffectManager::GetInstance();
	auto& resIns = ResourceManager::GetInstance();
	using EFFECT = EffectManager::EFFECT;
	effIns.Add(EFFECT::GUARD, resIns.Load(ResourceManager::SRC::GUARD).handleId_);

}

void Knight::Update(void)
{
	PlayerBase::Update();

	size_t arrowSize = arrow_.size();
	//矢と矢に対応した攻撃の更新
	for (int a = 0; a < arrowSize; a++)
	{
		if (arrow_[a].get()->GetIsAlive())
		{
			CntUp(slashArrow_.cnt_);
		}
		//攻撃状態が終わったら矢を破壊
		if (!slashArrow_.IsAttack())
		{
			arrow_[a].get()->Destroy();
			InitSlashAtk(slashArrow_);
			isShotArrow_ = false;
		}
		//更新
		arrow_[a].get()->Update(slashArrow_);
	}
}

void Knight::Draw(void)
{
	PlayerBase::Draw();
	size_t arrowSize = arrow_.size();
	for (auto& arrow : arrow_)
	{
		arrow.get()->Draw();
	}
}

void Knight::InitAct(void)
{
	//通常攻撃の最大値
	atkMax_.emplace(ATK_ACT::ATK, ATK_MAX);

	//スキル１の最大値
	atkMax_.emplace(ATK_ACT::SKILL1, SKILL_ONE_MAX);

	//スキル２の最大値
	atkMax_.emplace(ATK_ACT::SKILL2, SKILL_TWO_MAX);

	//斬撃
	slashArrow_ = SLASH_MAX;



	//クールタイム
	coolTimeMax_[static_cast<int>(ATK_ACT::ATK)] = ATK_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_COOLTIME;

	//攻撃発生時間
	atkStartTime_[static_cast<int>(ATK_ACT::ATK)] = ATK_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_START;



}


void Knight::AtkFunc(void)
{
	if (isSkill_)return;
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;

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
		else if(atk_.IsFinishMotion())/*これつけると通常連打の時にバグる*/
		{
			InitAtk();
			isAtk_ = false;
		}
	}
}


void Knight::NmlAtkInit(void)
{
}

void Knight::SkillOneInit(void)
{
	//スキルごとにアニメーションを決めて、カウント開始
	ChangeAct(static_cast<ATK_ACT>(skillNo_));
	ResetParam(atk_);
	CntUp(atkStartCnt_);
}

void Knight::SkillTwoInit(void)
{

}


void Knight::InitSkill(ATK_ACT _act)
{
	//スキルごとにアニメーションを決めて、カウント開始
	if (isAtk_ || isSkill_ || isCool_[static_cast<int>(_act)])return;
	ChangeAct(static_cast<ATK_ACT>(_act));
	ResetParam(atk_);
	CntUp(atkStartCnt_);
	moveAble_ = false;
	isSkill_ = true;
}

void Knight::Skill1Func(void)
{
	if (isAtk_)return;
	//斬撃飛ばす
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	//明日からアーチャー作成する！
	if (IsFinishAtkStart() && !isShotArrow_)
	{
		if (hp_ == hpMax_)
		{
			CreateSlash();
			//CreateAtk();
			isShotArrow_ = true;
		}
	}

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
		else if (atk_.IsFinishMotion())/*これつけると通常連打の時にバグる*/
		{
			InitAtk();
			isSkill_ = false;
		}
	}

}

void Knight::Skill2Func(void)
{
	if (!isSkill_)return;
	//ボタン長押ししているときにクールタイムが0秒以下になった時
	if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] <= 0.0f)
	{
		//スキル切り替え出来ないようにする
		isSkill_ = false;
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
		InitAtk();
		return;
	}
	if (isSkill_&&!IsFinishAtkStart())CntUp(atkStartCnt_);

	else if (isSkill_ && IsFinishAtkStart())
	{
		EffectManager::GetInstance().Stop(EffectManager::EFFECT::HIT2);
		CntUp(atk_.cnt_);
		CntDown(coolTime_[static_cast<int>(SKILL_NUM::TWO)]);
		//スキル２継続中は防御力２倍になる
		SetBuff(STATUSBUFF_TYPE::DEF_BUFF, SKILL_BUFF::GUARD, 100.0f, 0.1f);

		SetIsBuff(SKILL_BUFF::GUARD, true);
		//最初の1フレームだけの処理
		if (atk_.cnt_ <= 1.0f / 60.0f)
		{
			auto& efeIns = EffectManager::GetInstance();
			efeIns.Play(
				EffectManager::EFFECT::GUARD,
				atk_.pos_,
				Quaternion(),
				GUARD_EFFECT_SIZE,
				SoundManager::SOUND::NONE);
		}
	
	}
	//ボタンを押していても残りクールタイムが3秒以下だったら
	else if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] <= SKILL_TWO_START_COOLTIME)
	{
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
		InitAtk();
		isSkill_ = false;
		return;
	}
}

void Knight::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::UNIQUE_2, BLOCKING_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void Knight::InitSlashAtk(ATK& arrowAtk)
{
	//攻撃カウント初期化
	arrowAtk.ResetCnt();

	SyncActPos(arrowAtk);

	//消すかも
	arrowAtk.isHit_ = false;
}

void Knight::CreateSlash(void)
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
			arrow->Init(arrowMdlId_, trans_, SLASH_SPEED);
			InitSlashAtk(slashArrow_);

			arrow->ChangeState(Arrow::STATE::SHOT);

			//カウント増加
			arrowCnt_++;

			return;
		}
	}


	//新しく作る場合
	//新しく配列を追加
	std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
	arrow->Init(arrowMdlId_, trans_, SLASH_SPEED);
	InitSlashAtk(slashArrow_);
	arrow->ChangeState(Arrow::STATE::SHOT);

	//配列に格納
	arrow_.emplace_back(arrow);

	//カウント増加
	arrowCnt_++;
}

void Knight::CreateSlashAtk(void)
{

}

#ifdef DEBUG_ON
void Knight::DrawDebug(void)
{
	PlayerBase::DrawDebug();
	DrawFormatString(0, 200, 0xffffff
		, "AtkCooltime(%.2f)\nSkill1Cool(%.2f)\nSkill2Cool(%.2f)\natkDulation(%f)\natkCnt(%f)"
		, coolTime_[static_cast<int>(ATK_ACT::ATK)]
		, coolTime_[static_cast<int>(ATK_ACT::SKILL1)]
		, coolTime_[static_cast<int>(ATK_ACT::SKILL2)]
		, atk_.duration_
		, atkStartCnt_);
}
#endif // DEBUG_ON

