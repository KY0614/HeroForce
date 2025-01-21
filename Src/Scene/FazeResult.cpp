#include<DxLib.h>
#include<cassert>
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/ResourceManager.h"
#include"../Application.h"

#include "FazeResult.h"
FazeResult::FazeResult(void)
{
	exp_ = 0.0f;
	afterExp_ = 0.0f;
	rank_ = RANK::MAX;
	isEnd_ = false;
}
FazeResult::~FazeResult(void)
{
}
void FazeResult::Init(void)
{
	rankImg_[static_cast<int>(RANK::S)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANK_S).handleId_;
	rankImg_[static_cast<int>(RANK::A)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANK_A).handleId_;
	rankImg_[static_cast<int>(RANK::B)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANK_B).handleId_;
	rankImg_[static_cast<int>(RANK::C)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANK_C).handleId_;
	backImg_= ResourceManager::GetInstance().Load(ResourceManager::SRC::RESULT).handleId_;

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
	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	//背景
	DrawRotaGraph(
		cx, cy,
		0.7f, 0.0f, backImg_, true);

	//評価値UI  
	DrawRotaGraph(
		cx, cy - 100,
		0.3f, 0.0f, rankImg_[static_cast<int>(rank_)], true);
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
