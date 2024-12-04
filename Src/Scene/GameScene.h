#pragma once
#include "SceneBase.h"
#include<vector>

class Grid;
class PlayerBase;
class Enemy;
class StageManager;
class SkyDome;
class LevelScreenManager;
class UnitPositionLoad;
class ChickenManager;

class GameScene : public SceneBase
{
public:
	//デバッグ用
	#define _DEBUG_COL	//テスト用の敵とプレイヤーを一体ずつ生成（この行をコメントアウトすると消える）

	//定数
	static constexpr int PLAYER_NUM = 4;

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//ステージ
	StageManager* stage_;

	//スカイドーム
	SkyDome* sky_;

	// グリッド線
	Grid* grid_;
	//プレイヤー
	PlayerBase* players_[PLAYER_NUM];
	PlayerBase* playerTest_;
	//敵
	std::vector<Enemy*> enemys_;
	Enemy* enemyTest_;

	//チキン
	ChickenManager* chicken_;

	LevelScreenManager* level_;

	UnitPositionLoad* unitLoad_;

	//当たり判定（他項目に干渉するもののみ）
	void Collision(void);

	//強化要素反映
	void LevelUpReflection();
};

