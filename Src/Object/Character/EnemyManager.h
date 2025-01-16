#pragma once

#include<vector>
#include<memory>

class Enemy;
class EneArcher;
class EneAxe;
class EneBrig;
class EneGolem;
class EneMage;

class EnemyManager
{
public:
	//一種類の敵の最大数
	static constexpr int ONETYPE_MAX = 5;
	//敵の出現最大数
	static constexpr int ENEMY_MAX = 8;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	const std::vector<std::unique_ptr<Enemy>> GetActiveEnemys(void);
private:

	//更新等をかける動きのある敵
	std::vector<std::unique_ptr<Enemy>>activeEnms_;

	//各種敵たち
	std::unique_ptr<EneArcher>archer_[ONETYPE_MAX];
	std::unique_ptr<EneAxe>axe_[ONETYPE_MAX];
	std::unique_ptr<EneBrig>brig_[ONETYPE_MAX];
	std::unique_ptr<EneMage>mage_[ONETYPE_MAX];
	std::unique_ptr<EneGolem>golem_;

	//引数の敵をアクティブ状態に
	void SetActive(std::unique_ptr<Enemy> enm);
};

