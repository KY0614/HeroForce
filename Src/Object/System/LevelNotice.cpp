#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/GameSystem/TextManager.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Common/Vector2.h"
#include "LevelNotice.h"

LevelNotice::LevelNotice() 
{
	newLevel_ = -1;
	cnt_ = -1;
	scl_ = -1.0f;
	state_ = STATE::NONE;
	alphaMes_ = -1.0f;
	alphaLevel_ = -1.0f;
	fontMes_ = -1;
	fontLevel_ = -1;
	text_ = "";

	// 状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&LevelNotice::ChangeStateNone, this));
	stateChanges_.emplace(STATE::FADE_IN, std::bind(&LevelNotice::ChangeStateFade, this));
	stateChanges_.emplace(STATE::MAINTAIN, std::bind(&LevelNotice::ChangeStateMaintain, this));
	stateChanges_.emplace(STATE::FIN, std::bind(&LevelNotice::ChangeStateFin , this));
}

void LevelNotice::Init()
{
	//読み込み
	Load();

	//初期化
	Reset();
}

void LevelNotice::Update()
{
	//更新処理
	stateUpdate_();
}

void LevelNotice::Draw()
{
	//描画設定
	Vector2 pos ={ 0,0 };
	float angle = 0.0f;
	bool trans = true;
	bool reverse = false;
	int c = TextManager::CENTER_TEXT;
	int cH = TextManager::CENTER_TEXT_H;	
	
	//エフェクト描画
	if (isEfe_) {
		DrawRotaGraph(EFFECT_POS_X,
			EFFECT_POS_Y,
			EFFECT_RATE,
			0.0f,
			imgEfe_[efeAnimNum_],
			true,
			false);
	}

	//メッセージ描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alphaMes_);
	DrawMessage();	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//レベルの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alphaLevel_);
	DrawLevel();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void LevelNotice::Release()
{
	DeleteFontToHandle(fontMes_);
	DeleteFontToHandle(fontLevel_);
}

void LevelNotice::Load()
{
	//テキスト
	auto& text_m = TextManager::GetInstance();
	text_ = text_m.TextLoad(TextManager::TEXTS::LEVEL_UP);

	//フォント
	fontMes_ = CreateFontToHandle(
		text_m.GetFontName(TextManager::FONT_TYPE::KAKUMEI).c_str(),
		FONT_TEXT_SIZE,
		FONT_THICK);
	fontLevel_ = CreateFontToHandle(
		text_m.GetFontName(TextManager::FONT_TYPE::KAKUMEI).c_str(),
		FONT_LEVEL_SIZE,
		FONT_THICK);

	//エフェクト
	imgEfe_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEVEL_SCREEN_EFE).handleIds_;
}

void LevelNotice::Reset()
{
	cnt_ = START_ALPHA_LEVEL;
	scl_ = 1.0f;	
	alphaMes_ = 0.0f;
	alphaLevel_ = 0.0f;
	ChangeState(LevelNotice::STATE::NONE);
	efeStep_ = 0;
	efeSpeed_ = EFFECT_ANIM_SPPED;
	efeAnimNum_ = 0;
	isEfe_ = false;
}

void LevelNotice::ChangeState(const STATE _state)
{
	// 状態受け取り
	state_ = _state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();
}

void LevelNotice::SetNewLevel(const int& _newLevel)
{
	newLevel_ = _newLevel;
}

void LevelNotice::ChangeStateNone()
{
	stateUpdate_ = std::bind(&LevelNotice::UpdateNone, this);
}

void LevelNotice::ChangeStateFade()
{
	stateUpdate_ = std::bind(&LevelNotice::UpdateFade, this);
}

void LevelNotice::ChangeStateMaintain()
{
	stateUpdate_ = std::bind(&LevelNotice::UpdateMaintain, this);
}

void LevelNotice::ChangeStateFin()
{
	stateUpdate_ = std::bind(&LevelNotice::UpdateFin, this);
}

void LevelNotice::UpdateNone()
{
	ChangeState(STATE::FADE_IN);
}

void LevelNotice::UpdateFade()
{
	float rate = EXPANSION_RATE;	
	float max = Fader::ALPHA_MAX;		

	//カウント更新
	cnt_ -= SceneManager::GetInstance().GetDeltaTime();

	//透過処理
	alphaMes_ += rate;
	if (cnt_ <= 0.0f) { 
		alphaLevel_ += rate; 
		isEfe_ = true;
		EffectUpdate();
	}

	//アルファ値の最大確認
	if (alphaMes_ >= max) { alphaMes_ = max; }
	if (alphaLevel_ >= max) //レベルのアルファ値が最大になった時は状態を変える
	{
		alphaLevel_ = max; 
		cnt_ = MAINTAIN_SECONDS;
		ChangeState(STATE::MAINTAIN);
	}
}

void LevelNotice::UpdateMaintain()
{
	//カウント更新
	cnt_ -= SceneManager::GetInstance().GetDeltaTime();
	
	//エフェクト更新
	EffectUpdate();

	if (cnt_ <= 0.0f)
	{
		//次の状態へ変更
		ChangeState(STATE::FIN);
	}
}

void LevelNotice::UpdateFin()
{
}

void LevelNotice::EffectUpdate()
{
	//エフェクト更新処理
	efeStep_++;

	//アニメーション番号割り当て
	const int ANIM_NUM = ResourceManager::LV_EFE_NUM_X * ResourceManager::LV_EFE_NUM_Y;
	efeAnimNum_ = efeStep_ / efeSpeed_ % ANIM_NUM;
	
	//エフェクトの非表示
	if (efeSpeed_ * ANIM_NUM < efeStep_) {
		isEfe_ = false;
	}
}

void LevelNotice::DrawMessage()
{
	int c = TextManager::CENTER_TEXT;
	int cH = TextManager::CENTER_TEXT_H;
	int length = static_cast<int>(text_.length());

	Vector2 pos{
	Application::SCREEN_SIZE_X / c - length * FONT_TEXT_SIZE / cH - MES_TEXT_POS_X,
	Application::SCREEN_SIZE_Y / c - FONT_TEXT_SIZE / c - MES_TEXT_POS_Y };
	
	DrawFormatStringToHandle(pos.x, pos.y, AsoUtility::WHITE, fontMes_, text_.c_str());
}

void LevelNotice::DrawLevel()
{
	std::string levelText = std::to_string(newLevel_);
	int c = TextManager::CENTER_TEXT;
	int cH = TextManager::CENTER_TEXT_H;
	int length = static_cast<int>(levelText.length());
	Vector2 pos{
	Application::SCREEN_SIZE_X / c - length * FONT_LEVEL_SIZE / cH,
	(Application::SCREEN_SIZE_Y - FONT_LEVEL_SIZE) / c + LEVEL_TEXT_POS_Y };
	
	DrawFormatStringToHandle(pos.x, pos.y, AsoUtility::WHITE, fontLevel_, levelText.c_str() );
}
