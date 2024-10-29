#include<DxLib.h>
#include"../../Application.h"
#include"../../Manager/Resource.h"
#include"../../Manager/InputManager.h"
#include"../../Manager/ResourceManager.h"
#include"../../Utility/AsoUtility.h"
#include "Enemy.h"

Enemy::Enemy()
{
	atkCdt_ = 0.0f;
	exp_ = 1.0f;
	hp_ = 5;
	moveSpeed_ = 0.0f;
	state_ = STATE::NORMAL;
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

	//※デバッグ　仮置きアニメーション
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_1);
	animNum_.emplace(ANIM::SKILL_2, ANIM_SKILL_2);

	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);

	float scl = 0.2f;

	//共通の変数の初期化
	trans_.scl = { scl,scl,scl };
	trans_.pos = AsoUtility::VECTOR_ZERO;
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
	state_ = STATE::NORMAL;
	atkCdt_ = 0.0f;
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
	colRadius_ = MY_COL_RADIUS;
	exp_ = 1.0f;
	hp_ = 5;
	stunDefMax_ = 100;
}

void Enemy::Update(void)
{
	//アニメーション
	Anim();

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

	//※デバッグ　ダメージ処理
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Q)) { Damage(1, 10); }

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
	//攻撃クールダウンが終わったなら攻撃準備開始
	if (!IsAtkCD())
	{
		//攻撃クールダウンカウンタ初期化
		atkCdt_ = 0.0f;

		//攻撃警告状態に遷移
		ChangeState(STATE::ALERT);

		return;
	}

	//待機アニメーション
	if (moveSpeed_ == 0.0) { ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM); }

	//※いずれ消す
	//クールダウンカウンタ
	atkCdt_++;

	//移動処理
	Move();
}

void Enemy::UpdateAlert(void)
{
	//警告カウンタが終わったなら攻撃開始
	if (!IsAlertTime())
	{
		//警告カウンタ初期化
		alertCnt_ = 0.0f;

		//攻撃状態に遷移
		ChangeState(STATE::ATTACK);

		return;
	}

	//クールダウンカウンタ
	alertCnt_++;

	//生成が終わってないなら生成する
	if (atk_.IsFinishMotion())
	{
		//ランダムで攻撃生成
		RandSkill();
	}
}

void Enemy::UpdateAtk(void)
{
	//攻撃が終わっているなら状態遷移
	if (atk_.IsFinishMotion())
	{
		//休憩状態に遷移
		ChangeState(STATE::BREAK);

		return;
	}

	//**********************************************************
	//攻撃中の処理
	//**********************************************************

	//攻撃アニメーション
	ResetAnim(nowSkillAnim_, DEFAULT_SPEED_ANIM);

	//攻撃のカウンタ
	atk_.cnt_++;

	//攻撃処理
	Attack();
}

void Enemy::UpdateBreak(void)
{
	if (!IsBreak())
	{
		//攻撃休憩時間の初期化
		breakCnt_ = 0;

		//通常状態に遷移
		ChangeState(STATE::NORMAL);
		return;
	}

	//待機アニメーション
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);

	//攻撃休憩時間カウンタ
	breakCnt_++;
}

void Enemy::Search(void)
{
}

void Enemy::Draw(void)
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
	DrawSphere3D(colPos_, colRadius_, 4, 0xffff00, 0xffff00, false);

	//敵モデルの描画
	MV1DrawModel(trans_.modelId);

	//攻撃の描画
	if (atk_.IsAttack()) { DrawSphere3D(atk_.pos_, nowSkillColRadius_, 50.0f, 0xff0f0f, 0xff0f0f, true); }
	else if (atk_.IsBacklash()) { DrawSphere3D(atk_.pos_, nowSkillColRadius_, 5.0f, 0xff0f0f, 0xff0f0f, false); }
}

void Enemy::Move(void)
{
	//移動量の初期化
	moveSpeed_ = 0.0f;

	//入力用
	InputManager& ins = InputManager::GetInstance();

	//移動(デバッグ)
	if (ins.IsNew(KEY_INPUT_UP)) { moveSpeed_ = WALK_SPEED; ProcessMove(moveSpeed_, 0.0f); }
	if (ins.IsNew(KEY_INPUT_RIGHT)){ moveSpeed_ = WALK_SPEED; ProcessMove(moveSpeed_, 90.0f); }
	if (ins.IsNew(KEY_INPUT_DOWN)){ moveSpeed_ = WALK_SPEED; ProcessMove(moveSpeed_, 180.0f); }
	if (ins.IsNew(KEY_INPUT_LEFT)){ moveSpeed_ = WALK_SPEED; ProcessMove(moveSpeed_, 270.0f); }

	//歩きアニメーション
	if(moveSpeed_ > 0.0f)ResetAnim(ANIM::WALK, DEFAULT_SPEED_ANIM);
}

void Enemy::ProcessMove(const float _moveSpeed, const float _deg)
{
	//方向転換
	Turn(_deg, AsoUtility::AXIS_Y);

	//移動方向
	VECTOR dir = trans_.quaRot.GetForward();

	//移動量
	VECTOR movePow = VScale(dir, _moveSpeed);

	//移動
	trans_.pos = VAdd(trans_.pos, movePow);
}

void Enemy::Turn(float _deg, VECTOR _axis)
{
	//trans_.quaRot = trans_.quaRot.Mult(trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis));
	trans_.quaRot = trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}

void Enemy::InitSkill(void)
{
	//ここにスキルの数分格納させる
	skills_.emplace_back(SKILL_1);
	skills_.emplace_back(SKILL_2);

	//ここにスキルの数分アニメーションを格納させる
	skillAnims_.emplace_back(ANIM::SKILL_1);
	skillAnims_.emplace_back(ANIM::SKILL_2);

	//ここにスキルの当たり判定半径を格納させる
	skillColRadius_.emplace_back(SKILL_1_COL_RADIUS);
	skillColRadius_.emplace_back(SKILL_2_COL_RADIUS);

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

	//座標の設定
	atk_.pos_ = VAdd(colPos_, VScale(dir, nowSkillColRadius_));
}

void Enemy::RandSkill(void)
{
	//スキルの数
	size_t size = skills_.size();

	//スキルの数分からランダムを取る
	int rand = GetRand(size - 1);

	//ランダムでとってきた攻撃の種類を今から発動するスキルに設定
	atk_ = skills_[rand];
	nowSkillAnim_ = skillAnims_[rand];
	nowSkillColRadius_ = skillColRadius_[rand];
}
