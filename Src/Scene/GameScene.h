#pragma once
#include "SceneBase.h"

class Grid;
class SpaceDome;
class PlayerShip;

class GameScene : public SceneBase
{
public:

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �O���b�h��
	Grid* grid_;

	SpaceDome* spaceDome_;

	PlayerShip* playerShip_;
};

