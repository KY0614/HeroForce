#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../../Common/Vector2.h"
#include "LevelBase.h"

LevelBase::LevelBase(void)
{
	exp_ = 0.0f;
	nowLevel_ = 0;
	gauge_ = 0.0f;
}

LevelBase::~LevelBase(void)
{
}

void LevelBase::Destroy(void)
{
}

void LevelBase::Init(void)
{
	//画像の読み込み
	Load();

	//初期状態の設定
	gauge_ = 30;
}

void LevelBase::Update(void)
{
	Debag();

	CheckExp();
}

void LevelBase::Draw(void)
{
	Vector2 pos = { 0,0 };
	DrawFormatString(pos.x,pos.y,0xffffff,"現在の経験値%2f", exp_);

	pos = { 0,16 };
	DrawFormatString(pos.x,pos.y,0xffffff,"現在のレベル%d", nowLevel_);
}

void LevelBase::Load(void)
{
}

void LevelBase::SetExp(float value)
{
	exp_ += value;
}

void LevelBase::SetGage(int level)
{
	//敵の経験値量を決めて修正予定
	gauge_ = level * CONSTANT_GAGE;
}

void LevelBase::CheckExp()
{
	//経験値が次のレベル条件まで達したら
	if (exp_ >= gauge_)
	{
		nowLevel_++;	//レベルを上げる
		exp_ = 0;		//経験値の初期化
	}
}

void LevelBase::Debag()
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsNew(KEY_INPUT_B))
	{
		SetExp(1);
	}
}
