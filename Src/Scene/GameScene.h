#pragma once
#include "SceneBase.h"
#include<vector>

class Grid;
class PlayerBase;
class Enemy;
class StageBase;
class SkyDome;
class LevelBase;

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
	StageBase* stage_;

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

	LevelBase* level_;

	//当たり判定（他項目に干渉するもののみ）
	void Collision(void);
};

