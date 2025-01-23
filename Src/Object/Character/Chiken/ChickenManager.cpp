#include "ChickenManager.h"

ChickenManager::ChickenManager(std::vector<VECTOR> pos)
{
	//���̎󂯎��
	pos_ = pos;

	//������
	targetPos_ = AsoUtility::VECTOR_ZERO;
}

ChickenManager::~ChickenManager()
{
}

void ChickenManager::Init()
{
	//���W�̒��g���V���b�t������
	ShufflePos();

	//�`�L���̐���
	for (int i = 0; i < CREATE_COUNT; i++)
	{
		//�C���X�^���X�쐬
		auto chicken = std::make_shared<ChickenBase>();

		//���W����
		VECTOR pos = pos_[i];

		//����
		chicken->Create(pos);

		//�i�[
		chickens_.emplace_back(std::move(chicken));
	}
}

void ChickenManager::Update()
{
	for (auto c : chickens_)
	{
		//�^�[�Q�b�g�ʒu�擾
		c->SetTarget(targetPos_);

		//�X�V����
		c->Update();
	}
}

void ChickenManager::Draw()
{
	for (auto c : chickens_)
	{
		c->Draw();
	}
}

void ChickenManager::ShufflePos()
{
	// �����_���G���W���𐶐�
	std::random_device rd;
	std::mt19937 gen(rd());

	// �V���b�t��
	std::shuffle(pos_.begin(), pos_.end(), gen);
}

void ChickenManager::SetTargetPos(const VECTOR pos)
{
	targetPos_ = pos;
}
