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

void EnemyHpBar::Draw(Enemy& _enemy)
{
	//hpの反映
	hp_ = _enemy.GetHp();

	//hpが0以下の場合以下の処理を行わない
	if (hp_ <= 0) { return; }

	//デバッグ関連変数
	VECTOR pos = ConvWorldPosToScreenPos(_enemy.GetPos());
	pos = VAdd(pos, LOCAL_HPBAR_POS);

	//hpバーの長さを計算
	int hpMax = _enemy.GetHpMax();
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
		pos.x - SIZE.x, 
		pos.y - SIZE.y + HP_BAR_OFFSET_TOP,
		pos.x - SIZE.x + barLength, 
		pos.y + SIZE.y + HP_BAR_OFFSET_BOTTOM,
		imgHpBar_,
		true);
}
