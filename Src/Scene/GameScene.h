#pragma once
#include "SceneBase.h"

class Grid;
class SpaceDome;
class PlayerShip;

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

	// グリッド線
	Grid* grid_;

	SpaceDome* spaceDome_;

	PlayerShip* playerShip_;
};

