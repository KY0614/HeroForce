#include<DxLib.h>
#include<cassert>
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/GameSystem/TextManager.h"
#include"../Manager/GameSystem/DataBank.h"

#include "FazeResult.h"
FazeResult::FazeResult(void)
{
	exp_ = 0.0f;
	afterExp_ = 0.0f;
	rank_ = RANK::MAX;
	isEnd_ = false;
	imgBack_ = -1;

	int i = -1;
	imgRank_ = &i;
	state_ = STATE::NOMAL;
}
FazeResult::~FazeResult(void)
{
}
void FazeResult::Init(void)
{
	//画像格納
	rankImg_[static_cast<int>(RANK::S)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANK_S).handleId_;
	rankImg_[static_cast<int>(RANK::A)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANK_A).handleId_;
	rankImg_[static_cast<int>(RANK::B)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANK_B).handleId_;
	rankImg_[static_cast<int>(RANK::C)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANK_C).handleId_;
	backImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RESULT).handleId_;

	rank_ = RANK::S;
	isEnd_ = false;

	//獲得経験値量はフェーズ終了時のプレイヤーたちの経験値をデータバンクから取得する
	exp_ = 1500.0f;

	//画像
	imgBack_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::REZALT_BACK).handleId_;
	imgRank_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANKS).handleIds_;

	//フォント生成
	font_ = CreateFontToHandle(
		TextManager::GetInstance().GetFontName(TextManager::FONT_TYPE::LOGO).c_str(),
		FONT_SIZE,
		0);

	heasdFont_ = CreateFontToHandle(
		TextManager::GetInstance().GetFontName(TextManager::FONT_TYPE::LOGO).c_str(),
		FONT_HEAD_SIZE,
		0);

	SetResult();
}

void FazeResult::Update(void)
{
	ChangeRank();
	step_++;
}

void FazeResult::Draw(void)
{
	//背景
	DrawExtendGraph(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		imgBack_,
		true
	);

	int fezeCnt = 1;
	std::string mes = "%dフェーズ終了";
	//テキストの描画
	DrawFormatStringToHandle(
		HEAD_TEXT_POS_X,
		HEAD_TEXT_POS_Y - mes.length() * FONT_HEAD_SIZE / 2,
		0xffffff,
		font_,
		mes.c_str(),
		fezeCnt);

	if (step_ < INTERVEL) { return; }
	DrawFormatStringToHandle(
		MES1_POS_X, MES1_POS_Y,
		0xffffff,
		font_,
		"敵を倒した総数 : %d体",
		dunkEnm_);

	if (state_ == STATE::NOMAL) {
		if (step_ < INTERVEL * 2) { return; }
		DrawFormatStringToHandle(
			MES2_POS_X, MES2_POS_Y,
			0xffffff,
			font_,
			"ニワトリの生存数 : %d体",
			aliveChicken_);
	}

	//経験値の描画
	if (step_ < INTERVEL * 3) { return; }
	DrawFormatStringToHandle(
		MES3_POS_X, MES3_POS_Y,
		0xffffff,
		font_,
		"Exp.%d",
		static_cast<int>(afterExp_));


	//ランクの描画
	if (step_ < INTERVEL * 4) { return; }
	DrawRotaGraph(
		RANK_POS_X,
		RANK_POS_Y,
		0.85f,
		0.0f,
		imgRank_[static_cast<int>(rank_)],
		true,
		false);

}

void FazeResult::Release(void)
{
}

void FazeResult::Reset(void)
{
	isEnd_ = false;
}

void FazeResult::SetLast(void)
{
	dunkEnm_ = DataBank::GetInstance().Output(DataBank::INFO::GAME_DUNK_ENEMY);
}

void FazeResult::SetResult(void)
{
	//リザルト情報
	dunkEnm_ = DataBank::GetInstance().Output(DataBank::INFO::FAZE_DUNK_ENEMY);
	aliveChicken_ = DataBank::GetInstance().Output(DataBank::INFO::ALIVE_CHICKEN);
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

	//フェーズの終了
	if (ins.IsTrgDown(KEY_INPUT_SPACE)||ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1,InputManager::JOYPAD_BTN::RIGHT))isEnd_ = true;

	afterExp_ = GetBonusExp(rank_);
}
