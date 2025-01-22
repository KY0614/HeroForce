#pragma once
#include <vector>
#include <random>
#include <DxLib.h>
#include "../../Common/Transform.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/GameSystem/Collision.h"
#include "../../Stage/StageManager.h"
#include "../PlayableChara/PlayerBase.h"
#include "ChickenBase.h"


class ChickenBase;

class ChickenManager
{
public:

	//�`�L��������
	static constexpr int CREATE_COUNT = 10;

	/// <summary>
	/// �`�L���}�l�[�W���[
	/// </summary>
	/// <param name="pos"></param>	�`�L���̐����ʒu���󂯎��
	/// <param name="stageTrans"></param>	�X�e�[�W�̃g�����X�t�H�[��
	/// <param name="playerTrans"></param>	�v���C���[�̃g�����X�t�H�[��
	ChickenManager(
		std::vector<VECTOR> pos,
		const Transform& stageTrans,
		const Transform& playerTrans);
	~ChickenManager();

	void Init();
	void Update();
	void Draw();

	//�����ʒu�̏����V���b�t������
	void ShufflePos();

	//�v���C���[�ʒu���擾(1P�̂ŗǂ�)
	void SetTargetPos(const VECTOR pos);

	//�Փ˔���
	void CollisionStage(const Transform& stageTrans, std::shared_ptr<ChickenBase> cheken);

	//���������擾
	const int GetAliveNum(void)const;

private:

	//���X�|�[���ʒu�p���W�z��
	std::vector<VECTOR> pos_;

	//�^�[�Q�b�g�ʒu
	VECTOR targetPos_;

	//�v���C���[�p�g�����X�t�H�[��
	const Transform &playerTrans_;
	const Transform &stageTrans_;

	//�`�L���C���X�^���X
	std::vector<std::shared_ptr<ChickenBase>> chickens_;
};

