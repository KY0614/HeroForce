#pragma once

#include<vector>
#include<memory>

class Enemy;

class EnemyManager
{
public:
	//一種類の敵の最大数
	static constexpr int ONETYPE_MAX = 5;
	//敵の出現最大数
	static constexpr int ENEMY_MAX = 8;

	//敵の出現半径
	static constexpr float GENELATE_RADIUS = 100.0f;

	//敵間の距離
	static constexpr float ENEMY_DISTANCE = 30.0f;

	enum class TYPE {
		ARCHER,
		AXE,
		BRIG,
		GOLEM,
		MAGE,
		MAX,
	};

	//コンストラクタ
	EnemyManager(std::vector<VECTOR> _pos) : createPos_(_pos) {};

	//デストラクタ
	~EnemyManager() = default;

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

	//衝突判定
	void CollisionStage(const Transform& stageTrans);
	void DeathEnemy(int _num);
private:

	//更新等をかける動きのある敵
	Enemy* activeEnemys_[ENEMY_MAX];

	//アクティブな敵の総数
	int activeNum_;

	//生成座標
	std::vector<VECTOR> createPos_;

	
	//引数の敵をアクティブ状態に
	void CreateEnemy(void);

	/// <summary>
	/// 重なっていない座標を取る
	/// </summary>
	/// <param name="_type">敵のタイプ</param>
	/// <returns>重なっていない座標</returns>
	VECTOR GetNotOverlappingPos(TYPE _type);

	/// <summary>
	/// 円範囲のランダム一点を取る
	/// </summary>
	/// <param name="_myPos">出現予定座標</param>
	/// <param name="_r">出現範囲</param>
	/// <param name="_tPos">出現させる座標</param>
	void GetRandomPointInCircle(VECTOR _myPos, const int _r, VECTOR& _tPos);

	/// <summary>
	/// 敵達が重なっていないか
	/// </summary>
	/// <param name="_thisEnemy">比較したい敵</param>
	/// <param name="_minDist">敵の直径</param>
	/// <returns>敵が重なったか(true:重なった)</returns>
	bool IsOverlap(VECTOR& _tPos, float _minDist);
};

