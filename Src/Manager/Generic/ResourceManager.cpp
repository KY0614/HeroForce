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

	//�����I��pUI�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectUI.png");
	resourcesMap_.emplace(SRC::SELECT_UI, res);

	//�J�[�\���܂Ƃ߉摜(�����I��p)
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Carsors.png", CARSORS_NUM_X, CARSORS_NUM_Y, CARSORS_SIZE, CARSORS_SIZE);
	resourcesMap_.emplace(SRC::CARSOLS, res);

	//�o���l�Q�[�W
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "CircleGage.png");
	resourcesMap_.emplace(SRC::CIRCLE_GAGE, res);

	//�o���l�o���l�Q�[�W
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "CircleExpGage.png");
	resourcesMap_.emplace(SRC::CIRCLE_EXP_GAGE, res);

	//�Q�[��UI�p�����摜
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Numbers.png", NUMBERS_NUM_X,NUMBERS_NUM_Y, NUMBERS_SIZE, NUMBERS_SIZE);
	resourcesMap_.emplace(SRC::NUMBERS, res);

	//���x����ʗp�G�t�F�N�g
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "LevelUpEffect.png", LV_EFE_NUM_X, LV_EFE_NUM_Y, LV_EFE_SIZE, LV_EFE_SIZE);
	resourcesMap_.emplace(SRC::LEVEL_SCREEN_EFE, res);

	//�����n�G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "LifeUp.efkefc");
	resourcesMap_.emplace(SRC::LIFE_UP_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "AttackUp.efkefc");
	resourcesMap_.emplace(SRC::ATTACK_UP_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "DefenceUp.efkefc");
	resourcesMap_.emplace(SRC::DEFENCE_UP_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "SpeedUp.efkefc");
	resourcesMap_.emplace(SRC::SPEED_UP_EFE, res);

	//����UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "AttackUp.png");
	resourcesMap_.emplace(SRC::ATTACK_UP_UI, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "DefenseUp.png");
	resourcesMap_.emplace(SRC::DEFENCE_UP_UI, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SpeedUp.png");
	resourcesMap_.emplace(SRC::SPEED_UP_UI, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "LifeUp.png");
	resourcesMap_.emplace(SRC::LIFE_UP_UI, res);

	//HP�֌WUI
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

	//�`�L��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);

	//�w���v�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Help!.png");
	resourcesMap_.emplace(SRC::HELP, res);
}

void ResourceManager::InitResult(void)
{
	Resource res;
}

//�v���C���[
//*************************************************************************************************************
void ResourceManager::ResourcePlayer(void)
{
	Resource res;

	

	//���f��
// ********************************************************************
//���@�g���i�v���C���[�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Mage.mv1");
	resourcesMap_.emplace(SRC::PLAYER_MAGE, res);

	//�R�m�i�v���C���[�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Knight.mv1");
	resourcesMap_.emplace(SRC::PLAYER_KNIGHT, res);

	//�|�g���i�v���C���[�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Archer.mv1");
	resourcesMap_.emplace(SRC::PLAYER_ARCHER, res);

	//���g���i�v���C���[�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Axeman.mv1");
	resourcesMap_.emplace(SRC::PLAYER_AXEMAN, res);
}

//�G
//*************************************************************************************************************
void ResourceManager::ResourceEnemy(void)
{
	Resource res;

	//���@�g���i�G�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Mage.mv1");
	resourcesMap_.emplace(SRC::ENEMY_MAGE, res);

	//�R���i�G�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Brigant.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BRIGANT, res);

	//�|�g���i�G�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Archer.mv1");
	resourcesMap_.emplace(SRC::ENEMY_ARCHER, res);

	//���g���i�G�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Axeman.mv1");
	resourcesMap_.emplace(SRC::ENEMY_AXEMAN, res);

	//�S�[�����i�G�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Boss_Golem.mv1");
	resourcesMap_.emplace(SRC::ENEMY_GOLEM, res);
}

//�X�e�[�W
//*************************************************************************************************************
void ResourceManager::ResourceStage(void)
{
	Resource res;
	//�X�J�C�h�[��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_SKYDOME + "SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);

	//�X�e�[�W[�M]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "barrel.mv1");
	resourcesMap_.emplace(SRC::STAGE_BARREL, res);

	//�X�e�[�W[�x���`]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "bench.mv1");
	resourcesMap_.emplace(SRC::STAGE_BENCH, res);

	//�X�e�[�W[�򒷂�]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "fencesLong.mv1");
	resourcesMap_.emplace(SRC::STAGE_FENCES_LONG, res);

	//�X�e�[�W[��Z��]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "fencesShort.mv1");
	resourcesMap_.emplace(SRC::STAGE_FENCES_SHORT, res);

	//�X�e�[�W[�n��]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "ground.mv1");
	resourcesMap_.emplace(SRC::STAGE_GROUND, res);

	//�X�e�[�W[��01]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "house01.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_01, res);

	//�X�e�[�W[��02]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "house02.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_02, res);

	//�X�e�[�W[��03]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "house03.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_03, res);

	//�X�e�[�W[��04]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "house04.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_04, res);

	//�X�e�[�W[��05]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "house05.mv1");
	resourcesMap_.emplace(SRC::STAGE_HOUSE_05, res);

	//�X�e�[�W[������01]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "hunger01.mv1");
	resourcesMap_.emplace(SRC::STAGE_HUNGER_01, res);

	//�X�e�[�W[������02]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "hunger02.mv1");
	resourcesMap_.emplace(SRC::STAGE_HUNGER_02, res);

	//�X�e�[�W[��01]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "rock01.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROCK_01, res);

	//�X�e�[�W[��02]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "rock02.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROCK_02, res);

	//�X�e�[�W[��03]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "rock03.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROCK_03, res);

	//�X�e�[�W[���]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "sack.mv1");
	resourcesMap_.emplace(SRC::STAGE_SACK, res);

	//�X�e�[�W[��]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "table.mv1");
	resourcesMap_.emplace(SRC::STAGE_TABLE, res);

	//�X�e�[�W[��01]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "tree01.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_01, res);

	//�X�e�[�W[��02]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "tree02.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_02, res);

	//�X�e�[�W[��03]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "tree03.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_03, res);

	//�X�e�[�W[��04]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "tree04.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_04, res);

	//�X�e�[�W[��05]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "tree05.mv1");
	resourcesMap_.emplace(SRC::STAGE_TREE_05, res);

	//�X�e�[�W[�׎�]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "wagon.mv1");
	resourcesMap_.emplace(SRC::STAGE_WAGON, res);

	//�X�e�[�W[���]
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "well.mv1");
	resourcesMap_.emplace(SRC::STAGE_WELL, res);

	//�X�e�[�W[�ۑ�]
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
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}