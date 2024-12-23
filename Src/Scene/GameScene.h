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
	//�f�o�b�O�p
	//#define _DEBUG_COL	//�e�X�g�p�̓G�ƃv���C���[����̂������i���̍s���R�����g�A�E�g����Ə�����j

	//�萔
	//�v���C���[�̐�
	static constexpr int PLAYER_NUM = 4;

	static constexpr int PHASE_TIME = 180;


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
	std::unique_ptr<StageManager> stage_;

	//�X�J�C�h�[��
	std::unique_ptr<SkyDome> sky_;

	//�t�F�[�_�[
	std::unique_ptr<Fader>fader_;

	//�v���C���[
	std::unique_ptr<PlayerBase>players_[PLAYER_NUM];
	PlayerBase* playerTest_;
	//�G
	std::vector<std::unique_ptr<Enemy>> enemys_;
	Enemy* enemyTest_;

	//�`�L��
	std::unique_ptr<ChickenManager> chicken_;

	std::unique_ptr<LevelScreenManager> level_;

	std::unique_ptr<UnitPositionLoad> unitLoad_;

	//�Q�[���V�[���̃t�F�[�Y�J�ڒ�����
	bool isPhaseChanging_;
	int phaseCnt_;

	//�����蔻��i�����ڂɊ�������̂̂݁j
	void Collision(void);
	void CollisionEnemy(void);	//�G�֘A�̓����蔻��
	void CollisionPlayer(void);	//�v���C���[�֘A�̓����蔻��
	void CollisionPlayerCPU(PlayerBase& _player,const VECTOR& _pPos);	//�v���C���[(CPU)�֘A�̓����蔻��

	//�t�F�[�h
	void Fade(void);

	//�t�F�[�Y�J��
	void ChangePhase(void);
	//�t�F�[�Y�X�V
	void UpdatePhase(void);
	//�t�F�[�Y�`��
	void DrawPhase(void);

	//�����v�f���f
	void LevelUpReflection();
};

