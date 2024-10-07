#include<DxLib.h>
#include"../Manager/Resource.h"
#include"../Manager/InputManager.h"
#include"../Manager/ResourceManager.h"
#include"../Utility/AsoUtility.h"
#include"EnemyAtk.h"
#include "Enemy.h"

Enemy::Enemy()
{
}

void Enemy::Destroy(void)
{
}

void Enemy::Init(void)
{
	//trans_.SetModel()

	//変数の初期化
	trans_.scl = { 1.0f,1.0f,1.0f };
	trans_.pos = { 0.0f,0.0f,0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion();
	
	state_ = STATE::NML;

	atkCdt_ = 0.0f;
	
	trans_.Update();
}

void Enemy::Update(void)
{
	//攻撃クールダウン中なら待機
	atkCdt_ < ATK_CDT
		? state_ = STATE::NML
		: state_ = STATE::ATK;

	//状態ごとのUpdate
	switch (state_)
	{
	case Enemy::STATE::NML:
		UpdateNml();
		break;

	case Enemy::STATE::ATK:
		UpdateAtk();
		break;

	case Enemy::STATE::DEAD:
		UpdateDead();
		break;
	}
}

void Enemy::UpdateNml(void)
{
	//クールダウンカウンタ
	atkCdt_++;

	//移動処理
	Move(5.0f);
}

void Enemy::UpdateAtk(void)
{
	//攻撃処理
	Attack();
}

void Enemy::UpdateDead(void)
{
}

void Enemy::Draw(void)
{
	DrawSphere3D(trans_.pos, 10.0f, 10, 0xff00ff, 0xff00ff, true);
}

std::vector<EnemyAtk*> Enemy::GetAtk(void)
{
	return atk_;
}

void Enemy::Move(const float _moveSpeed)
{
	//入力用
	InputManager& ins = InputManager::GetInstance();

	//移動方向
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	//移動(デバッグ)
	if (ins.IsNew(KEY_INPUT_UP))
	{
		dir = VAdd(dir, trans_.quaRot.GetForward());
	}
	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		dir = VAdd(dir, trans_.quaRot.GetRight());
	}
	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		dir = VAdd(dir, trans_.quaRot.GetBack());
	}
	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		dir = VAdd(dir, trans_.quaRot.GetLeft());
	}

	//移動量
	VECTOR movePow = VScale(dir, _moveSpeed);

	//移動
	trans_.pos = VAdd(trans_.pos, movePow);
}

void Enemy::Attack(void)
{
	//攻撃の生成
	CreateAtk();

	//攻撃クールダウンカウンタ初期化
	atkCdt_ = 0.0f;
}

void Enemy::CreateAtk(void)
{
	//有効な攻撃を取得
	EnemyAtk* atk = GetValidAtk();

	//攻撃の生成
	atk->Init(this);
}

EnemyAtk* Enemy::GetValidAtk(void)
{
	//使用しきった配列を探す
	for (auto& atk : atk_)
	{
		//役目が終わっているなら
		if (!atk->IsAlive())
		{
			//再利用
			return atk;
		}
	}

	//ないので新しく攻撃用に配列を追加
	EnemyAtk* newAtk = new EnemyAtk();
	atk_.emplace_back(newAtk);

	//新しく作ったのを返す
	return newAtk;
}
