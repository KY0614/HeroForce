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

}

void ResourceManager::InitTitle(void)
{
	Resource res;
}

void ResourceManager::InitSelect(void)
{
	ResourcePlayer();
	ResourceStage();
}

void ResourceManager::InitGame(void)
{
	Resource res;

	//レベル通知用
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Level-Up.png");
	resourcesMap_.emplace(SRC::LEVEL_UP, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectUI.png");
	resourcesMap_.emplace(SRC::SELECT_UI, res);

	ResourcePlayer();
	ResourceEnemy();
	ResourceStage();

	//チキン
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);
}

void ResourceManager::InitResult(void)
{
	Resource res;
}

//プレイヤー
//*************************************************************************************************************
void ResourceManager::ResourcePlayer(void)
{
	Resource res;

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
}

//敵
//*************************************************************************************************************
void ResourceManager::ResourceEnemy(void)
{
	Resource res;

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

	//ゴーレム（敵）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Boss_Golem.mv1");
	resourcesMap_.emplace(SRC::ENEMY_GOLEM, res);
}

//ステージ
//*************************************************************************************************************
void ResourceManager::ResourceStage(void)
{
	Resource res;
	//スカイドーム
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);

	//ステージ[樽]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "barrel.mv1");
	resourcesMap_.emplace(SRC::STAGE_BARREL, res);

	//ステージ[ベンチ]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "bench.mv1");
	resourcesMap_.emplace(SRC::STAGE_BENCH, res);

	//ステージ[柵長い]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "fencesLong.mv1");
	resourcesMap_.emplace(SRC::STAGE_FENCES_LONG, res);

	//ステージ[柵短め]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "fencesShort.mv1");
	resourcesMap_.emplace(SRC::STAGE_FENCES_SHORT, res);

	//ステージ[地面]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "ground.mv1");
	resourcesMap_.emplace(SRC::STAGE_GROUND, res);

	//ステージ[家01]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "house01.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_01, res);

	//ステージ[家02]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "house02.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_02, res);

	//ステージ[家03]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "house03.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_03, res);

	//ステージ[家04]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "house04.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_04, res);

	//ステージ[家05]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "house05.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_05, res);

	//ステージ[物干し01]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "hunger01.mv1");
	resourcesMap_.emplace(SRC::STAGE_HUNGER_01, res);

	//ステージ[物干し02]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "hunger02.mv1");
	resourcesMap_.emplace(SRC::STAGE_HUNGER_02, res);

	//ステージ[岩01]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "rock01.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROCK_01, res);

	//ステージ[岩02]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "rock02.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROCK_02, res);

	//ステージ[岩03]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "rock03.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROCK_03, res);

	//ステージ[包袋]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "sack.mv1");
	resourcesMap_.emplace(SRC::STAGE_SACK, res);

	//ステージ[机]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "table.mv1");
	resourcesMap_.emplace(SRC::STAGE_TABLE, res);

	//ステージ[木01]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "tree01.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_01, res);

	//ステージ[木02]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "tree02.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_02, res);

	//ステージ[木03]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "tree03.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_03, res);

	//ステージ[木04]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "tree04.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_04, res);

	//ステージ[木05]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "tree05.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_05, res);

	//ステージ[荷車]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "wagon.mv1");
	resourcesMap_.emplace(SRC::STAGE_WAGON, res);

	//ステージ[井戸]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "well.mv1");
	resourcesMap_.emplace(SRC::STAGE_WELL, res);

	//ステージ[丸太]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "wood.mv1");
	resourcesMap_.emplace(SRC::STAGE_WOOD, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
	resourcesMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
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
