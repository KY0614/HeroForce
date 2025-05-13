#pragma once

#include<vector>
#include<memory>

class Enemy;

class EnemyManager
{
public:

	//****************************************************************
	//定数
	//****************************************************************
	
	//敵の数
	static constexpr int ONETYPE_MAX = 3;	//一種類の敵の最大数
	static constexpr int ENEMY_MAX = 8;		//敵の出現最大数

	//フェーズ
	static constexpr int PHASE_FIRST = 1;					//最初のフェーズ
	static constexpr int PHASE_ONE_INIT_CREATE_ENEMY = 4;	//フェーズ1の敵の初期生成数
	static constexpr int PHASE_TWO_INIT_CREATE_ENEMY = 6;	//フェーズ2の敵の初期生成数

	//敵の出現関係
	static constexpr float GENELATE_RADIUS = 300.0f;			//敵の出現半径
	static constexpr float ENEMY_DISTANCE = 20.0f;				//敵間の距離
	static constexpr float CREATE_INTERVAL = 5.0f;				//敵の生成間隔

	//ループの制限
	static constexpr int RETRY_LIMIT = 200;	//ループ回数上限

	//****************************************************************
	//列挙型
	//****************************************************************

	//敵の種類
	enum class TYPE {
		ARCHER,
		AXE,
		BRIG,
		GOLEM,
		MAGE,
		MAX,
	};

	//****************************************************************
	//メンバ関数
	//****************************************************************

	//コンストラクタ
	EnemyManager(std::vector<VECTOR> _pos);

	//デストラクタ
	~EnemyManager() = default;

	//初期化
	void Init(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);
	//解放
	void Release(void);

	//衝突判定
	void CollisionStage(const Transform& stageTrans);

	/// <summary>
	/// 指定した敵の取得
	/// </summary>
	/// <param name="_num">欲しい敵の配列番号</param>
	/// <returns>敵のインスタンス</returns>
	Enemy& GetActiveEnemy(int _num)const { return *activeEnemys_[_num]; }

	//総経験値量の取得
	const float GetAllExp(void)const { return allExp_; }

	//総経験値を削除
	void ClearAllExp(void) { allExp_ = 0.0f; }

	//敵の総数の取得
	const int GetActiveNum(void)const { return activeNum_; }

	//倒された敵の後処理 _num=倒された敵の配列番号
	void DeathEnemy(int _num);

	//敵の倒された数を返す
	const int GetDunkCnt(void)const { return dunkCnt_; }

	//フェーズ変更
	void ProcessChangePhase(const int _phase);

private:
	//****************************************************************
	//メンバ変数
	//****************************************************************
	
	std::unique_ptr<Enemy> activeEnemys_[ENEMY_MAX];	//生きている敵
	TYPE activeEnemysType_[ENEMY_MAX];					//生きている敵の種類

	int activeNum_;										//アクティブな敵の総数
	int activeTypeNum_[static_cast<int>(TYPE::MAX)];	//種類別の敵の量

	float allExp_;	//総経験値数
	int dunkCnt_;	//倒した敵の総数(フェーズごとにリセット)

	std::vector<VECTOR> createPos_;	//生成座標
	float createIntCnt_;			//生成間隔
	
	//****************************************************************
	//メンバ関数
	//****************************************************************

	//引数の敵をアクティブ状態に
	void CreateEnemy(void);

	//敵をすべて削除する
	void DeleteAllEnemy(void);

	//ボスを作成
	void CreateBoss(void);
};

