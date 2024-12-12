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

	//���x���ʒm�p
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Level-Up.png");
	resourcesMap_.emplace(SRC::LEVEL_UP, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectUI.png");
	resourcesMap_.emplace(SRC::SELECT_UI, res);

	ResourcePlayer();
	ResourceEnemy();
	ResourceStage();

	//�`�L��
	res = Resource(Resource::TYPE::MODEL, "Data/Model/Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);
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
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "SkyDome.mv1");
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
