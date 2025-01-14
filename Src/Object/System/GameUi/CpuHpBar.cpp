#include "CpuHpBar.h"
#include "../../../Manager/ResourceManager.h"
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

void CpuHpBar::SetParam()
{
	//種類
	typeHpBar_ = HPBAR_TYPE::CPU;
}

void CpuHpBar::Draw()
{
	VECTOR pos = ConvWorldPosToScreenPos(pos_);

	DrawRotaGraph(
	pos.x,pos.y,
		IMG_RATE,
		0.0f,
		imgHpGage_,
		true,
		false);

	//デバッグ関連変数
	int hpMax = ChickenBase::DEFAULT_LIFE;
	int parcent = (int)SIZE.x % (hpMax * hp_);
	pos = VAdd(pos, LOCAL_HPBAR_POS);

	DrawExtendGraph(
		pos.x - SIZE.x, pos.y - SIZE.y,
		pos.x + parcent, pos.y + SIZE.y,
		imgHpBar_,
		true);
}
