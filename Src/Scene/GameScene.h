#pragma once
#include "SceneBase.h"

class Grid;
class StageBase;
class SkyDome;
class LevelBase;

class GameScene : public SceneBase
{
public:

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

	LevelBase* level_;
};

