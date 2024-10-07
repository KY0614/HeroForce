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
	//�e
	enemy_ = _parent;

	//�ϐ��̏�����
	trans_.scl = { 1.0f,1.0f,1.0f };
	trans_.pos = enemy_->GetPos();
	trans_.quaRot = enemy_->GetRot();
	trans_.quaRotLocal = Quaternion();

	atkDuration_ = 0.0f;
}

void EnemyAtk::Update(void)
{
	if (!IsAlive()) { return; }

	//�U���̎������ԃJ�E���g
	atkDuration_++;
}

void EnemyAtk::Draw(void)
{
	if (!IsAlive()) { return; }
	DrawSphere3D(trans_.pos, 2.0f, 20, 0xff0000, 0xff0000, true);
}