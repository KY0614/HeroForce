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
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Archer1.mv1");
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

	//ステージ[草]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/Stage1/Bush_03.mv1");
	resourcesMap_.emplace(SRC::BUSH_03, res);

	//ステージ[花]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/Stage1/Flowers_02.mv1");
	resourcesMap_.emplace(SRC::FLOWERS_02, res);

	//ステージ[岩1]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/Stage1/Rock_01.mv1");
	resourcesMap_.emplace(SRC::ROCK_01, res);

	//ステージ[岩2]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/Stage1/Rock_04.mv1");
	resourcesMap_.emplace(SRC::ROCK_04, res);

	//ステージ[切り株]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/Stage1/Stump_01.mv1");
	resourcesMap_.emplace(SRC::STUMP_01, res);

	//ステージ[地形]
 	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/Stage1/Terrain.mv1");
	resourcesMap_.emplace(SRC::TERRAIN, res);

	//ステージ[木]
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Stage/Stage1/Tree_05.mv1");
	resourcesMap_.emplace(SRC::TREE_05, res);

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
