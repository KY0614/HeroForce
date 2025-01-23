#include<DxLib.h>
#include<cmath>
#include"../../../Application.h"
#include"../../../Utility/AsoUtility.h"
#include "Enemy.h"

Enemy::Enemy(const VECTOR& _spawnPos)
{
	trans_.pos = _spawnPos;
}

void Enemy::Destroy(void)
{
	animNum_.clear();

	lastAtk_ = nullptr;
	delete lastAtk_;
}

void Enemy::Init(void)
{
	//状態管理
	stateChanges_.emplace(STATE::NORMAL, std::bind(&Enemy::ChangeStateNormal, this));
	stateChanges_.emplace(STATE::ALERT, std::bind(&Enemy::ChangeStateAlert, this));
	stateChanges_.emplace(STATE::ATTACK, std::bind(&Enemy::ChangeStateAtk, this));
	stateChanges_.emplace(STATE::BREAK, std::bind(&Enemy::ChangeStateBreak, this));

	//探索状態管理
	SearchStateInfo_.emplace(SEARCH_STATE::NOT_FOUND, std::bind(&Enemy::SearchMoveInfo, this));
	SearchStateInfo_.emplace(SEARCH_STATE::CHICKEN_FOUND, std::bind(&Enemy::FoundMoveInfo, this));
	SearchStateInfo_.emplace(SEARCH_STATE::PLAYER_FOUND, std::bind(&Enemy::FoundMoveInfo, this));

	//キャラ固有設定
	SetParam();

	//アニメーション番号の初期化
	InitAnim();

	//共通の変数の初期化
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
	ChangeState(STATE::NORMAL);
	alertCnt_ = 0.0f;
	breakCnt_ = 0.0f;
	stunDef_ = 0;
	atkAct_ = ATK_ACT::MAX;
	isEndAllAtkSign_ = false;
	isEndAllAtk_ = false;
	isMove_ = false;
	ChangeSearchState(SEARCH_STATE::NOT_FOUND);

	//攻撃情報の初期化
	InitSkill();
	atk_.ResetCnt();

	trans_.Update();
}

void Enemy::Update(void)
{
	//移動前座標
	prePos_ = trans_.pos;

	//アニメーション
	Anim();

#ifdef DEBUG_ENEMY
	//入力用
	InputManager& ins = InputManager::GetInstance();

	//if (ins.IsNew(KEY_INPUT_W)) { targetPos_.z+= 3.0f; }
	//if (ins.IsNew(KEY_INPUT_D)) { targetPos_.x+= 3.0f; }
	//if (ins.IsNew(KEY_INPUT_S)) { targetPos_.z-= 3.0f; }
	//if (ins.IsNew(KEY_INPUT_A)) { targetPos_.x-= 3.0f; }

	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Q)) { Damage(1, 10); }
#endif // DEBUG_ENEMY


	//やられているなら何もしない
	if (!IsAlive()) { return; }

	//当たり判定座標の更新
	colPos_ = VAdd(trans_.pos, localCenterPos_);

	//状態ごとのUpdate
	stateUpdate_();

	//モデル制御
	trans_.Update();
}

void Enemy::ChangeSearchState(const SEARCH_STATE _searchState)
{
	//状態遷移
	searchState_ = _searchState;

	//状態による情報更新
	SearchStateInfo_[searchState_]();
}

void Enemy::Damage(const int _damage, const int _stunPow)
{
	//既にやられているなら処理しない
	if (!IsAlive()) { return; }

	//ダメージカウント
	hp_ -= _damage;

	//スタン値カウント
	stunDef_ += _stunPow;

	//やられたら死亡アニメーション
	if (!IsAlive()){ ResetAnim(ANIM::DEATH, changeSpeedAnim_[ANIM::DEATH]); }
}

void Enemy::ChangeState(const STATE _state)
{
	//状態遷移
	state_ = _state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();
}

void Enemy::ChangeStateNormal(void)
{
	//更新処理の中身初期化
	stateUpdate_ = std::bind(&Enemy::UpdateNml, this);
}

void Enemy::ChangeStateAlert(void)
{
	//更新処理の中身初期化
	stateUpdate_ = std::bind(&Enemy::UpdateAlert, this);

	//向きを改めて設定
	trans_.quaRot = trans_.quaRot.LookRotation(GetMoveVec(trans_.pos,targetPos_));

	//ランダムで攻撃情報を生成
	RandSkill();

	//攻撃予兆を生成
	lastAtkSign_ = &createSkillSign_();

	//予備動作アニメーション
	ResetAnim(nowSkillPreAnim_, changeSpeedAnim_[nowSkillPreAnim_]);

	//警告カウンタ初期化
	alertCnt_ = 0.0f;
}

void Enemy::ChangeStateAtk(void)
{
	//更新処理の中身初期化
	stateUpdate_ = std::bind(&Enemy::UpdateAtk, this);

	//攻撃生成
	lastAtk_ = &createSkill_();
}

void Enemy::ChangeStateBreak(void)
{
	//更新処理の中身初期化
	stateUpdate_ = std::bind(&Enemy::UpdateBreak, this);

	//攻撃休憩時間の初期化
	breakCnt_ = 0;
}

void Enemy::InitAnim()
{
	//共通アニメーション
	animNum_.emplace(ANIM::IDLE, ANIM_IDLE);
	animNum_.emplace(ANIM::WALK, ANIM_WALK);
	animNum_.emplace(ANIM::RUN, ANIM_RUN);
	animNum_.emplace(ANIM::DAMAGE, ANIM_DAMAGE);
	animNum_.emplace(ANIM::DEATH, ANIM_DEATH);
	animNum_.emplace(ANIM::ENTRY, ANIM_ENTRY);

	//アニメーション速度設定
	changeSpeedAnim_.emplace(ANIM::IDLE, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::WALK, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::RUN, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::DAMAGE, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::DEATH, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::ENTRY, SPEED_ANIM);
}

void Enemy::Alert(void)
{
	//警告
	alertNowSkill_();
}

void Enemy::Attack(void)
{
	//対応スキル発動
	processSkill_();

	//各攻撃のカウント
	for (auto& nowSkill : nowSkill_)
	{
		//攻撃のカウント
		CntUp(nowSkill.cnt_);
	}
}

void Enemy::UpdateNml(void)
{
	//**********************************************************
	//終了処理
	//**********************************************************

	/*誰かが攻撃範囲に入ったら状態を遷移します*/
	
	/*処理はゲームシーンにあります*/

	//**********************************************************
	//動作処理
	//**********************************************************

	//待機アニメーション
	if (moveSpeed_ == 0.0)
		ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
	//歩きアニメーション
	else if (moveSpeed_ > 0.0f && searchState_ == SEARCH_STATE::NOT_FOUND)
		ResetAnim(ANIM::WALK, changeSpeedAnim_[ANIM::WALK]);
	//走りアニメーション
	else if (moveSpeed_ > 0.0f && searchState_ != SEARCH_STATE::NOT_FOUND)
		ResetAnim(ANIM::RUN, changeSpeedAnim_[ANIM::RUN]);
	
	//索敵
	//if (!isMove_)return;		※のちに消すかも

	//移動処理
	Move();
}

void Enemy::UpdateAlert(void)
{
	//**********************************************************
	//終了処理
	//**********************************************************

	//警告カウンタが終わったなら攻撃開始
	if (!IsAlertTime())
	{		
		//攻撃状態に遷移
		ChangeState(STATE::ATTACK);

		return;
	}

	//**********************************************************
	//動作処理
	//**********************************************************

	//警告
	Alert();

	//クールダウンカウンタ
	CntUp(alertCnt_);
}

void Enemy::UpdateAtk(void)
{
	//**********************************************************
	//終了処理
	//**********************************************************

	//攻撃が終わっているなら状態遷移
	if (isEndAllAtk_)
	{
		//攻撃終了判定の初期化
		ResetAtkJudge();

		//休憩状態に遷移
		ChangeState(STATE::BREAK);
		return;
	}

	//**********************************************************
	//動作処理
	//**********************************************************

	//攻撃処理
	Attack();

	//攻撃アニメーション
	ResetAnim(nowSkillAnim_, changeSpeedAnim_[nowSkillAnim_]);
}

void Enemy::UpdateBreak(void)
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
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);

	//攻撃休憩時間カウンタ
	CntUp(breakCnt_);
}

void Enemy::DrawDebug(void)
{
	//デバッグ
	DrawFormatString(0, Application::SCREEN_SIZE_Y - 16, 0xffffff, "EnemyHP = %d", hp_);
	int statePos = Application::SCREEN_SIZE_Y - 32;
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		DrawFormatString(0, statePos, 0x00ff00, "敵現在状態:NORMAL");
		break;

	case Enemy::STATE::ALERT:
		DrawFormatString(0, statePos, 0x00ff00, "敵現在状態:ALERT_ATTACK");
		break;

	case Enemy::STATE::ATTACK:
		DrawFormatString(0, statePos, 0x00ff00, "敵現在状態:ATTACK");
		break;

	case Enemy::STATE::BREAK:
		DrawFormatString(0, statePos, 0x00ff00, "敵現在状態:BREAK");
		break;
	}
	//敵の当たり判定
	DrawSphere3D(colPos_, radius_, 4, 0xffff00, 0xffff00, false);
	//敵の索敵判定
	DrawSphere3D(trans_.pos, searchRange_, 2, isMove_ ? 0xff0000 : 0xffffff, isMove_ ? 0xff0000 : 0xffffff, false);
	//敵の索敵判定
	DrawSphere3D(trans_.pos, atkStartRange_, 2, state_ == STATE::ALERT ? 0xff0000 : 0xffffff, state_ == STATE::ALERT ? 0x0000ff : 0xffffff, false);
}

void Enemy::SearchMoveInfo(void)
{
	//移動速度更新
	moveSpeed_ = walkSpeed_;
}

void Enemy::FoundMoveInfo(void)
{
	//移動速度更新
	moveSpeed_ = runSpeed_;
}

void Enemy::Draw(void)
{
#ifdef DEBUG_ENEMY
	
	//デバッグ
	DrawDebug();

#endif // DEBUG_ENEMY

	//敵モデルの描画
	MV1DrawModel(trans_.modelId);

	for (auto& nowSkill : nowSkill_)
	{
		//攻撃の描画
		if (nowSkill.IsAttack()) { DrawSphere3D(nowSkill.pos_, nowSkill.radius_, 50.0f, 0xff0f0f, 0xff0f0f, true); }
		else if (nowSkill.IsBacklash()) { DrawSphere3D(nowSkill.pos_, nowSkill.radius_, 5.0f, 0xff0f0f, 0xff0f0f, false); }
	}

	for (auto& nowSkillSign : nowSkillSign_)
	{
		//攻撃予兆の描画
		if (nowSkillSign.IsAttack()) { DrawSphere3D(nowSkillSign.pos_, nowSkillSign.radius_, 50.0f, 0x0fff0f, 0x0fff0f, true); }
		else if (nowSkillSign.IsBacklash()) { DrawSphere3D(nowSkillSign.pos_, nowSkillSign.radius_, 5.0f, 0x0fff0f, 0x0fff0f, false); }
	}
}

const VECTOR Enemy::GetMoveVec(const VECTOR _start, const VECTOR _goal, const float _speed)
{
	//標的への方向ベクトルを取得
	VECTOR targetVec = VSub(_goal, _start);

	//正規化
	targetVec = VNorm(targetVec);

	//Y座標は必要ないので要素を消す
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//移動量を求める
	VECTOR ret = VScale(targetVec, _speed);

	return ret;
}

void Enemy::Move(void)
{
	//移動ベクトル取得
	VECTOR targetVec = GetMoveVec(trans_.pos, targetPos_, moveSpeed_);

	//向き回転
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);

	//移動
	trans_.pos = VAdd(trans_.pos, targetVec);

	//移動量の初期化
	moveSpeed_ = 0.0f;
}

Enemy::ATK& Enemy::CreateSkillSign(ATK_ACT _atkAct)
{
	//**********************************************************
	//使い終わった攻撃がある場合
	//**********************************************************

	//使い終わった攻撃に上書き
	for (auto& nowSkillSign : nowSkillSign_)
	{
		if (nowSkillSign.IsFinishMotion())
		{
			//スキル上書き
			nowSkillSign = skills_[_atkAct];

			//カウンタの初期化
			nowSkillSign.ResetCnt();

			//ヒット判定の初期化
			nowSkillSign.isHit_ = false;

			//処理終了
			return nowSkillSign;
		}
	}

	//**********************************************************
	//ない場合
	//**********************************************************

	//ランダムでとってきた攻撃の種類を今から発動するスキルに設定
	nowSkillSign_.emplace_back(skills_[_atkAct]);

	//カウンタの初期化
	nowSkillSign_.back().ResetCnt();

	//ヒット判定の初期化
	nowSkillSign_.back().isHit_ = false;

	//処理終了
	return nowSkillSign_.back();
}

Enemy::ATK& Enemy::CreateSkill(ATK_ACT _atkAct)
{
	//**********************************************************
	//使い終わった攻撃がある場合
	//**********************************************************

	//使い終わった攻撃に上書き
	for (auto& nowSkill : nowSkill_)
	{
		if (nowSkill.IsFinishMotion())
		{
			//スキル上書き
			nowSkill = skills_[_atkAct];
			
			//カウンタの初期化
			nowSkill.ResetCnt();

			//ヒット判定の初期化
			nowSkill.isHit_ = false;

			//処理終了
			return nowSkill;
		}
	}

	//**********************************************************
	//ない場合
	//**********************************************************

	//ランダムでとってきた攻撃の種類を今から発動するスキルに設定
	nowSkill_.emplace_back(skills_[_atkAct]);

	//カウンタの初期化
	nowSkill_.back().ResetCnt();

	//ヒット判定の初期化
	nowSkill_.back().isHit_ = false;

	//処理終了
	return nowSkill_.back();
}

void Enemy::FinishAnim(void)
{
	//アニメーション判定
	switch (anim_)
	{
	case UnitBase::ANIM::IDLE:
	case UnitBase::ANIM::WALK:
	case UnitBase::ANIM::RUN:
		//ループ再生
		stepAnim_ = 0.0f;
		break;

	case UnitBase::ANIM::SKILL_1:
	case UnitBase::ANIM::SKILL_2:
	case UnitBase::ANIM::DAMAGE:
	case UnitBase::ANIM::DEATH:
	case UnitBase::ANIM::ENTRY:
		break;
	}
}

void Enemy::ResetAtkJudge(void)
{
	//攻撃終了判定の初期化
	isEndAllAtk_ = false;
}

void Enemy::RandSkill(void)
{
	//スキルの数
	size_t size = skills_.size();

	//スキルの数分からランダムを取る
	int rand = GetRand(size - 1);

	//スキル
	atkAct_ = static_cast<ATK_ACT>(rand);

	//スキル生成準備
	SetUpSkill(atkAct_);

	//スキルに対応した予備動作アニメーションの記録
	nowSkillPreAnim_ = skillPreAnims_[static_cast<int>(atkAct_)];

	//スキルに対応したアニメーションの記録
	nowSkillAnim_ = skillAnims_[static_cast<int>(atkAct_)];

	//スキルの予兆生成
	createSkillSign_ = std::bind(&Enemy::CreateSkillSign, this, atkAct_);

	//スキル生成
	createSkill_ = std::bind(&Enemy::CreateSkill, this, atkAct_);
}

void Enemy::SetUpSkill(ATK_ACT _atkAct)
{
	//警告情報をセット
	alertNowSkill_ = alertSkills_[_atkAct];

	//攻撃情報をセット
	processSkill_ = changeSkill_[_atkAct];
}