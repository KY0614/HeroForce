#include<DxLib.h>
#include<cassert>
#include"../Manager/Generic/InputManager.h"

#include "FazeResult.h"
void FazeResult::Init(void)
{
	rankString_[static_cast<int>(RANK::S)] = 'S';
	rankString_[static_cast<int>(RANK::A)] = 'A';
	rankString_[static_cast<int>(RANK::B)] = 'B';
	rankString_[static_cast<int>(RANK::C)] = 'C';

	rank_ = RANK::S;
	isEnd_ = false;

	//獲得経験値量はフェーズ終了時のプレイヤーたちの経験値をデータバンクから取得する
	exp_ = 1500.0f;
}

void FazeResult::Update(void)
{
	ChangeRank();
}

void FazeResult::Draw(void)
{
	DrawFormatString(0, 20, 0x000000, "RANK=%c", rankString_[static_cast<int>(rank_)]);
	DrawFormatString(0, 40, 0x000000, "GetExp=%d", static_cast<int>(afterExp_));
}

void FazeResult::Release(void)
{
}

void FazeResult::Reset(void)
{
	isEnd_ = false;
}

float FazeResult::GetBonusExp(const RANK _rank) const
{
	float exp = exp_;

	switch (_rank)
	{
	case RANK::S:
		exp = exp * BNS_RANK_S;
		break;
	case RANK::A:
		exp = exp * BNS_RANK_A;
		break;
	case RANK::B:
		exp = exp * BNS_RANK_B;
		break;
	case RANK::C:
		exp = exp * BNS_RANK_C;
		break;
	default:
		//デバッグ用
		assert("ランク適用外のものが使用されています。");
		break;
	}

	return exp;
}

void FazeResult::ChangeRank(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_S))rank_ = RANK::S;
	if (ins.IsTrgDown(KEY_INPUT_A))rank_ = RANK::A;
	if (ins.IsTrgDown(KEY_INPUT_B))rank_ = RANK::B;
	if (ins.IsTrgDown(KEY_INPUT_C))rank_ = RANK::C;
	if (ins.IsTrgDown(KEY_INPUT_SPACE))isEnd_ = true;

	afterExp_ = GetBonusExp(rank_);
}
