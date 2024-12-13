#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/InputManager.h"
#include "../../Common/Vector2.h"
#include "../Common/Fader.h"
#include "LevelupNotice.h"
#include "LevelupSelect.h"

LevelScreenManager::LevelScreenManager(void)
{
	notice_ = nullptr;
	select_ = nullptr;
	exp_ = -1.0f;
	nowLevel_ = -1;
	gauge_ = -1.0f;
	alpha_ = -1.0f;
	state_ = STATE::NONE;

	// 状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&LevelScreenManager::ChangeStateNone, this));
	stateChanges_.emplace(STATE::NOTICE, std::bind(&LevelScreenManager::ChangeStateNotice, this));
	stateChanges_.emplace(STATE::SELECT, std::bind(&LevelScreenManager::ChangeStateSelect, this));
	stateChanges_.emplace(STATE::END, std::bind(&LevelScreenManager::ChangeStateEnd, this));
}

LevelScreenManager::~LevelScreenManager(void)
{
}

void LevelScreenManager::Init(void)
{
	//レベルの初期設定
	nowLevel_ = DEFAULT_LEVEL;

	//インスタンス設定
	Load();

	//初期化処理
	Reset();
}

void LevelScreenManager::Update(void)
{
	// 更新ステップ
	stateUpdate_();
}

void LevelScreenManager::ChangeState(const STATE state)
{
	// 状態受け取り
	state_ = state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();
}

void LevelScreenManager::ChangeStateNone()
{	
	stateUpdate_ = std::bind(&LevelScreenManager::UpdateNone, this);
	stateDraw_ = std::bind(&LevelScreenManager::DrawNone, this);
}

void LevelScreenManager::ChangeStateNotice()
{
	stateUpdate_ = std::bind(&LevelScreenManager::UpdateNotice, this);
	stateDraw_ = std::bind(&LevelScreenManager::DrawNotice, this);
}

void LevelScreenManager::ChangeStateSelect()
{
	stateUpdate_ = std::bind(&LevelScreenManager::UpdateSelect, this);
	stateDraw_ = std::bind(&LevelScreenManager::DrawSelect, this);
}

void LevelScreenManager::ChangeStateEnd()
{
	stateUpdate_ = std::bind(&LevelScreenManager::UpdateEnd, this);
	stateDraw_ = std::bind(&LevelScreenManager::DrawEnd, this);
}

void LevelScreenManager::UpdateNone(void)
{
	//デバッグ処理(Bで経験値を得る)
	DebagUpdate();

	//経験値の確認
	CheckExp();
}

void LevelScreenManager::UpdateNotice(void)
{
	//文字の拡大	
	notice_->Update();	

	//背景画面の透過処理
	alpha_+= ALPHA_SPEED;
	if (alpha_ >= ALPHA_MAX)
	{
		alpha_ = ALPHA_MAX;
	}

	//処理の終了確認
	if (notice_->GetState() == LevelupNotice::STATE::FIN)
	{
		ChangeState(STATE::SELECT);
	}
}

void LevelScreenManager::UpdateSelect(void)
{
	//プレイヤーの強化の選択処理
	select_->Update();	

	//処理の終了確認
	if (select_->GetState() == LevelupSelect::STATE::FIN)
	{
		ChangeState(STATE::END);
	}
}

void LevelScreenManager::UpdateEnd(void)
{
	//強化要素設定
	for (int i = 0; i < playerNum_; i++) {
		selectTypes_[i] = select_->GetType(i);
	}

	//各種リセット
	ChangeState(STATE::NONE);	//状態変化
	notice_->Reset();			//通知処理のリセット
	select_->Reset();			//選択処理のリセット
	alpha_ = 0.0f;
}

void LevelScreenManager::Draw(void)
{
	Vector2 pos = { 0,0 };

	//処理中はフェードを行う
	if(state_ != STATE::NONE){ FaderDraw(); }

	//状態別描画処理
	stateDraw_();

	//デバッグ描画
	DebagDraw();
}

void LevelScreenManager::DrawNone()
{
	//~(°ω°)~
}

void LevelScreenManager::DrawNotice()
{
	notice_->Draw();
}

void LevelScreenManager::DrawSelect()
{
	select_->Draw();
}

void LevelScreenManager::DrawEnd()
{
}

void LevelScreenManager::Release(void)
{
	notice_->Release();
	select_->Release();
}

void LevelScreenManager::Load(void)
{
	//各インスタンス読み込み
	notice_ = std::make_unique<LevelupNotice>();
	notice_->Init();

	select_ = std::make_unique<LevelupSelect>();
	select_->Init();
}

void LevelScreenManager::Reset()
{
	//各プレイヤーごとの強化要素初期化
	playerNum_ = 2;	//仮プレイヤー人数
	selectTypes_.resize(playerNum_, TYPE::MAX);

	//経験値ゲージ量
	gauge_ = 30;

	//初期経験値
	exp_ = 0.0f;

	//画面透過値
	alpha_ = 0.0f;

	//状態
	ChangeState(STATE::NONE);
}

void LevelScreenManager::AddExp(const float value)
{
	exp_ += value;
}

void LevelScreenManager::SetGage(const int level)
{
	//敵の経験値量を決めて修正予定
	gauge_ = level * CONSTANT_GAGE;
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
		
		//通知に移る			
		ChangeState(STATE::NOTICE);

		//レベルの取得
		notice_->SetNewLevel(nowLevel_);
	}
}

inline LevelScreenManager::TYPE LevelScreenManager::GetType(const int playerNum) const
{
	return selectTypes_[playerNum];
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

void LevelScreenManager::DebagUpdate()
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsNew(KEY_INPUT_B))
	{
		AddExp(1);
	}
}

void LevelScreenManager::DebagDraw()
{
	Vector2 pos = { 0,0 };
	DrawFormatString
	(pos.x, pos.y, 0xffffff, "現在の経験値%2f", exp_);
	pos = { 0,16 };
	DrawFormatString
	(pos.x, pos.y, 0xffffff, "現在のレベル%d", nowLevel_);
	pos = { 0,32 };
	DrawFormatString
	(pos.x, pos.y, 0xffffff, "現在のステート%d", static_cast<int>(state_));
}