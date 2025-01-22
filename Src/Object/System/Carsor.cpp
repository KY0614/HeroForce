#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "Carsor.h"

Carsor::Carsor()
{
	key_.right_ = -1;
	key_.left_ = -1;
	key_.up_ = -1;
	key_.down_ = -1;
	key_.decide_ = -1;

	pos_ = { -1,-1 };
	playerNum_ = -1;
	decide_ = false;
	rate_ = -1.0f;
	img_ = -1;
}

Carsor::~Carsor()
{
}

void Carsor::Init(const int plNum,const int img)
{
	//プレイヤーナンバー
	playerNum_ = plNum;

	//画像
	img_ = img;

	//パッド情報
	switch (playerNum_)
	{
	case 0:
		pad_ = InputManager::JOYPAD_NO::PAD1;
		break;
	case 1:
		pad_ = InputManager::JOYPAD_NO::PAD2;
		break;
	case 2:
		pad_ = InputManager::JOYPAD_NO::PAD3;
		break;
	case 3:
		pad_ = InputManager::JOYPAD_NO::PAD4;
		break;
	}

	//初期設定
	Reset();
}

void Carsor::Update()
{
	//決定済みの場合
	if (decide_) { return; }

	auto& ins = InputManager::GetInstance();
	int stickX = ins.GetJPadInputState(pad_).AKeyLX;
	int stickY = ins.GetJPadInputState(pad_).AKeyLY;

	//操作処理
	if (ins.IsNew(key_.right_) || stickX > 0) 
	{ 
		pos_.x += MOVE_POW; 
		if (pos_.x > Application::SCREEN_SIZE_X)
		{
			pos_.x = Application::SCREEN_SIZE_X;
		}
	}
	if (ins.IsNew(key_.left_) || stickX < 0)	
	{
		pos_.x -= MOVE_POW; 
		if (pos_.x < 0)
		{
			pos_.x = 0;
		}
	}
	if (ins.IsNew(key_.up_) || stickY < 0)	
	{ 
		pos_.y -= MOVE_POW; 
		if (pos_.y < 0)
		{
			pos_.y = 0;
		}
	}
	if (ins.IsNew(key_.down_) || stickY > 0)	
	{ 
		pos_.y += MOVE_POW;
		if (pos_.y > Application::SCREEN_SIZE_Y)
		{
			pos_.y = Application::SCREEN_SIZE_Y;
		}
	}

	//決定
	if (ins.IsTrgDown(key_.decide_) ||
		ins.IsPadBtnTrgDown(pad_, InputManager::JOYPAD_BTN::RIGHT))
	{
		decide_ = true;
	}
}

void Carsor::Draw()
{
	float angle = AsoUtility::Deg2RadF(ANGLE);
	bool trans = true;
	bool revers = false;

	//決定済みの場合は描画しない
	if (decide_) { return; }

	//アイコン描画
	DrawRotaGraph(
		pos_.x,
		pos_.y,
		rate_,
		angle,
		img_,
		trans,
		revers);
}

void Carsor::Reset()
{
	//決定
	decide_ = false;

	//座標
	pos_ = { DEFAULT_POS_X * playerNum_,DEFAULT_POS_Y};

	//拡大率
	rate_ = SCALE_RATE;
}

void Carsor::SetContllorKey(const int right, const int left, const int up, const int down, const int decide)
{
	key_.right_ = right;
	key_.left_ = left;
	key_.up_ = up;
	key_.down_ = down;
	key_.decide_ = decide;
}

void Carsor::SetDecide(const bool value)
{
	decide_ = value;
}
