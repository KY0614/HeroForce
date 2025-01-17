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

	enum class TYPE {
		ARCHER,
		AXE,
		BRIG,
		GOLEM,
		MAGE,
		MAX,
	};

	void Init(void);
	void Update(VECTOR _target);
	void Draw(void);
	void Release(void);

	/// <summary>
	/// �w�肵���G�̎擾
	/// </summary>
	/// <param name="_num">�~�����G�̔z��ԍ�</param>
	/// <returns>�G�̃C���X�^���X</returns>
	Enemy* GetActiveEnemy(int _num){ return activeEnemys_[_num]; }

	//�G�̑����̎擾
	const int GetActiveNum(void) { return activeNum_; }

	//�|���ꂽ�G�̌㏈�� _num=�|���ꂽ�G�̔z��ԍ�
	void DethEnemy(int _num);
private:

	//�X�V���������铮���̂���G
	Enemy* activeEnemys_[ENEMY_MAX];

	//�A�N�e�B�u�ȓG�̑���
	int activeNum_;

	
	//�����̓G���A�N�e�B�u��Ԃ�
	void CreateEnemy(void);
};

