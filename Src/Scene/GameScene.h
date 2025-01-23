#pragma once
#include "SceneBase.h"
#include"../Common/Fader.h"
#include<vector>
#include<memory>

class Grid;
class PlayerManager;
class PlayerBase;
class Enemy;
class EnemyManager;
class StageManager;
class SkyDome;
class LevelScreenManager;
class UnitPositionLoad;
class ChickenManager;
class FazeResult;

class GameScene : public SceneBase
{
public:
	//デバッグ用
	//#define _DEBUG_COL	//テスト用の敵とプレイヤーを一体ずつ生成（この行をコメントアウトすると消える）

	//定数
	static constexpr int PHASE_TIME = 180;	//フェーズ切り替えの時間（仮）

	static constexpr int LAST_FAZE = 3;


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
	//ゲームシーンのフェーズ遷移中判定
	bool isPhaseChanging_;
	int phaseCnt_;


	//フェーズリザルト
	std::unique_ptr<FazeResult>fazeResult_;
	//リザルト中か
	bool isFazeRezult_;

	//プレイヤー
	std::unique_ptr<PlayerManager>playerMng_;


	//敵
	std::unique_ptr<EnemyManager>enmMng_;

	//倒した敵の総数
	int dunkEnmCnt_;

	int fazeCnt_;

	//チキン
	std::unique_ptr<ChickenManager> chicken_;

	std::unique_ptr<LevelScreenManager> level_;

	std::unique_ptr<UnitPositionLoad> unitLoad_;


	//当たり判定（他項目に干渉するもののみ）
	void Collision(void);
	void CollisionEnemy(void);	//敵関連の当たり判定
	void CollisionPlayer(void);	//プレイヤー関連の当たり判定
	void CollisionPlayerCPU(PlayerBase& _player,const VECTOR& _pPos);	//プレイヤー(CPU)関連の当たり判定

	//フェード
	void Fade(void);

	//フェーズ遷移
	void ChangePhase(void);
	//フェーズ更新(完全暗転中)
	void UpdatePhase(void);
	//フェーズ描画
	void DrawPhase(void);

	//強化要素反映
	void LevelUpReflection();

	//ゲームオーバー判定
	bool IsGameOver(void);
};

