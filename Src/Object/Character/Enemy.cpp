#include<DxLib.h>
#include<cmath>
#include"../../Application.h"
#include"../../Manager/Resource.h"
#include"../../Manager/InputManager.h"
#include"../../Manager/ResourceManager.h"
#include"../../Manager/Collision.h"
#include"../../Utility/AsoUtility.h"
#include "Enemy.h"

Enemy::Enemy()
{
	exp_ = 1.0f;
	hp_ = 5;
	moveSpeed_ = 0.0f;
	state_ = STATE::NORMAL;
	anim_ = ANIM::NONE;
}

void Enemy::Destroy(void)
{
	animNum_.clear();
}

void Enemy::Init(void)
{
	//キャラ固有設定
	SetParam();

	//アニメーション番号の初期化
	InitAnimNum();

#ifdef DEBUG_ENEMY
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_1);
	animNum_.emplace(ANIM::SKILL_2, ANIM_SKILL_2);
	targetPos_ = { -30.0f, 0.0f,-30.0f };
#endif // DEBUG_ENEMY

	//アニメーションリセット
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);

	//共通の変数の初期化
	trans_.scl = { CHARACTER_SCALE,CHARACTER_SCALE,CHARACTER_SCALE };
	trans_.pos = AsoUtility::VECTOR_ZERO;
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
	state_ = STATE::NORMAL;
	alertCnt_ = 0.0f;
	breakCnt_ = 0.0f;
	stunDef_ = 0;
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);

	//当たり判定の設定
	radius_ = MY_COL_RADIUS;
	//攻撃情報の初期化
	InitSkill();
	atk_.ResetCnt();

	trans_.Update();
}

void Enemy::SetParam(void)
{
	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_AXEMAN));

	//※個々で設定する
	radius_ = MY_COL_RADIUS;
	exp_ = 1.0f;
	hp_ = 5;
	stunDefMax_ = 100;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
}

void Enemy::Update(void)
{
	//アニメーション
	Anim();

#ifdef DEBUG_ENEMY
	//入力用
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_W)) { targetPos_.z+= 3.0f; }
	if (ins.IsNew(KEY_INPUT_D)) { targetPos_.x+= 3.0f; }
	if (ins.IsNew(KEY_INPUT_S)) { targetPos_.z-= 3.0f; }
	if (ins.IsNew(KEY_INPUT_A)) { targetPos_.x-= 3.0f; }

	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Q)) { Damage(1, 10); }
#endif // DEBUG_ENEMY


	//やられているなら何もしない
	if (!IsAlive()) { return; }

	//当たり判定座標の更新
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);

	//状態ごとのUpdate
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		UpdateNml();
		break;

	case Enemy::STATE::ALERT:
		UpdateAlert();
		break;

	case Enemy::STATE::ATTACK:
		UpdateAtk();
		break;

	case Enemy::STATE::BREAK:
		UpdateBreak();
		break;
	}

	//モデル制御
	trans_.Update();
}

void Enemy::Damage(const int _damage, const int _stunPow)
{
	//既にやられているなら処理しない
	if (!IsAlive()) { return; }

	//ダメージ
	hp_ -= _damage;

	//スタン値
	stunDef_ += _stunPow;

	//やられたら死亡アニメーション
	if (!IsAlive()){ ResetAnim(ANIM::DEATH, DEFAULT_SPEED_ANIM); }
}

void Enemy::ChangeState(const STATE _state)
{
	//状態遷移
	state_ = _state;

	//状態遷移における初期化
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		break;
	
	case Enemy::STATE::ALERT:
		//警告カウンタ初期化
		alertCnt_ = 0.0f;
		break;

	case Enemy::STATE::ATTACK:
		break;
	
	case Enemy::STATE::BREAK:
		//攻撃休憩時間の初期化
		breakCnt_ = 0;
		break;
	}
}

void Enemy::InitAnimNum()
{
	//共通アニメーション
	animNum_.emplace(ANIM::IDLE, ANIM_IDLE);
	animNum_.emplace(ANIM::WALK, ANIM_WALK);
	animNum_.emplace(ANIM::RUN, ANIM_RUN);
	animNum_.emplace(ANIM::DAMAGE, ANIM_DAMAGE);
	animNum_.emplace(ANIM::DEATH, ANIM_DEATH);
	animNum_.emplace(ANIM::ENTRY, ANIM_ENTRY);
}

void Enemy::UpdateNml(void)
{
	//**********************************************************
	//終了処理
	//**********************************************************


	/*ゲームシーンにあります*/


	//**********************************************************
	//動作処理
	//**********************************************************

	//待機アニメーション
	if (moveSpeed_ == 0.0)ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
	//歩きアニメーション
	else if (moveSpeed_ > 0.0f)ResetAnim(ANIM::WALK, DEFAULT_SPEED_ANIM);

	//移動量の初期化
	moveSpeed_ = 0.0f;
	
	//索敵
	if (!isMove_)return;

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

	//クールダウンカウンタ
	alertCnt_++;

	//生成が終わってないなら生成する
	if (nowSkill_.front().IsFinishMotion())
	{
		//ランダムで攻撃生成
		RandSkill();
	}
}

void Enemy::UpdateAtk(void)
{
	//攻撃が終わっているなら状態遷移
	if (nowSkill_.back().IsFinishMotion())
	{
		//休憩状態に遷移
		ChangeState(STATE::BREAK);
		return;
	}

	//**********************************************************
	//動作処理
	//**********************************************************

	//攻撃アニメーション
	ResetAnim(nowSkillAnim_, DEFAULT_SPEED_ANIM);

	for (auto& nowSkill : nowSkill_)
	{
		//攻撃のカウンタ
		nowSkill.cnt_++;
	}

	//攻撃処理
	Attack();
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
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);

	//攻撃休憩時間カウンタ
	breakCnt_++;
}

void Enemy::Draw(void)
{
#ifdef DEBUG_ENEMY
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
	//ターゲットの座標
	DrawSphere3D(targetPos_, 3.0f, 10, 0x0000ff, 0x0000ff, true);
#endif // DEBUG_ENEMY

	//敵モデルの描画
	MV1DrawModel(trans_.modelId);

	for (auto& nowSkill : nowSkill_)
	{
		//攻撃の描画
		if (nowSkill.IsAttack()) { DrawSphere3D(nowSkill.pos_, nowSkill.radius_, 50.0f, 0xff0f0f, 0xff0f0f, true); }
		else if (nowSkill.IsBacklash()) { DrawSphere3D(nowSkill.pos_, nowSkill.radius_, 5.0f, 0xff0f0f, 0xff0f0f, false); }
	}
	//攻撃の描画
	//if (atk_.IsAttack()) { DrawSphere3D(atk_.pos_, nowSkillColRadius_, 50.0f, 0xff0f0f, 0xff0f0f, true); }
	//else if (atk_.IsBacklash()) { DrawSphere3D(atk_.pos_, nowSkillColRadius_, 5.0f, 0xff0f0f, 0xff0f0f, false); }
}

const VECTOR Enemy::GetTargetVec(void)const
{
	//標的への方向ベクトルを取得						※TODO:ベクトルはSceneGameからもらう
	VECTOR targetVec = VSub(targetPos_, trans_.pos);

	//正規化
	targetVec = VNorm(targetVec);

	//Y座標は必要ないので要素を消す
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//移動量を求める
	VECTOR ret = VScale(targetVec, moveSpeed_);

	return ret;
}

void Enemy::Move(void)
{
	//移動速度の更新
	moveSpeed_ = WALK_SPEED;

	//方向ベクトル取得
	VECTOR targetVec = GetTargetVec();

	//回転
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);

	//移動
	trans_.pos = VAdd(trans_.pos, targetVec);
}

void Enemy::InitSkill(void)
{
	//ここにスキルの数分格納させる
	skills_.emplace_back(SKILL_1);
	skills_.emplace_back(SKILL_2);

	//ここにスキルの数分アニメーションを格納させる
	skillAnims_.emplace_back(ANIM::SKILL_1);
	skillAnims_.emplace_back(ANIM::SKILL_2);

	//初期スキルを設定しておく
	RandSkill();
}

void Enemy::FinishAnim(void)
{
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

void Enemy::Attack(void)
{
	//前方向
	VECTOR dir = trans_.quaRot.GetForward();

	for (auto& nowSkill : nowSkill_)
	{
		//座標の設定
		nowSkill.pos_ = VAdd(colPos_, VScale(dir, nowSkill.radius_));
	}
	//座標の設定
	//atk_.pos_ = VAdd(colPos_, VScale(dir, nowSkillColRadius_));
}

void Enemy::RandSkill(void)
{
	//スキルの数
	size_t size = skills_.size();

	//スキルの数分からランダムを取る
	int rand = GetRand(size - 1);

	//**********************************************************
	//使い終わった攻撃がある場合
	//**********************************************************
	
	//使い終わった攻撃に上書き
	for (auto& nowSkill : nowSkill_)
	{
		if (nowSkill.IsFinishMotion())
		{
			//上書き
			nowSkill = skills_[rand];
			
			//カウンタの初期化
			nowSkill.ResetCnt();

			//処理終了
			return;
		}
	}

	//**********************************************************
	//ない場合
	//**********************************************************

	//ランダムでとってきた攻撃の種類を今から発動するスキルに設定
	nowSkill_.emplace_back(skills_[rand]);
	//atk_ = skills_[rand];
	nowSkillAnim_ = skillAnims_[rand];

	//カウンタの初期化
	nowSkill_.back().ResetCnt();
}