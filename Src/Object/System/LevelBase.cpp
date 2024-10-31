#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "../../Common/Vector2.h"
#include "../Common/Fader.h"
#include "LevelBase.h"
#include "LevelupNotice.h"
#include "PowerSelect.h"

LevelBase::LevelBase(void)
{
	fader_ = nullptr;
	notice_ = nullptr;
	power_ = nullptr;
	exp_ = 0.0f;
	nowLevel_ = 0;
	gauge_ = 0.0f;
	state_ = STATE::NONE;
}

LevelBase::~LevelBase(void)
{
}

void LevelBase::Destroy(void)
{
}

void LevelBase::Init(void)
{
	//インスタンス設定
	Load();

	//初期状態の設定
	gauge_ = 30;
}

void LevelBase::Update(void)
{
	switch (state_)
	{
	case STATE::NONE:
		Debag();		//デバッグ処理(Bで経験値を得る)
		CheckExp();		//経験値の確認
		break;

	case STATE::NOTICE:
		NoticeUpdate();	//通知処理
		break;

	case STATE::SELECT:
		PowerUpdate();	//選択処理
		break;

	};
	
}

void LevelBase::NoticeUpdate(void)
{
	//Faderの更新処理
	fader_->Update();
	
	//文字の拡大	
	notice_->Update();						

	if (notice_->GetState() == LevelupNotice::STATE::FIN)
	{
		state_ = STATE::SELECT;					//処理のステートを選択にする
		power_->SetState					
		(PowerSelect::STATE::EXPANSION);		//選択処理のステートを変えとく
	}
}

void LevelBase::PowerUpdate(void)
{
	power_->Update();						//プレイヤーの強化の選択処理

	if (power_->GetState() == PowerSelect::STATE::FIN)
	{
		state_ = STATE::NONE;
		notice_->Reset();
		power_->Reset();
	}
}

void LevelBase::Draw(void)
{
	Vector2 pos = { 0,0 };

	//処理中はフェードを行う
	if(state_ != STATE::NONE){ FaderDraw(); }

	switch (state_)
	{
	case STATE::NONE:
	DrawFormatString
	(pos.x,pos.y,0xffffff,"現在の経験値%2f", exp_);
	pos = { 0,16 };
	DrawFormatString
	(pos.x,pos.y,0xffffff,"現在のレベル%d", nowLevel_);
		break;

	case STATE::NOTICE:
		notice_->Draw();
		break;	
	
	case STATE::SELECT:
		power_->Draw();
		break;	
	};
}

void LevelBase::Release(void)
{
	delete power_;
	delete notice_;
	delete fader_;
}

void LevelBase::Load(void)
{
	fader_ = new Fader();
	fader_->Init();

	notice_ = new LevelupNotice();
	notice_->Init();

	power_ = new PowerSelect();
	power_->Init();
}

void LevelBase::SetExp(const float value)
{
	exp_ += value;
}

void LevelBase::SetGage(const int level)
{
	//敵の経験値量を決めて修正予定
	gauge_ = level * CONSTANT_GAGE;
}

void LevelBase::SetState(const STATE state)
{
	state_ = state;
}

void LevelBase::CheckExp()
{
	//経験値が次のレベル条件まで達したら
	if (exp_ >= gauge_)
	{
		//レベルを上げる
		nowLevel_++;	
		
		//経験値の初期化
		exp_ = 0;					

		//画像のフェード処理の設定;						
		notice_->SetState
		(LevelupNotice::STATE::FADE_IN);

		//画面全体のフェード処理の設定
		fader_->SetFade
		(Fader::STATE::FADE_NOTICE);
		
		//通知に移る			
		SetState(STATE::NOTICE);
	}
}

void LevelBase::FaderDraw()
{
	fader_->Draw();
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
