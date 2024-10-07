#include "Enemy.h"
#include "EnemyAtk.h"

EnemyAtk::EnemyAtk(void)
{
}

void EnemyAtk::Destroy(void)
{
}

void EnemyAtk::Init(Enemy* _parent)
{
	//親
	enemy_ = _parent;

	//変数の初期化
	trans_.scl = { 1.0f,1.0f,1.0f };
	trans_.pos = enemy_->GetPos();
	trans_.quaRot = enemy_->GetRot();
	trans_.quaRotLocal = Quaternion();

	atkDuration_ = 0.0f;
}

void EnemyAtk::Update(void)
{
	if (!IsAlive()) { return; }

	//攻撃の持続時間カウント
	atkDuration_++;
}

void EnemyAtk::Draw(void)
{
	if (!IsAlive()) { return; }
	DrawSphere3D(trans_.pos, 2.0f, 20, 0xff0000, 0xff0000, true);
}