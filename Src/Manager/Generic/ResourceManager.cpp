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

	//スカイドーム
	res = Resource(Resource::TYPE::MODEL, Application::PATH_SKYDOME + "SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);

	//タイトルロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleLogo.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO , res);

	//キー指示メッセージ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "PleaseKey.png");
	resourcesMap_.emplace(SRC::PLEASE_KEY, res);
}

void ResourceManager::InitSelect(void)
{
	Resource res;

	//人数選択画像
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "PlayerNumber.png", 4, 1, 300, 300);
	resourcesMap_.emplace(SRC::PLAYER_NUM, res);

	//右矢印
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Right_point.png", 1, 1, 52, 52);
	resourcesMap_.emplace(SRC::RIGHT_POINT, res);

	//左矢印
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Left_point.png", 1, 1, 52, 52);
	resourcesMap_.emplace(SRC::LEFT_POINT, res);

	//準備
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "test.png", 1, 1, 500, 200);
	resourcesMap_.emplace(SRC::READY, res);

	//役職
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "RoleNum.png", 1, 1, 300, 300);
	resourcesMap_.emplace(SRC::ROLE, res);

	//デバイス
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Device.png", 1, 1, 300, 300);
	resourcesMap_.emplace(SRC::DEVICE, res);

	//キャラクターパラメータリスト
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "CharaParams.png", CHARA_PARAM_NUM_X, CHARA_PARAM_NUM_Y, CHARA_PARAM_SIZE_X, CHARA_PARAM_SIZE_Y);
	resourcesMap_.emplace(SRC::CHARA_PARAMS, res);

	//チキン
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);

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

	//レベルアップ背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "LevelUpBack.png");
	resourcesMap_.emplace(SRC::LEVEL_UP_BACK, res);

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

	//時間
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "time.png");
	resourcesMap_.emplace(SRC::TIME, res);


	ResourcePlayer();
	ResourceEnemy();
	ResourceStage();

	//チキン
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);

	//ヘルプ画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Help!.png");
	resourcesMap_.emplace(SRC::HELP, res);

	//ランクリスト
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Ranks.png", RANKS_NUM_X, RANKS_NUM_Y, RANK_SIZE, RANK_SIZE);
	resourcesMap_.emplace(SRC::RANKS, res);

	//リザルト背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "FazeRezaltBack.jpg");
	resourcesMap_.emplace(SRC::REZALT_BACK, res);
}

void ResourceManager::InitResult(void)
{
	Resource res;
	//ランクリスト
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Ranks.png", RANKS_NUM_X, RANKS_NUM_Y, RANK_SIZE, RANK_SIZE);
	resourcesMap_.emplace(SRC::RANKS, res);

	//リザルト背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "FazeRezaltBack.jpg");
	resourcesMap_.emplace(SRC::REZALT_BACK, res);
}

void ResourceManager::InitGameOver(void)
{
	Resource res;
}

void ResourceManager::InitGameClear(void)
{
	Resource res;

	//cong
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Congratulations.png");
	resourcesMap_.emplace(SRC::CONGRATULATIONS, res);

	//花火
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Fireworks.efkefc");
	resourcesMap_.emplace(SRC::FIREWORK, res);

	ResourcePlayer();
	ResourceEnemy();
	ResourceStage();

	//チキン
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "To_Title.png");
	resourcesMap_.emplace(SRC::TO_TITLE, res);
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
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "Stage1.mv1");
	resourcesMap_.emplace(SRC::STAGE_01, res);

	//装飾
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "Deco1.mv1");
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
