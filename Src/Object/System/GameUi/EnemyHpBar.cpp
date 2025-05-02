#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/GameSystem/CharacterParamData.h"
#include "../../Character/EnemySort/Enemy.h"
#include "EnemyHpBar.h"

EnemyHpBar::EnemyHpBar()
{
}

void EnemyHpBar::Load()
{
	//読み込み
	GamaUIBase::Load();

	//画像
	imgHpBar_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::HP_ENEMY).handleId_;
}

void EnemyHpBar::SetParam()
{
	//種類
	typeHpBar_ = HPBAR_TYPE::ENEMY;
}

void EnemyHpBar::Draw(Enemy& enemy)
{
	//hpの反映
	hp_ = enemy.GetHp();

	//hpが0以下の場合以下の処理を行わない
	if (hp_ <= 0) { return; }

	//デバッグ関連変数
	VECTOR pos = ConvWorldPosToScreenPos(enemy.GetPos());
	pos = VAdd(pos, LOCAL_HPBAR_POS);

	//hpバーの長さを計算
	int hpMax = enemy.GetHpMax();
	float divSize = SIZE.x * 2 / hpMax;
	int barLength = static_cast<int>(divSize * hp_);

	//ゲージの描画
	DrawRotaGraph(
		pos.x, pos.y,
		IMG_RATE,
		0.0f,
		imgHpGage_,
		true,
		false);

	//バーの描画
	DrawExtendGraph(
		pos.x - SIZE.x, pos.y - SIZE.y + 3,
		pos.x - SIZE.x + barLength, pos.y + SIZE.y + 2,
		imgHpBar_,
		true);
}
