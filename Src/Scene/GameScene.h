#pragma once
#include "SceneBase.h"
#include"../Common/Fader.h"
#include<vector>
#include<memory>

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
	StageBase* stage_;

	//スカイドーム
	SkyDome* sky_;

	//フェーダー
	std::unique_ptr<Fader>fader_;

	// グリッド線
	Grid* grid_;
	//プレイヤー
	std::unique_ptr<PlayerBase>players_[PLAYER_NUM];
	PlayerBase* playerTest_;
	//敵
	std::vector<std::unique_ptr<Enemy>> enemys_;
	Enemy* enemyTest_;

	LevelBase* level_;

	//ゲームシーンのフェーズ遷移中判定
	bool isPhaseChanging_;
	int phaseCnt_;

	//当たり判定（他項目に干渉するもののみ）
	void Collision(void);

	//フェード
	void Fade(void);

	//フェーズ遷移
	void ChangePhase(void);
	//フェーズ更新
	void UpdatePhase(void);
	//フェーズ描画
	void DrawPhase(void);
};

