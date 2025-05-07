#include "CpuHpBar.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/GameSystem/CharacterParamData.h"
#include "../../Character/Chiken/ChickenBase.h"

CpuHpBar::CpuHpBar()
{
}

CpuHpBar::~CpuHpBar()
{
}

void CpuHpBar::Load()
{
	//読み込み
	GamaUIBase::Load();

	//画像
	imgHpBar_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::HP_CPU).handleId_;
}


void CpuHpBar::Draw()
{

	if (hp_ <= 0) { return; }

	VECTOR pos = ConvWorldPosToScreenPos(pos_);

	DrawRotaGraph(
	pos.x,pos.y,
		IMG_RATE,
		0.0f,
		imgHpGage_,
		true,
		false);

	//デバッグ関連変数
	int hpMax = CharacterParamData::GetInstance().GetParamData(CharacterParamData::UNIT_TYPE::CHICKEN).hp_;
	float divSize = SIZE.x * 2 / hpMax;
	pos = VAdd(pos, LOCAL_HPBAR_POS);

	int barLength = static_cast<int>(divSize * hp_);

	DrawExtendGraph(
		pos.x - SIZE.x, pos.y - SIZE.y,
		pos.x - SIZE.x + barLength,pos.y + SIZE.y,
		imgHpBar_,
		true);
}
