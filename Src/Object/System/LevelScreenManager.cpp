#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/InputManager.h"
#include "../../Common/Vector2.h"
#include "../Common/Fader.h"
#include "LevelScreenManager.h"
#include "LevelupNotice.h"
#include "LevelupSelect.h"

LevelScreenManager::LevelScreenManager(void)
{
	fader_ = nullptr;
	notice_ = nullptr;
	select_ = nullptr;
	exp_ = -1.0f;
	nowLevel_ = -1;
	gauge_ = -1.0f;
	alpha_ = -1.0f;
	state_ = STATE::NONE;
	type_ = TYPE::MAX;
}

LevelScreenManager::~LevelScreenManager(void)
{
}

void LevelScreenManager::Init(void)
{
	//インスタンス設定
	Load();

	//初期化処理
	Reset();
}

void LevelScreenManager::Update(void)
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

void LevelScreenManager::NoticeUpdate(void)
{
	//Faderの更新処理
	fader_->Update();
	
	//文字の拡大	
	notice_->Update();	

	//画面の透過処理
	alpha_+= ALPHA_SPEED;
	if (alpha_ >= ALPHA_MAX)
	{
		alpha_ = ALPHA_MAX;
	}

	//状態変更の確認
	if (notice_->GetState() == LevelupNotice::STATE::FIN)
	{
		state_ = STATE::SELECT;					//処理のステートを選択にする
		select_->SetState					
		(LevelupSelect::STATE::EXPANSION);		//選択処理のステートを変えとく
	}
}

void LevelScreenManager::PowerUpdate(void)
{
	select_->Update();						//プレイヤーの強化の選択処理

	if (select_->GetState() == LevelupSelect::STATE::FIN)
	{
		type_ = select_->GetType();	//強化要素の取得
		state_ = STATE::FIN;		//強化反映の処理へ移る
		notice_->Reset();			//通知処理のリセット
		select_->Reset();			//選択処理のリセット
		alpha_ = 0.0f;
	}
}

void LevelScreenManager::Draw(void)
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
		select_->Draw();
		break;	
	};
}

void LevelScreenManager::Release(void)
{
	delete select_;
	delete notice_;
	delete fader_;
}

void LevelScreenManager::Load(void)
{
	//各インスタンス読み込み
	fader_ = new Fader();
	fader_->Init();
	fader_->SetAlpha(Fader::LITTLE_ALPHA);

	notice_ = new LevelupNotice();
	notice_->Init();

	select_ = new LevelupSelect();
	select_->Init();
}

void LevelScreenManager::Reset()
{
	//初期状態の設定
	gauge_ = 30;
	exp_ = 0.0f;
	alpha_ = 0.0f;
}

void LevelScreenManager::SetExp(const float value)
{
	exp_ += value;
}

void LevelScreenManager::SetGage(const int level)
{
	//敵の経験値量を決めて修正予定
	gauge_ = level * CONSTANT_GAGE;
}

void LevelScreenManager::SetState(const STATE state)
{
	state_ = state;
}

void LevelScreenManager::CheckExp()
{
	//経験値が次のレベル条件まで達したら
	if (exp_ >= gauge_)
	{
		//レベルを上げる
		nowLevel_++;	
		
		//経験値の初期化
		exp_ = 0.0f;					

		//画像のフェード処理の設定;						
		notice_->SetState
		(LevelupNotice::STATE::FADE_IN);

		//画面全体のフェード処理の設定
		fader_->SetFade
		(Fader::STATE::SET_FADE_OUT);
		
		//通知に移る			
		SetState(STATE::NOTICE);
	}
}

void LevelScreenManager::FaderDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
	DrawBox(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void LevelScreenManager::Debag()
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsNew(KEY_INPUT_B))
	{
		SetExp(1);
	}
}
