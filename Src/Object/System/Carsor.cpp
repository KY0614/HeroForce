#include <DxLib.h>
#include "../Manager/Generic/InputManager.h"
#include "Carsor.h"

Carsor::Carsor()
{

	key_.right_ = -1;
	key_.left_ = -1;
	key_.up_ = -1;
	key_.down_ = -1;
	key_.decide_ = -1;
	img_ = -1;
	pos_ = { -1,-1 };
	decide_ = false;
}

Carsor::~Carsor()
{
}

void Carsor::Init()
{
	pos_ = { 100,100 };
}

void Carsor::Update()
{
	auto& ins = InputManager::GetInstance();

	//ÉLÅ[ëÄçÏ
	if (ins.IsNew(key_.right_)) 
	{ pos_.x += MOVE_POW; }
	if (ins.IsNew(key_.left_))	{ pos_.x -= MOVE_POW; }
	if (ins.IsNew(key_.up_))	{ pos_.y -= MOVE_POW; }
	if (ins.IsNew(key_.down_))	{ pos_.y += MOVE_POW; }

	//åàíË
	if (ins.IsTrgDown(key_.decide_))
	{
		if (!decide_) { decide_ = true; }
		else if (decide_) { decide_ = false; }
	}
}

void Carsor::Draw()
{
	DrawCircle(pos_.x, pos_.y, RADIUS,
		0x00ff00, true);
}

void Carsor::Reset()
{
	decide_ = false;
}

void Carsor::Load()
{
}

void Carsor::SetContllorKey(const int right, const int left, const int up, const int down, const int decide)
{
	key_.right_ = right;
	key_.left_ = left;
	key_.up_ = up;
	key_.down_ = down;
	key_.decide_ = decide;
}
