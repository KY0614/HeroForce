#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	Resource res;


	//レベル通知用
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Level-Up.png");
	resourcesMap_.emplace(SRC::LEVEL_UP, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectUI.png");
	resourcesMap_.emplace(SRC::SELECT_UI, res);

	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Carsors.png",CARSORS_NUM_X,CARSORS_NUM_Y,CARSORS_SIZE, CARSORS_SIZE);
	resourcesMap_.emplace(SRC::CARSOLS, res);

	//モデル
	// ********************************************************************
	//魔法使い（プレイヤー）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Mage.mv1");
	resourcesMap_.emplace(SRC::PLAYER_MAGE, res);

	//騎士（プレイヤー）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Knight.mv1");
	resourcesMap_.emplace(SRC::PLAYER_KNIGHT, res);

	//弓使い（プレイヤー）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Archer.mv1");
	resourcesMap_.emplace(SRC::PLAYER_ARCHER, res);

	//斧使い（プレイヤー）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Axeman.mv1");
	resourcesMap_.emplace(SRC::PLAYER_AXEMAN, res);

	//魔法使い（敵）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Mage.mv1");
	resourcesMap_.emplace(SRC::ENEMY_MAGE, res);

	//山賊（敵）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Brigant.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BRIGANT, res);

	//弓使い（敵）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Archer.mv1");
	resourcesMap_.emplace(SRC::ENEMY_ARCHER, res);

	//斧使い（敵）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Axeman.mv1");
	resourcesMap_.emplace(SRC::ENEMY_AXEMAN, res);

	//ステージ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "Stagei.mv1");
	resourcesMap_.emplace(SRC::STAGE_1, res);

	//--------------------------------------------------------------------------------------
	//スカイドーム
	res = Resource(Resource::TYPE::MODEL, "Data/Model/SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);

	//ステージ[樽]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/barrel.mv1");
	resourcesMap_.emplace(SRC::STAGE_BARREL, res);

	//ステージ[ベンチ]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/bench.mv1");
	resourcesMap_.emplace(SRC::STAGE_BENCH, res);

	//ステージ[柵長い]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/fencesLong.mv1");
	resourcesMap_.emplace(SRC::STAGE_FENCES_LONG, res);

	//ステージ[柵短め]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/fencesShort.mv1");
	resourcesMap_.emplace(SRC::STAGE_FENCES_SHORT, res);

	//ステージ[地面]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/ground.mv1");
	resourcesMap_.emplace(SRC::STAGE_GROUND, res);

	//ステージ[家01]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/house01.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_01, res);

	//ステージ[家02]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/house02.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_02, res);

	//ステージ[家03]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/house03.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_03, res);

	//ステージ[家04]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/house04.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_04, res);

	//ステージ[家05]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/house05.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_05, res);

	//ステージ[物干し01]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/hunger01.mv1");
	resourcesMap_.emplace(SRC::STAGE_HUNGER_01, res);

	//ステージ[物干し02]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/hunger02.mv1");
	resourcesMap_.emplace(SRC::STAGE_HUNGER_02, res);

	//ステージ[岩01]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/rock01.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROCK_01, res);

	//ステージ[岩02]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/rock02.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROCK_02, res);

	//ステージ[岩03]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/rock03.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROCK_03, res);

	//ステージ[包袋]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/sack.mv1");
	resourcesMap_.emplace(SRC::STAGE_SACK, res);

	//ステージ[机]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/table.mv1");
	resourcesMap_.emplace(SRC::STAGE_TABLE, res);

	//ステージ[木01]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/tree01.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_01, res);

	//ステージ[木02]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/tree02.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_02, res);

	//ステージ[木03]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/tree03.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_03, res);

	//ステージ[木04]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/tree04.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_04, res);

	//ステージ[木05]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/tree05.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_05, res);
	
	//ステージ[荷車]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/wagon.mv1");
	resourcesMap_.emplace(SRC::STAGE_WAGON, res);

	//ステージ[井戸]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/well.mv1");
	resourcesMap_.emplace(SRC::STAGE_WELL, res);

	//ステージ[丸太]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/wood.mv1");
	resourcesMap_.emplace(SRC::STAGE_WOOD, res);

	//チキン
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
