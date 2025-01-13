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
		//�f�o�b�O�p
		assert("�����N�K�p�O�̂��̂��g�p����Ă��܂��B");
		break;
	}

	return exp;
}
