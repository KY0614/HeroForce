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
	//�f�o�b�O�p
	//#define _DEBUG_COL	//�e�X�g�p�̓G�ƃv���C���[����̂������i���̍s���R�����g�A�E�g����Ə�����j

	//�萔
	static constexpr int PHASE_TIME = 180;	//�t�F�[�Y�؂�ւ��̎��ԁi���j

	static constexpr int LAST_FAZE = 3;


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
	//�Q�[���V�[���̃t�F�[�Y�J�ڒ�����
	bool isPhaseChanging_;
	int phaseCnt_;


	//�t�F�[�Y���U���g
	std::unique_ptr<FazeResult>fazeResult_;
	//���U���g����
	bool isFazeRezult_;

	//�v���C���[
	std::unique_ptr<PlayerManager>playerMng_;


	//�G
	std::unique_ptr<EnemyManager>enmMng_;

	//�|�����G�̑���
	int dunkEnmCnt_;

	int fazeCnt_;

	//�`�L��
	std::unique_ptr<ChickenManager> chicken_;

	std::unique_ptr<LevelScreenManager> level_;

	std::unique_ptr<UnitPositionLoad> unitLoad_;


	//�����蔻��i�����ڂɊ�������̂̂݁j
	void Collision(void);
	void CollisionEnemy(void);	//�G�֘A�̓����蔻��
	void CollisionPlayer(void);	//�v���C���[�֘A�̓����蔻��
	void CollisionPlayerCPU(PlayerBase& _player,const VECTOR& _pPos);	//�v���C���[(CPU)�֘A�̓����蔻��

	//�t�F�[�h
	void Fade(void);

	//�t�F�[�Y�J��
	void ChangePhase(void);
	//�t�F�[�Y�X�V(���S�Ó]��)
	void UpdatePhase(void);
	//�t�F�[�Y�`��
	void DrawPhase(void);

	//�����v�f���f
	void LevelUpReflection();

	//�Q�[���I�[�o�[����
	bool IsGameOver(void);
};

