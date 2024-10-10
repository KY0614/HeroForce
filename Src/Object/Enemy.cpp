#include<DxLib.h>
#include"../Manager/Resource.h"
#include"../Manager/InputManager.h"
#include"../Manager/ResourceManager.h"
#include"../Utility/AsoUtility.h"
#include "Enemy.h"

Enemy::Enemy()
{
}

void Enemy::Destroy(void)
{
}

void Enemy::Init(void)
{
	//モデル読み込み
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_AXEMAN));

	float scl = 0.2f;

	//変数の初期化
	trans_.scl = { scl,scl,scl };
	trans_.pos = { 0.0f,0.0f,0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
	
	state_ = STATE::NML;

	atkCdt_ = 0.0f;

	trans_.Update();
}

void Enemy::Update(void)
{
	//攻撃クールダウン中なら待機
	if (atkCdt_ < ATK_CDT)state_ = STATE::NML;
	else state_ = STATE::ATK;

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

	//モデル制御
	trans_.Update();
}

void Enemy::UpdateNml(void)
{
	//クールダウンカウンタ
	atkCdt_++;

	//移動処理
	Move();
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
	//DrawSphere3D(trans_.pos, 10.0f, 10, 0xff00ff, 0xff00ff, true);

	MV1DrawModel(trans_.modelId);
}

void Enemy::Move(void)
{
	//入力用
	InputManager& ins = InputManager::GetInstance();

	//移動(デバッグ)
	if (ins.IsNew(KEY_INPUT_UP)) { ProcessMove(5.0f, 0.0f); }
	if (ins.IsNew(KEY_INPUT_RIGHT)){ ProcessMove(5.0f, 90.0f); }
	if (ins.IsNew(KEY_INPUT_DOWN)){ ProcessMove(5.0f, 180.0f); }
	if (ins.IsNew(KEY_INPUT_LEFT)){ ProcessMove(5.0f, 270.0f); }
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

void Enemy::Attack(void)
{
	//攻撃クールダウンカウンタ初期化
	atkCdt_ = 0.0f;
}

void Enemy::CreateAtk(void)
{
	//有効な攻撃を取得
	ATK atk = GetValidAtk();

	
}

const Enemy::ATK Enemy::GetValidAtk(void)
{
	for (auto& atk : atk_)
	{
		if (atk.duration_)
		{
			//カウンタを初期化してもう一度再利用する


			return atk;
		}
	}

	//新たな攻撃配列を生成
	ATK atk;
	
	//カウンタの初期化

	//新たに作った攻撃を配列に挿入

	//作った攻撃を返す
	return ATK();
}
