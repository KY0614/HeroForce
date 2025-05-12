#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Decoration/SoundManager.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Decoration/EffectManager.h"
#include "../../Manager/GameSystem/DataBank.h"
#include "../../Common/Vector2.h"
#include "../../Common/Fader.h"
#include "../../Common/ShaderFade.h"
#include "LevelNotice.h"
#include "LevelSelect.h"

LevelScreenManager::LevelScreenManager()
{
	notice_ = nullptr;
	select_ = nullptr;
	fader_ = nullptr;
	exp_ = -1.0f;
	restExp_ = -1.0f;
	nowLevel_ = -1;
	gauge_ = -1.0f;
	alpha_ = -1.0f;
	state_ = STATE::NONE;
	isFader_ = false;

	// 状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&LevelScreenManager::ChangeStateNone, this));
	stateChanges_.emplace(STATE::NOTICE, std::bind(&LevelScreenManager::ChangeStateNotice, this));
	stateChanges_.emplace(STATE::SELECT, std::bind(&LevelScreenManager::ChangeStateSelect, this));
	stateChanges_.emplace(STATE::END, std::bind(&LevelScreenManager::ChangeStateEnd, this));
}

LevelScreenManager::~LevelScreenManager()
{
}

void LevelScreenManager::Init()
{
	//レベルの初期設定
	nowLevel_ = DEFAULT_LEVEL;
	restExp_ = 0;

	//インスタンス設定
	Load();

	//初期化処理
	Reset();

	//前データの保持
	preTypeData_.resize(playerNum_, TYPE::MAX);
}

void LevelScreenManager::Update()
{
	//フェード更新処理
	fader_->Update();

	stateUpdate_();
}

void LevelScreenManager::ChangeState(const STATE _state)
{
	// 状態受け取り
	state_ = _state;

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

	//SEの再生
	SoundManager::GetInstance().Play(SoundManager::SOUND::LEVEL_UP);
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

void LevelScreenManager::UpdateNone()
{
	//デバッグ処理(Bで経験値を得る)
	//DebagUpdate();

	//経験値の確認
	CheckExp();
}

void LevelScreenManager::UpdateNotice()
{
	if (!isFader_) {	
		
		//スキップ
		SkipState(STATE::SELECT);

		//文字の拡大
		notice_->Update();

		//背景画面の透過処理
		alpha_ += ALPHA_SPEED;
		if (alpha_ >= ALPHA_MAX)
		{
			alpha_ = ALPHA_MAX;
		}

		//処理の終了確認
		if (notice_->GetState() == LevelNotice::STATE::FIN)
		{
			isFader_ = true;
			fader_->SetFade(Fader::STATE::FADE_OUT);
		}
	}
	else
	{
		Fade();
	}
}

void LevelScreenManager::UpdateSelect()
{
	if (!isFader_)
	{
		Fade();
	}
	else
	{
		//プレイヤーの強化の選択処理
		select_->Update();

		//処理の終了確認
		if (select_->GetState() == LevelSelect::STATE::FIN)
		{
			ChangeState(STATE::END);
		}
	}
}

void LevelScreenManager::UpdateEnd()
{
	//強化要素設定
	for (int i = 0; i < playerNum_; i++) {
		selectTypes_[i] = select_->GetType(i);
		preTypeData_[i] = selectTypes_[i];
	}

	//各種リセット
	ChangeState(STATE::NONE);	//状態変化
	notice_->Reset();			//通知処理のリセット
	select_->Reset();			//選択処理のリセット
	alpha_ = 0.0f;
	isFader_ = false;
}

void LevelScreenManager::Draw()
{
	Vector2 pos = { 0,0 };

	DrawLevelUI();
	
	//処理中はフェードを行う
	if(state_ != STATE::NONE){ FaderDraw(); }

	//状態別描画処理
	stateDraw_();

	//デバッグ描画
	//DebagDraw();

	//フェーダー描画
	fader_->Draw();
}

void LevelScreenManager::DrawNone()
{
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

void LevelScreenManager::DrawLevelUI()
{
	Vector2 pos = {
		GAGE_IMG_SIZE / 2 + GAGE_POS_X,
		GAGE_IMG_SIZE / 2 + GAGE_POS_Y };
	float percent = 100.0f / gauge_ * exp_;

	//経験値ゲージ
	DrawRotaGraph(pos.x, pos.y,
		GAGE_SCALE_RATE, 0.0f, imgGage_, true, false);

	//経験値
	DrawCircleGauge(pos.x, pos.y,
		percent, imgGageExp_, 0.0f, GAGE_SCALE_RATE);

	//1桁の場合
	if (nowLevel_ < AsoUtility::TWO_DIGIT_MIN)
	{	
		//1桁の場合
		DrawRotaGraph(pos.x, pos.y, NUM_SCALE_RATE, 0.0f, imgNumbers_[nowLevel_], true, false);
	}
	else
	{	
		//2桁の場合
		int leftNum = nowLevel_ / AsoUtility::TWO_DIGIT_MIN;
		int rightNum = nowLevel_ % AsoUtility::TWO_DIGIT_MIN;

		//左数字
		Vector2 leftPos = pos;
		leftPos.x -= RIGHT_LOCAL_POS_X;
		DrawRotaGraph(leftPos.x, leftPos.y, NUM_SCALE_RATE, 0.0f, imgNumbers_[leftNum], true, false);

		//右数字
		Vector2 rightPos = pos;
		rightPos.x += LEFT_LOCAL_POS_X;
		DrawRotaGraph(rightPos.x, rightPos.y, NUM_SCALE_RATE, 0.0f, imgNumbers_[rightNum], true, false);
	}
}

void LevelScreenManager::Release()
{
	notice_->Release();
	select_->Release();
}

void LevelScreenManager::Load()
{
	//各インスタンス読み込み
	notice_ = std::make_unique<LevelNotice>();
	notice_->Init();

	select_ = std::make_unique<LevelSelect>();
	select_->Init();

	fader_ = std::make_unique<ShaderFade>();
	fader_->Init();

	//画像
	imgGage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CIRCLE_GAGE).handleId_;
	imgGageExp_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CIRCLE_EXP_GAGE).handleId_;
	imgNumbers_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::NUMBERS).handleIds_;

	//エフェクト関係
	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::ATTACK_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::ATTACK_UP_EFE).handleId_);

	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::DEFENCE_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::DEFENCE_UP_EFE).handleId_);

	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::SPEED_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::SPEED_UP_EFE).handleId_);

	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::LIFE_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::LIFE_UP_EFE).handleId_);

	//SEの登録
	SoundManager::GetInstance().Add(
		SoundManager::TYPE::SE,
		SoundManager::SOUND::LEVEL_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::LEVEL_UP_SE).handleId_);

	SoundManager::GetInstance().Add(
		SoundManager::TYPE::SE,
		SoundManager::SOUND::POWER_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::POWER_UP_SE).handleId_);
}

void LevelScreenManager::Reset()
{
	//各プレイヤーごとの強化要素初期化
	playerNum_ = DataBank::GetInstance().Output(DataBank::INFO::USER_NUM);
	selectTypes_.resize(playerNum_, TYPE::MAX);

	//経験値ゲージ量
	gauge_ = CONSTANT_GAGE;

	//初期経験値
	exp_ = 0.0f;

	//画面透過値
	alpha_ = 0.0f;

	//状態
	ChangeState(STATE::NONE);
}

void LevelScreenManager::SetGage(const int _level)
{
	gauge_ = CONSTANT_GAGE * (1.0f + (_level / LEVEL_DIV));
}

void LevelScreenManager::Reflection(PlayerBase& _player, const int _playerNum)
{
	//反映効果
	TYPE type = selectTypes_[_playerNum];

	switch (type)
	{
	case TYPE::ATTACK:
		_player.SetAttack(ADD_ATK_PER);

		//変更箇所
		_player.SetPreStatus();

		//引数の値は上昇%の値
		//現在のステータスを%で上昇させる
		EffectManager::GetInstance().Play(
			EffectManager::EFFECT::ATTACK_UP,
			_player.GetPos(),
			Quaternion(),
			EFFECT_SCALE,SoundManager::SOUND::POWER_UP);
		break;

	case TYPE::DEFENSE:
		_player.SetDefense(ADD_DEF_PER);

		//変更箇所
		_player.SetPreStatus();

		EffectManager::GetInstance().Play(
			EffectManager::EFFECT::DEFENCE_UP,
			_player.GetPos(),
			Quaternion(),
			EFFECT_SCALE, SoundManager::SOUND::POWER_UP);
		break;

	case TYPE::LIFE:
		_player.SetHpMax(ADD_LIFE);
		EffectManager::GetInstance().Play(
			EffectManager::EFFECT::LIFE_UP,
			_player.GetPos(),
			Quaternion(),
			EFFECT_SCALE, SoundManager::SOUND::POWER_UP);

		break;

	case TYPE::SPEED:
		_player.SetSpeed(ADD_SPEED_PER);

		//変更箇所
		_player.SetPreStatus();

		EffectManager::GetInstance().Play(
			EffectManager::EFFECT::SPEED_UP,
			_player.GetPos(),
			Quaternion(),
			EFFECT_SCALE, SoundManager::SOUND::POWER_UP);
		break;

	default:
		break;
	}


	selectTypes_[_playerNum] = TYPE::MAX;
}

void LevelScreenManager::CheckExp()
{
	//経験値を1ずつ増やす
	if (restExp_ > 0)
	{
		restExp_-= EXP_SPEED;
		exp_+= EXP_SPEED;
	}

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

void LevelScreenManager::EffectSyne(PlayerBase& _player, const int _playerNum)
{
	//反映効果
	TYPE type = preTypeData_[_playerNum];
	EffectManager::EFFECT effectType = EffectManager::EFFECT::NONE;
	switch (type)
	{
	case TYPE::ATTACK:
		effectType = EffectManager::EFFECT::ATTACK_UP;
		break;

	case TYPE::DEFENSE:
		effectType = EffectManager::EFFECT::DEFENCE_UP;
		break;

	case TYPE::LIFE:
		effectType = EffectManager::EFFECT::LIFE_UP;
		break;

	case TYPE::SPEED:
		effectType = EffectManager::EFFECT::SPEED_UP;
		break;

	default:
		break;
	}

	//エフェクト設定
	EffectManager::GetInstance().SyncEffect(
		effectType,
		_player.GetPos(),
		Quaternion(),
		EFFECT_SCALE);
}

void LevelScreenManager::FaderDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
	DrawBox(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		AsoUtility::BLACK, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void LevelScreenManager::Fade()
{
	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isFader_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			// 暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);

			//状態変更
			ChangeState(STATE::SELECT);
		}
		break;
	}
}

void LevelScreenManager::SkipState(const STATE& _nextState)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_RETURN)||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1,InputManager::JOYPAD_BTN::RIGHT)||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2,InputManager::JOYPAD_BTN::RIGHT)||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD3,InputManager::JOYPAD_BTN::RIGHT)||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD4,InputManager::JOYPAD_BTN::RIGHT))
	{
		ChangeState(_nextState);
		isFader_ = true;
		select_->SetSkipState();
	}
}

void LevelScreenManager::DebagUpdate()
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_B))
	{
		AddExp(DEBAG_EXP);
	}
}

void LevelScreenManager::DebagDraw()
{
	Vector2 pos = { 0,0 };
	constexpr int INTERVAL = 16;
	DrawFormatString
	(pos.x, pos.y, AsoUtility::WHITE, "現在の経験値%2f", exp_);
	pos.y += INTERVAL;
	DrawFormatString
	(pos.x, pos.y, AsoUtility::WHITE, "現在のレベル%d", nowLevel_);
	pos.y += INTERVAL;
	DrawFormatString
	(pos.x, pos.y, AsoUtility::WHITE, "現在のステート%d", static_cast<int>(state_));
}