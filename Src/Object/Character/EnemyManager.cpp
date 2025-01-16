#include"Enemy.h"
#include"EnemySort/EneArcher.h"
#include"EnemySort/EneAxe.h"
#include"EnemySort/EneBrig.h"
#include"EnemySort/EneGolem.h"
#include"EnemySort/EneMage.h"

#include "EnemyManager.h"

void EnemyManager::Init(void)
{
	for (int i = 0; i < ONETYPE_MAX; i++)
	{
		archer_[i] = std::make_unique<EneArcher>();
		archer_[i]->Init();

		axe_[i] = std::make_unique<EneAxe>();
		axe_[i]->Init();

		brig_[i] = std::make_unique<EneBrig>();
		brig_[i]->Init();

		mage_[i] = std::make_unique<EneMage>();
		mage_[i]->Init();
	}

	golem_ = std::make_unique<EneGolem>();
	golem_->Init();
}

void EnemyManager::Update(void)
{
	for (auto& enm : activeEnms_)enm->Update();
}

void EnemyManager::Draw(void)
{
	for (auto& enm : activeEnms_)enm->Draw();
}

void EnemyManager::Release(void)
{
	for (int i = 0; i < ONETYPE_MAX; i++)
	{
		archer_[i]->Destroy();

		axe_[i]->Destroy();

		brig_[i]->Destroy();

		mage_[i]->Destroy();
	}
}

const std::vector<std::unique_ptr<Enemy>> EnemyManager::GetActiveEnemys(void)
{
	return activeEnms_;
}

void EnemyManager::SetActive(std::unique_ptr<Enemy> enm)
{
	//敵の更新等を掛けるやつをセット
	activeEnms_.push_back(enm);
}
