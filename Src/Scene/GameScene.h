#pragma once
#include"../Object/PlayerBase.h"
#include"../Object/PlayerCpu.h"
#include "SceneBase.h"

class Grid;

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
	//プレイヤー（テスト）
	PlayerBase* playerB_;
	PlayerCpu* playerCpu_;
};

