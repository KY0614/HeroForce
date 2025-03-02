#include "ChickenManager.h"

ChickenManager::ChickenManager(
	std::vector<VECTOR> pos,
	const Transform& stageTrans,
	const Transform& playerTrans)
	: pos_(pos), stageTrans_(stageTrans), playerTrans_(playerTrans)
{
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
	for (auto it = chickens_.begin(); it != chickens_.end(); ) {
		// ターゲット位置取得
		(*it)->SetTarget(playerTrans_.pos);

		// 更新処理
		(*it)->Update();

		//衝突判定
		CollisionStage(stageTrans_, *it);

		// チキンの削除
		if ((*it)->GetState() == ChickenBase::STATE::END) {
			it = chickens_.erase(it); // 削除し、次の要素に進む
		}
		else {
			++it; // 削除しない場合は次の要素に進む
		}
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

void ChickenManager::CollisionStage(const Transform& stageTrans, std::shared_ptr<ChickenBase> cheken)
{

	auto& col = Collision::GetInstance();
	if (col.IsHitUnitStageObject(stageTrans.modelId, cheken->GetTransform().pos, cheken->GetRadius()))
	{
		cheken->SetPrePos();
	}
}

const int ChickenManager::GetAliveNum(void) const
{
	int ret = 0;

	for (auto it = chickens_.begin(); it != chickens_.end(); ) {
		if ((*it)->IsAlive())ret++;
	}

	return ret;
}
