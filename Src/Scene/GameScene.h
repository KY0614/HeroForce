#pragma once
#include "SceneBase.h"
#include"../Common/Fader.h"
#include<vector>
#include<memory>

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
	//#define _DEBUG_COL	//テスト用の敵とプレイヤーを一体ずつ生成（この行をコメントアウトすると消える）

	//定数
	//プレイヤーの数
	static constexpr int PLAYER_NUM = 4;

	static constexpr int PHASE_TIME = 180;


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
	std::unique_ptr<StageManager> stage_;

	//スカイドーム
	std::unique_ptr<SkyDome> sky_;

	//フェーダー
	std::unique_ptr<Fader>fader_;

	//プレイヤー
	std::unique_ptr<PlayerBase>players_[PLAYER_NUM];
	PlayerBase* playerTest_;
	//敵
	std::vector<std::unique_ptr<Enemy>> enemys_;
	Enemy* enemyTest_;

	//チキン
	std::unique_ptr<ChickenManager> chicken_;

	std::unique_ptr<LevelScreenManager> level_;

	std::unique_ptr<UnitPositionLoad> unitLoad_;

	//ゲームシーンのフェーズ遷移中判定
	bool isPhaseChanging_;
	int phaseCnt_;

	//当たり判定（他項目に干渉するもののみ）
	void Collision(void);
	void CollisionEnemy(void);	//敵関連の当たり判定
	void CollisionPlayer(void);	//プレイヤー関連の当たり判定
	void CollisionPlayerCPU(PlayerBase& _player,const VECTOR& _pPos);	//プレイヤー(CPU)関連の当たり判定

	//フェード
	void Fade(void);

	//フェーズ遷移
	void ChangePhase(void);
	//フェーズ更新
	void UpdatePhase(void);
	//フェーズ描画
	void DrawPhase(void);

	//強化要素反映
	void LevelUpReflection();
};

