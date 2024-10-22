#pragma once
#include "SceneBase.h"

class Grid;
class StageBase;
class SkyDome;
class LevelBase;

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

	//�X�e�[�W
	StageBase* stage_;

	//�X�J�C�h�[��
	SkyDome* sky_;

	// �O���b�h��
	Grid* grid_;

	LevelBase* level_;
};

