#pragma once
#include "SceneBase.h"
#include<vector>

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
	//�f�o�b�O�p
	#define _DEBUG_COL	//�e�X�g�p�̓G�ƃv���C���[����̂������i���̍s���R�����g�A�E�g����Ə�����j

	//�萔
	static constexpr int PLAYER_NUM = 4;

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
	StageManager* stage_;

	//�X�J�C�h�[��
	SkyDome* sky_;

	// �O���b�h��
	Grid* grid_;
	//�v���C���[
	PlayerBase* players_[PLAYER_NUM];
	PlayerBase* playerTest_;
	//�G
	std::vector<Enemy*> enemys_;
	Enemy* enemyTest_;

	//�`�L��
	ChickenManager* chicken_;

	LevelScreenManager* level_;

	UnitPositionLoad* unitLoad_;

	//�����蔻��i�����ڂɊ�������̂̂݁j
	void Collision(void);

	//�����v�f���f
	void LevelUpReflection();
};

