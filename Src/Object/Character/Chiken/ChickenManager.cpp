#include "ChickenManager.h"

ChickenManager::ChickenManager(std::vector<VECTOR> pos)
{
	//情報の受け取り
	pos_ = pos;

	//初期化
	targetPos_ = AsoUtility::VECTOR_ZERO;
}

ChickenManager::~ChickenManager()
{
}

void ChickenManager::Init()
{
	//座標の中身をシャッフルする
	ShufflePos();

	//チキンの生成
	for (int i = 0; i < CREATE_COUNT; i++)
	{
		//インスタンス作成
		auto chicken = std::make_shared<ChickenBase>();

		//座標決め
		VECTOR pos = pos_[i];

		//生成
		chicken->Create(pos);

		//格納
		chickens_.emplace_back(std::move(chicken));
	}
}

void ChickenManager::Update()
{
	for (auto c : chickens_)
	{
		//ターゲット位置取得
		c->SetTarget(targetPos_);

		//更新処理
		c->Update();
	}
}

void ChickenManager::Draw()
{
	for (auto c : chickens_)
	{
		c->Draw();
	}
}

void ChickenManager::ShufflePos()
{
	// ランダムエンジンを生成
	std::random_device rd;
	std::mt19937 gen(rd());

	// シャッフル
	std::shuffle(pos_.begin(), pos_.end(), gen);
}

void ChickenManager::SetTargetPos(const VECTOR pos)
{
	targetPos_ = pos;
}
