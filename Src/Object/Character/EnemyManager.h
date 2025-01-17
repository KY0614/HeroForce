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
	/// 指定した敵の取得
	/// </summary>
	/// <param name="_num">欲しい敵の配列番号</param>
	/// <returns>敵のインスタンス</returns>
	Enemy* GetActiveEnemy(int _num){ return activeEnemys_[_num]; }

	//敵の総数の取得
	const int GetActiveNum(void) { return activeNum_; }

	//倒された敵の後処理 _num=倒された敵の配列番号
	void DethEnemy(int _num);
private:

	//更新等をかける動きのある敵
	Enemy* activeEnemys_[ENEMY_MAX];

	//アクティブな敵の総数
	int activeNum_;

	
	//引数の敵をアクティブ状態に
	void CreateEnemy(void);
};

