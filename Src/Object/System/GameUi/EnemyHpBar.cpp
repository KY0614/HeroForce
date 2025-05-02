#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/GameSystem/CharacterParamData.h"
#include "../../Character/EnemySort/Enemy.h"
#include "EnemyHpBar.h"

EnemyHpBar::EnemyHpBar()
{
}

void EnemyHpBar::Load()
{
	//�ǂݍ���
	GamaUIBase::Load();

	//�摜
	imgHpBar_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::HP_ENEMY).handleId_;
}

void EnemyHpBar::SetParam()
{
	//���
	typeHpBar_ = HPBAR_TYPE::ENEMY;
}

void EnemyHpBar::Draw(Enemy& enemy)
{
	//hp�̔��f
	hp_ = enemy.GetHp();

	//hp��0�ȉ��̏ꍇ�ȉ��̏������s��Ȃ�
	if (hp_ <= 0) { return; }

	//�f�o�b�O�֘A�ϐ�
	VECTOR pos = ConvWorldPosToScreenPos(enemy.GetPos());
	pos = VAdd(pos, LOCAL_HPBAR_POS);

	//hp�o�[�̒������v�Z
	int hpMax = enemy.GetHpMax();
	float divSize = SIZE.x * 2 / hpMax;
	int barLength = static_cast<int>(divSize * hp_);

	//�Q�[�W�̕`��
	DrawRotaGraph(
		pos.x, pos.y,
		IMG_RATE,
		0.0f,
		imgHpGage_,
		true,
		false);

	//�o�[�̕`��
	DrawExtendGraph(
		pos.x - SIZE.x, pos.y - SIZE.y + 3,
		pos.x - SIZE.x + barLength, pos.y + SIZE.y + 2,
		imgHpBar_,
		true);
}
