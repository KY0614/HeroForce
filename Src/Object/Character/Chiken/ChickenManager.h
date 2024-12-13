#pragma once
#include <vector>
#include <random>
#include <DxLib.h>
#include "../../Common/Transform.h"
#include "../../../Utility/AsoUtility.h"
#include "../PlayerBase.h"
#include "ChickenBase.h"


class ChickenBase;

class ChickenManager
{
public:

	//�`�L��������
	static constexpr int CREATE_COUNT = 10;

	ChickenManager(std::vector<VECTOR> pos);	//�����ʒu�����炤
	~ChickenManager();

	void Init();
	void Update();
	void Draw();

	//�����ʒu�̏����V���b�t������
	void ShufflePos();

	//�v���C���[�ʒu���擾(1P�̂ŗǂ�)
	void SetTargetPos(const VECTOR pos);

private:

	//���X�|�[���ʒu�p���W�z��
	std::vector<VECTOR> pos_;

	//�^�[�Q�b�g�ʒu
	VECTOR targetPos_;

	//�v���C���[�p�g�����X�t�H�[��
	Transform playerTrans_;

	//�`�L���C���X�^���X
	std::vector<std::shared_ptr<ChickenBase>> chickens_;
};
