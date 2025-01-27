#include <locale>
#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/GameSystem/TextManager.h"
#include "../../Manager/GameSystem/DataBank.h"
#include "LevelupSelect.h"
#include "Carsor.h"

LevelupSelect::LevelupSelect()
{
	fontMes_ = -1;
	fontExp_ = -1;
	plNum_ = -1;
	img_ = -1;
	imgBack_ = -1;
	element_ = -1;
	state_ = STATE::NONE;

	//カーソル画像初期化
	int value = -1;
	imgCarsors_ = &value;

	//画像
	for (int i = 0; i < LevelScreenManager::TYPE_MAX; i++) {
		imgSelects_[i] = -1;
	}

	// 状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&LevelupSelect::ChangeStateNone, this));
	stateChanges_.emplace(STATE::EXPANSION, std::bind(&LevelupSelect::ChangeStateExpansion, this));
	stateChanges_.emplace(STATE::SELECT, std::bind(&LevelupSelect::ChangeStateSelect, this));
	stateChanges_.emplace(STATE::FIN, std::bind(&LevelupSelect::ChangeStateFin, this));
}

LevelupSelect::~LevelupSelect()
{
}

void LevelupSelect::Init()
{
	//仮プレイヤー人数
	plNum_ = DataBank::GetInstance().Output(DataBank::INFO::USER_NUM);

	//読み込み
	Load();
	
	//要素数の決定
	element_ = SELECT_ELEMENT;
	ele_.resize(element_);

	//カーソルの初期化
	carsors_.resize(plNum_);
	for (int i = 0; i < carsors_.size(); i++)
	{
		carsors_[i] = std::make_unique<Carsor>();
		carsors_[i]->Init(i, imgCarsors_[i]);
		carsors_[i]->SetContllorKey(
			KEY_INPUT_RIGHT,
			KEY_INPUT_LEFT,
			KEY_INPUT_UP,
			KEY_INPUT_DOWN,
			KEY_INPUT_RETURN);
	}

	//変数の設定
	Reset();

	//要素の位置を設定
	SetFirstPos();

	//フォント生成
	CreateFonts();
}

void LevelupSelect::Update()
{
	stateUpdate_();
}

void LevelupSelect::Draw()
{
	//テキスト用変数
	int color = 0;
	int length = 0;
	Vector2 pos = { 0,0 };

	//背景
	DrawExtendGraph(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		imgBack_,
		false);

	for (auto ele : ele_)
	{
		int type = static_cast<int>(ele.type_);

		//画像描画
		DrawRotaGraph(
			ele.pos_.x, ele.pos_.y,
			ele.scl_,
			0.0f,
			imgSelects_[type],
			true,
			false);

		if (state_ == STATE::SELECT)
		{
			if (ele.isText_) {

				//テキスト描画
				int pow = 30;
				color = 0xffffff;
				length = expTexts_[type].length();
				pos = ele.pos_;
				pos.x -= length * FONT_EXP_SIZE / 4;
				pos.y += 150;

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
				DrawBox(
					pos.x, pos.y,
					pos.x + length * FONT_EXP_SIZE / 2.3,
					pos.y + FONT_EXP_SIZE,
					0x000000, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

				DrawFormatStringToHandle(
					pos.x, pos.y,
					color,
					fontExp_,
					expTexts_[type].c_str(),
					pow);
			}
		}
	}

	//テキスト用変数
	color = 0xffffff;
	length = mesText_.length() / 2;
	pos = { MES_TEXT_POS_X,MES_TEXT_POS_Y };
	pos.x -= length * FONT_MES_SIZE / 2;

	//メッセージを描画
	DrawFormatStringToHandle(
		pos.x, pos.y,
		color,
		fontMes_,
		mesText_.c_str());

	if (state_ == STATE::SELECT)
	{
		for (int i = 0; i < carsors_.size(); i++)
		{
			carsors_[i]->Draw();
		}
	}
}

void LevelupSelect::Release()
{
	DeleteFontToHandle(fontExp_);
	DeleteFontToHandle(fontMes_);
}

void LevelupSelect::Reset()
{
	//初期状態の設定
	ChangeState(LevelupSelect::STATE::NONE);

	//強化要素の種類
	ele_[static_cast<int>(LevelScreenManager::TYPE::ATTACK)].type_ = LevelScreenManager::TYPE::ATTACK;
	ele_[static_cast<int>(LevelScreenManager::TYPE::DEFENSE)].type_ = LevelScreenManager::TYPE::DEFENSE;
	ele_[static_cast<int>(LevelScreenManager::TYPE::LIFE)].type_ = LevelScreenManager::TYPE::LIFE;
	ele_[static_cast<int>(LevelScreenManager::TYPE::SPEED)].type_ = LevelScreenManager::TYPE::SPEED;

	//強化要素関係の初期化
	for (int i = 0; i < ele_.size(); i++)
	{
		//初期拡大値
		ele_[i].scl_ = 0.00f;

		//強化要素の種類
		//ele_[i].type_ = GetRandType();

		//サイズ
		ele_[i].size_ = { static_cast<int>(ELE_IMG_SIZE_X * SCALE_MAX),
			static_cast<int>(ELE_IMG_SIZE_Y * SCALE_MAX) };

		//テキストを非表示にしておく
		ele_[i].isText_ = false;
	}

	//カーソル初期化
	for (int i = 0; i < carsors_.size(); i++)
	{
		carsors_[i]->Reset();
	}

	//各プレイヤーごとの強化要素初期化
	selectTypes_.resize(plNum_);
	for (int i = 0; i < plNum_; i++) {
		selectTypes_[i] = LevelScreenManager::TYPE::MAX;
	}
}

void LevelupSelect::Load()
{
	//画像
	auto& res = ResourceManager::GetInstance();
	img_ = res.Load(ResourceManager::SRC::SELECT_UI).handleId_;
	imgSelects_[static_cast<int>(LevelScreenManager::TYPE::ATTACK)] = res.Load(ResourceManager::SRC::ATTACK_UP_UI).handleId_;
	imgSelects_[static_cast<int>(LevelScreenManager::TYPE::DEFENSE)] = res.Load(ResourceManager::SRC::DEFENCE_UP_UI).handleId_;
	imgSelects_[static_cast<int>(LevelScreenManager::TYPE::LIFE)] = res.Load(ResourceManager::SRC::LIFE_UP_UI).handleId_;
	imgSelects_[static_cast<int>(LevelScreenManager::TYPE::SPEED)] = res.Load(ResourceManager::SRC::SPEED_UP_UI).handleId_;
	imgCarsors_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CARSOLS).handleIds_;
	imgBack_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEVEL_UP_BACK).handleId_;
	
	//テキスト関係
	auto& text_m = TextManager::GetInstance();
	mesText_ = text_m.TextLoad(TextManager::TEXTS::LV_UP_SELECT_MES);
	expTexts_[static_cast<int>(LevelScreenManager::TYPE::ATTACK)] = text_m.TextLoad(TextManager::TEXTS::LV_ATK_MES);
	expTexts_[static_cast<int>(LevelScreenManager::TYPE::DEFENSE)] = text_m.TextLoad(TextManager::TEXTS::LV_DEF_MES);
	expTexts_[static_cast<int>(LevelScreenManager::TYPE::SPEED)] = text_m.TextLoad(TextManager::TEXTS::LV_SPEED_MES);
	expTexts_[static_cast<int>(LevelScreenManager::TYPE::LIFE)] = text_m.TextLoad(TextManager::TEXTS::LV_LIFE_MES);
}

void LevelupSelect::SetFirstPos()
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
		ele_[cnt].pos_ = { ELEMENT_POS_LU_X,ELEMENT_POS_LU_Y };
		cnt++;
		ele_[cnt].pos_ = { ELEMENT_POS_RU_X,ELEMENT_POS_RU_Y };
		cnt++;
		ele_[cnt].pos_ = { ELEMENT_POS_LD_X,ELEMENT_POS_LD_Y };
		cnt++;
		ele_[cnt].pos_ = { ELEMENT_POS_RD_X,ELEMENT_POS_RD_Y };
		break;
	default:
		return;
		break;
	}
}

void LevelupSelect::CreateFonts()
{
	auto& text_m = TextManager::GetInstance();
	
	fontMes_ = CreateFontToHandle(
		text_m.GetFontName(TextManager::FONT_TYPE::LOGO).c_str(),
		FONT_MES_SIZE,
		FONT_MES_THICK);

	//説明用テキストフォント
	fontExp_ =CreateFontToHandle(
		text_m.GetFontName(TextManager::FONT_TYPE::LOGO).c_str(),
		FONT_EXP_SIZE,
		FONT_EXP_THICK);
}

inline LevelScreenManager::TYPE LevelupSelect::GetRandType()
{
	int rand = GetRand(LevelScreenManager::TYPE_MAX - 1);
	LevelScreenManager::TYPE type = static_cast<LevelScreenManager::TYPE>(rand);
	return type;
}

void LevelupSelect::ChangeState(const STATE state)
{
	//状態受け取り
	state_ = state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();
}

void LevelupSelect::ChangeStateNone()
{
	stateUpdate_ = std::bind(&LevelupSelect::UpdateNone, this);
}

void LevelupSelect::ChangeStateExpansion()
{
	stateUpdate_ = std::bind(&LevelupSelect::UpdateExpansion, this);
}

void LevelupSelect::ChangeStateSelect()
{
	stateUpdate_ = std::bind(&LevelupSelect::UpdateSelect, this);
}

void LevelupSelect::ChangeStateFin()
{
	stateUpdate_ = std::bind(&LevelupSelect::UpdateFin, this);
}

void LevelupSelect::UpdateNone()
{
	//拡大処理に移る
	ChangeState(STATE::EXPANSION);
}

void LevelupSelect::UpdateExpansion()
{
	float scl = SCALE_RATE;
	float max = SCALE_MAX;

	for (int i = 0; i < ele_.size(); i++)
	{
		//画像の拡大
		ele_[i].scl_ += scl;

		//最大値に達したら
		if (ele_[i].scl_ >= max)
		{
			ele_[i].scl_ = max;

			//状態変更
			ChangeState(STATE::SELECT);
		}
	}
}

void LevelupSelect::UpdateSelect()
{
	//状態変更
	bool isChange = true;

	//テキストを全て非表示にしておく
	for (int j = 0; j < ele_.size(); j++) {
		ele_[j].isText_ = false;
	}

	for (int i = 0; i < carsors_.size(); i++)
	{
		//カーソルの更新処理
		carsors_[i]->Update();

		for (int j = 0; j < ele_.size(); j++)
		{
			//プレイヤーの強化要素が決まっている場合
			if (selectTypes_[i] != LevelScreenManager::TYPE::MAX)
			{
				//このプレイヤーの処理を終了する
				continue;
			}

			//衝突判定(強化項目とカーソルが衝突してる場合)
			if (IsCollisionBoxCircle(
				ele_[j].pos_,
				ele_[j].size_,
				carsors_[i]->GetPos(),
				Carsor::RADIUS))
			{
				//強化要素の説明を表示
				ele_[j].isText_ = true;

				//カーソルが決定の場合
				if (carsors_[i]->IsDecide()) 
				{
					//選択している要素を格納
					selectTypes_[i] = ele_[j].type_;
				}
				else
				{
					//状態変更を行わない
					isChange = false;
				}
			}
			//当たってない場合
			else
			{
				//状態変更を行わない
				isChange = false;
			}
		}
		//決定済みかどうか
		if (selectTypes_[i] == LevelScreenManager::TYPE::MAX)
		{
			//カーソルを未決定に戻す
			carsors_[i]->SetDecide(false);
		}
	}
	//状態遷移
	if (isChange)
	{
		ChangeState(STATE::FIN);
	}
}

void LevelupSelect::UpdateFin()
{
}

void LevelupSelect::DebagDraw()
{
	for (auto ele : ele_)
	{
		Vector2 LUpos{ ele.pos_.x - ele.size_.x/2,
			ele.pos_.y - ele.size_.y/2 };
		Vector2 RDpos{ ele.pos_.x + ele.size_.x/2,
			ele.pos_.y + ele.size_.y/2 };

		DrawBox(LUpos.x ,LUpos.y, RDpos.x,RDpos.y,0xff00ff,true);
	}
}

bool LevelupSelect::IsCollisionBoxCircle(Vector2 pos1, Vector2 size1, Vector2 pos2, float radius2)
{
	Vector2 righttop = pos1;
	righttop.x += size1.x / 2;
	righttop.y -= size1.y / 2;
	righttop = V2Sub(pos2, righttop);
	int rightTop = std::hypot(righttop.x, righttop.y);

	Vector2 rightdown = pos1;
	rightdown.x += size1.x / 2;
	rightdown.y += size1.y / 2;
	rightdown = V2Sub(pos2, rightdown);
	int rightDown = std::hypot(rightdown.x, rightdown.y);

	Vector2 lefttop = pos1;
	lefttop.x -= size1.x / 2;
	lefttop.y -= size1.y / 2;
	lefttop = V2Sub(pos2, lefttop);
	int leftTop = std::hypot(lefttop.x, lefttop.y);

	Vector2 leftdown = pos1;
	leftdown.x -= size1.x / 2;
	leftdown.y += size1.y / 2;
	leftdown = V2Sub(pos2, leftdown);
	int leftDown = std::hypot(leftdown.x, leftdown.y);

	//-----------------------------------------------------------

	auto diffX = (pos1.x - pos2.x);/////終点から始点を引く///////
	auto diffY = (pos1.y - pos2.y);/////終点から始点を引く///////

	if (rightTop <= radius2 ||
		rightDown <= radius2 ||
		leftTop <= radius2 ||
		leftDown <= radius2 ||
		(fabsf(diffX) < radius2 && fabsf(diffY) < radius2 + size1.y / 2) ||
		(fabsf(diffX) < radius2 + size1.x / 2 && fabsf(diffY) < size1.y / 2))
	{
		//衝突している
		return true;
	}
	
	return false;
}

Vector2 LevelupSelect::V2Sub(Vector2 pos1, Vector2 pos2)
{
	Vector2 pos;
	pos.x = pos1.x - pos2.x;
	pos.y = pos1.y - pos2.y;
	return pos;
}