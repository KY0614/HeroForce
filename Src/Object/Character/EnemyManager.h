#pragma once

#include<vector>
#include<memory>

class Enemy;

class EnemyManager
{
public:
	//一種類の敵の最大数
	static constexpr int ONETYPE_MAX = 3;
	//敵の出現最大数
	static constexpr int ENEMY_MAX = 8;

	//フェーズごとの初期生成数
	
	//フェーズ1の敵の初期生成数
	static constexpr int PHASE_ONE_INIT_CREATE_ENEMY = 4;
	//フェーズ2の敵の初期生成数
	static constexpr int PHASE_TWO_INIT_CREATE_ENEMY = 6;

	//敵の出現半径
	static constexpr float GENELATE_RADIUS = 300.0f;

	//敵間の距離
	static constexpr float ENEMY_DISTANCE = 20.0f;

	//敵間の生成回数上限
	static constexpr int ENEMY_CREATE_CHALLENGE_LIMIT = 5000;

	//敵の生成間隔
	static constexpr float CREATE_INTERVAL = 5.0f;

	//敵の種類
	enum class TYPE {
		ARCHER,
		AXE,
		BRIG,
		GOLEM,
		MAGE,
		MAX,
	};

	//コンストラクタ
	EnemyManager(std::vector<VECTOR> _pos);

	//デストラクタ
	~EnemyManager() = default;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//衝突判定
	void CollisionStage(const Transform& stageTrans);

	/// <summary>
	/// 指定した敵の取得
	/// </summary>
	/// <param name="_num">欲しい敵の配列番号</param>
	/// <returns>敵のインスタンス</returns>
	Enemy* GetActiveEnemy(int _num)const { return activeEnemys_[_num]; }

	//総経験値量の取得
	const float GetAllExp(void)const { return allExp_; }

	//総経験値を削除
	void ClearAllExp(void) { allExp_ = 0.0f; }

	//敵の総数の取得
	const int GetActiveNum(void)const { return activeNum_; }

	//倒された敵の後処理 _num=倒された敵の配列番号
	void DeathEnemy(int _num);

	const int GetDunkCnt(void)const { return dunkCnt_; }

	//フェーズ変更
	void ProcessChangePhase(const int _phase);

private:

	//更新等をかける動きのある敵
	Enemy* activeEnemys_[ENEMY_MAX];

	TYPE activeEnemysType_[ENEMY_MAX];

	//アクティブな敵の総数
	int activeNum_;

	//種類別の敵の量
	int activeTypeNum_[static_cast<int>(TYPE::MAX)];

	//総経験値数
	float allExp_;
	int dunkCnt_;	//倒した敵の総数(フェーズごとにリセット)

	//生成座標
	std::vector<VECTOR> createPos_;

	//生成間隔
	float createIntCnt_;
	
	//引数の敵をアクティブ状態に
	void CreateEnemy(void);

	//敵をすべて削除する
	void DeleteAllEnemy(void);

	//ボスを作成
	void CreateBoss(void);

	//重なっていない座標を取る
	VECTOR GetNotOverlappingPos(void);

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

