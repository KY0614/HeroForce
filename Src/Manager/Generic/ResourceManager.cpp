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

	//強化選択用UI画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectUI.png");
	resourcesMap_.emplace(SRC::SELECT_UI, res);

	//カーソルまとめ画像(強化選択用)
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Carsors.png", CARSORS_NUM_X, CARSORS_NUM_Y, CARSORS_SIZE, CARSORS_SIZE);
	resourcesMap_.emplace(SRC::CARSOLS, res);

	//経験値ゲージ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "CircleGage.png");
	resourcesMap_.emplace(SRC::CIRCLE_GAGE, res);

	//経験値経験値ゲージ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "CircleExpGage.png");
	resourcesMap_.emplace(SRC::CIRCLE_EXP_GAGE, res);

	//ゲームUI用数字画像
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Numbers.png", NUMBERS_NUM_X,NUMBERS_NUM_Y, NUMBERS_SIZE, NUMBERS_SIZE);
	resourcesMap_.emplace(SRC::NUMBERS, res);

	//レベル画面用エフェクト
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "LevelUpEffect.png", LV_EFE_NUM_X, LV_EFE_NUM_Y, LV_EFE_SIZE, LV_EFE_SIZE);
	resourcesMap_.emplace(SRC::LEVEL_SCREEN_EFE, res);

	//強化系エフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "LifeUp.efkefc");
	resourcesMap_.emplace(SRC::LIFE_UP_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "AttackUp.efkefc");
	resourcesMap_.emplace(SRC::ATTACK_UP_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "DefenceUp.efkefc");
	resourcesMap_.emplace(SRC::DEFENCE_UP_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "SpeedUp.efkefc");
	resourcesMap_.emplace(SRC::SPEED_UP_EFE, res);

	//強化UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "AttackUp.png");
	resourcesMap_.emplace(SRC::ATTACK_UP_UI, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "DefenseUp.png");
	resourcesMap_.emplace(SRC::DEFENCE_UP_UI, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SpeedUp.png");
	resourcesMap_.emplace(SRC::SPEED_UP_UI, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "LifeUp.png");
	resourcesMap_.emplace(SRC::LIFE_UP_UI, res);

	//HP関係UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpGage.png");
	resourcesMap_.emplace(SRC::HP_GAGE, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpBarPlayer.png");
	resourcesMap_.emplace(SRC::HP_PLAYER, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpBarEnemy.jpg");
	resourcesMap_.emplace(SRC::HP_ENEMY, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpBarCpu.jpg");
	resourcesMap_.emplace(SRC::HP_CPU, res);


	ResourcePlayer();
	ResourceEnemy();
	ResourceStage();

	//チキン
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);

	//ヘルプ画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Help!.png");
	resourcesMap_.emplace(SRC::HELP, res);
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

	//ステージ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "Stage.mv1");
	resourcesMap_.emplace(SRC::STAGE_01, res);

	//装飾
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "Deco.mv1");
	resourcesMap_.emplace(SRC::DECO_01, res);

	//スカイドーム
	res = Resource(Resource::TYPE::MODEL, Application::PATH_SKYDOME + "SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);
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
