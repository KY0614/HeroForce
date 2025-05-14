#include"../../Manager/Generic/ResourceManager.h"
#include"../../Scene/GameScene.h"
#include "InformFaze.h"

InformFaze::InformFaze(void)
{
	informStrPos_ = { 0.0f,0.0f,0.0f };
	fazeCnt_ = 0;
	informCnt_ = 0;
	missionImg_[static_cast<int>(INFORM_TYPE::NOMAL)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::MISSION_NOMAL).handleId_;
	missionImg_[static_cast<int>(INFORM_TYPE::LAST)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::MISSION_LAST).handleId_;
	font_ = CreateFontToHandle(NULL, FONT_SIZE, FONT_THICK, DX_FONTTYPE_EDGE);
	missionCnt_ = 0;

	informStrPos_ = { 0.0f,STRING_SPOS_Y,0.0f };
	informImgPos_ = { Application::SCREEN_SIZE_X / 2.0f,
		MISSION_SPOS_Y,
		0.0f };
}

InformFaze::~InformFaze(void)
{
}

bool InformFaze::Update(void)
{
	//カウントが上限より大きくなったら終了
	if (informCnt_ > INFORM_FINISH)return false;

	informCnt_++;

	//目標地点ではないとき座標を移動
	if (informStrPos_.y != STRING_FPOS_Y)informStrPos_.y++;
	//通知の折り返しに行ってない場合ここで更新終了
	if (informCnt_ <= INFORM_HARF)return true;

	//折り返し後なのでミッションを表示中の更新処理
	//目標地点ではないとき座標を移動
	if (informImgPos_.y != MISSION_FPOS_Y)informImgPos_.y++;

	return true;
}

void InformFaze::Draw(void)
{
	//通知中の半分はフェーズ数表示
	if (informCnt_ <= INFORM_HARF)
	{
		//フェーズ数表示
		DrawFormatStringToHandle(informStrPos_.x, informStrPos_.y, RED_COLOR, font_, "第%dフェーズ", fazeCnt_);
		return;
	}
	
	DrawRotaGraph(informImgPos_.x, informImgPos_.y,
		0.5f,
		0.0f,
		missionImg_[missionCnt_],
		true);
}

void InformFaze::SetFazeCnt(const int _cnt)
{
	//カウントの設定及び文章の作成
	fazeCnt_ = _cnt;
	std::string fazeStr_ = "第" + std::to_string(fazeCnt_) + "フェーズ";
	int len = (int)strlen(fazeStr_.c_str());
	int width = GetDrawStringWidthToHandle(fazeStr_.c_str(), len, font_);

	//位置の設定
	informStrPos_.x= Application::SCREEN_SIZE_X / 2.0f - width / 2.0f;
	informStrPos_.y = STRING_SPOS_Y;
	informImgPos_.y = MISSION_SPOS_Y;
	//カウント初期化
	informCnt_ = 0;

	if (fazeCnt_ == GameScene::LAST_FAZE)missionCnt_ = FAZE_INIT;
	else missionCnt_ = 0;
}
