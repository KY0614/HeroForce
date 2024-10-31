#include <DxLib.h>
#include "../Application.h"
#include "../../Manager/ResourceManager.h"
#include "PowerSelect.h"
#include "Carsor.h"

PowerSelect::PowerSelect()
{
	img_ = -1;
	element_ = 4;
	state_ = STATE::NONE;
}

PowerSelect::~PowerSelect()
{
}

void PowerSelect::Init()
{
	int num = 1;	//プレイヤーの数
	carsors_.resize(num);
	for (int i = 0; i < carsors_.size(); i++)
	{
		carsors_[i] = *new Carsor();
		carsors_[i].Init();
		carsors_[i].SetContllorKey(
		KEY_INPUT_RIGHT,
		KEY_INPUT_LEFT,
		KEY_INPUT_UP,
		KEY_INPUT_DOWN,
		KEY_INPUT_RETURN);
	}
	

	//画像読み込み
	Load();
	
	element_ = 4;
	ele_.resize(element_);

	//変数の設定
	Reset();
}

void PowerSelect::Update()
{
	//拡大量
	float scl = 0.01f;
	float max = 0.8f;

	switch (state_)
	{
	case STATE::NONE:
		break;

	case STATE::EXPANSION:
		for (int i = 0; i < ele_.size(); i++)
		{
			ele_[i].scl_ += scl;
			if (ele_[i].scl_ >= max)
			{
				ele_[i].scl_ = max;
				state_ = STATE::SELECT;
			}
		}
		break;

	case STATE::SELECT:	
		for (int i = 0; i < carsors_.size(); i++)
		{
			carsors_[i].Update();
			//決定処理
			for (auto ele : ele_)
			{
				if (carsors_[i].IsDecide())
				{
					state_ = STATE::FIN;
				}
			}
		}
		break;

	case STATE::FIN:
		break;
		
	};
}

void PowerSelect::Draw()
{
	for (auto ele : ele_)
	{
		DrawRotaGraph(
			ele.pos_.x, ele.pos_.y,
			ele.scl_,
			0.0f,
			img_,
			true,
			false);
	}

	for (int i = 0; i < carsors_.size(); i++)
	{
		carsors_[i].Draw();
	}
}

void PowerSelect::Reset()
{
	for (int i = 0; i < ele_.size(); i++)
	{
		ele_[i].scl_ = 0.00f;
	}
	state_ = STATE::NONE;

	//強化項目の設定
	SetFirstPos();

	//カーソル初期化
	for (int i = 0; i < carsors_.size(); i++)
	{
		carsors_[i].Reset();
	}
}

void PowerSelect::Load()
{
	img_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SELECT_UI).handleId_;
	if (img_ == -1)
	{
		return;
	}
}

void PowerSelect::SetFirstPos()
{
	Vector2 pos;
	int cnt = 0;
	int div = 4;
	int center= 2;

	switch (ele_.size())
	{
	case 2:
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / div, Application::SCREEN_SIZE_Y / 2 };
		cnt++;
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / div * 3, Application::SCREEN_SIZE_Y / 2 };

	case 3:
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / div, Application::SCREEN_SIZE_Y / div };
		cnt++;
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / div, Application::SCREEN_SIZE_Y / div * 3};
		cnt++;
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / div * 3};
		break;

	case 4:
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / div, Application::SCREEN_SIZE_Y / div };
		cnt++;
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / div * 3, Application::SCREEN_SIZE_Y / div };
		cnt++;
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / div, Application::SCREEN_SIZE_Y / div * 3 };
		cnt++;
		ele_[cnt].pos_ = { Application::SCREEN_SIZE_X / div * 3, Application::SCREEN_SIZE_Y / div * 3 };

		break;
	default:
		return;
		break;
	}
}

void PowerSelect::SetState(const STATE state)
{
	state_ = state;
}
