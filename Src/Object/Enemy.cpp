#include<DxLib.h>
#include"../Manager/Resource.h"
#include"../Manager/InputManager.h"
#include"../Manager/ResourceManager.h"
#include"../Utility/AsoUtility.h"
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

	float scl = 0.2f;

	//共通の変数の初期化
	trans_.scl = { scl,scl,scl };
	trans_.pos = { 0.0f,0.0f,0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
	state_ = STATE::NORMAL;
	atkCdt_ = 0.0f;
	stunDef_ = 0;

	//攻撃情報の初期化
	InitSkill();
	nowSkill_.ResetCnt();

	trans_.Update();
}

void Enemy::SetParam(void)
{
	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_AXEMAN));

	//アニメーション番号の初期化
	InitAnimNum();
	ResetAnim(ANIM::WALK);

	exp_ = 1.0f;
	hp_ = 5;
	stunDefMax_ = 100;
}

void Enemy::Update(void)
{
	//やられているなら何もしない
	if (!IsAlive()) { return; }

	//状態ごとのUpdate
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		UpdateNml();
		break;

	case Enemy::STATE::ALERT_ATTACK:
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
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Q)) { Damage(1); }

	//モデル制御
	trans_.Update();

	//アニメーション
	Anim();
}

void Enemy::Damage(const int _damage)
{
	//ダメージ
	hp_ -= _damage;
	if (!IsAlive()){ ResetAnim(ANIM::DEATH); }
}

void Enemy::InitAnimNum()
{
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
		ChangeState(STATE::ALERT_ATTACK);

		return;
	}

	//アニメーション変更(一度だけ)
	if (moveSpeed_ == 0.0f && anim_ != ANIM::IDLE) { ResetAnim(ANIM::IDLE); }

	//クールダウンカウンタ
	atkCdt_++;

	//移動処理
	Move();
}

void Enemy::UpdateAlert(void)
{
	//攻撃クールダウンが終わったなら攻撃開始
	if (!IsAtkCD())
	{
		//攻撃クールダウンカウンタ初期化
		atkCdt_ = 0.0f;

		//攻撃状態に遷移
		ChangeState(STATE::ATTACK);

		return;
	}

	//クールダウンカウンタ
	atkCdt_++;

	//生成が終わってないなら生成する
	if (nowSkill_.IsFinishMotion()) 
	{
		//ランダムで攻撃生成
		RandSkill();
	}
}

void Enemy::UpdateAtk(void)
{
	//攻撃が終わったら状態遷移
	if (nowSkill_.IsFinishMotion())
	{
		//攻撃クールダウンカウンタ初期化
		atkCdt_ = 0.0f;

		//休憩状態に遷移
		ChangeState(STATE::BREAK);

		return;
	}

	//攻撃のカウンタ
	nowSkill_.cnt_++;

	//攻撃処理
	Attack();
}

void Enemy::UpdateBreak(void)
{
	if (atkBtc_ > ATK_BTC)
	{
		//攻撃休憩時間の初期化
		atkBtc_ = 0;

		//通常状態に遷移
		ChangeState(STATE::NORMAL);
		return;
	}

	//攻撃休憩時間カウンタ
	atkBtc_++;
}

void Enemy::Search(void)
{
}

void Enemy::Draw(void)
{
	//デバッグ
	//DrawFormatString(0, 0, IsAtkCD() ? 0xffffff : 0xff0000, IsAtkCD() ? "攻撃クールダウン中" : "クールダウン終わり！！");
	//DrawFormatString(0, 16, nowSkill_.IsFinishMotion() ? 0xffffff : 0xff0000, nowSkill_.IsFinishMotion() ? "攻撃中！！" : "攻撃終了！！");
	DrawFormatString(0, 16, 0xffffff, "HP = %d", hp_);
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		DrawFormatString(0, 0, 0x00ff00, "現在状態:NORMAL");
		break;

	case Enemy::STATE::ALERT_ATTACK:
		DrawFormatString(0, 0, 0x00ff00, "現在状態:ALERT_ATTACK");
		break;

	case Enemy::STATE::ATTACK:
		DrawFormatString(0, 0, 0x00ff00, "現在状態:ATTACK");
		break;

	case Enemy::STATE::BREAK:
		DrawFormatString(0, 0, 0x00ff00, "現在状態:BREAK");
		break;
	}

	if (!IsAlive()) { return; }

	//敵モデルの描画
	MV1DrawModel(trans_.modelId);

	//攻撃の描画
	if (nowSkill_.IsAttack()) { DrawSphere3D(nowSkill_.pos_, 25.0f, 50.0f, 0xff0f0f, 0xff0f0f, true); }
	else if (nowSkill_.IsBacklash_()) { DrawSphere3D(nowSkill_.pos_, 20.0f, 50.0f, 0xff0f0f, 0xff0f0f, false); }
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
}

void Enemy::ProcessMove(const float _moveSpeed, const float _deg)
{
	//アニメーション変更(一度だけ)
	if (anim_ != ANIM::WALK) { ResetAnim(ANIM::WALK); }

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
	skill_.emplace_back(SKILL_1);
	skill_.emplace_back(SKILL_2);

	//初期スキルを設定しておく
	RandSkill();
}

void Enemy::Attack(void)
{
	//前方向
	VECTOR dir = trans_.quaRot.GetForward();

	//座標の設定
	nowSkill_.pos_ = VAdd(trans_.pos, VScale(dir, 20.0f));
}

void Enemy::RandSkill(void)
{
	//スキルの数
	size_t size = skill_.size();

	//スキルの数分からランダムを取る
	int rand = GetRand(size - 1);

	//ランダムでとってきた攻撃の種類を今から発動するスキルに設定
	nowSkill_ = skill_[rand];
}
