#pragma once
#include"../Object/PlayerBase.h"
#include"../Object/PlayerCpu.h"
#include "SceneBase.h"

class Grid;

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
	//�v���C���[�i�e�X�g�j
	PlayerBase* playerB_;
	PlayerCpu* playerCpu_;
};

