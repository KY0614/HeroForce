#include "FazeResult.h"
#include<cassert>

void FazeResult::Init(void)
{
}

void FazeResult::Update(void)
{
}

void FazeResult::Draw(void)
{
}

void FazeResult::Release(void)
{
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
