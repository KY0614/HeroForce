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
	//���ނ̓G�̍ő吔
	static constexpr int ONETYPE_MAX = 5;
	//�G�̏o���ő吔
	static constexpr int ENEMY_MAX = 8;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	const std::vector<std::unique_ptr<Enemy>> GetActiveEnemys(void);
private:

	//�X�V���������铮���̂���G
	std::vector<std::unique_ptr<Enemy>>activeEnms_;

	//�e��G����
	std::unique_ptr<EneArcher>archer_[ONETYPE_MAX];
	std::unique_ptr<EneAxe>axe_[ONETYPE_MAX];
	std::unique_ptr<EneBrig>brig_[ONETYPE_MAX];
	std::unique_ptr<EneMage>mage_[ONETYPE_MAX];
	std::unique_ptr<EneGolem>golem_;

	//�����̓G���A�N�e�B�u��Ԃ�
	void SetActive(std::unique_ptr<Enemy> enm);
};

