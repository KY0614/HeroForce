#include "ChickenManager.h"

ChickenManager::ChickenManager(
	std::vector<VECTOR> pos,
	const Transform& stageTrans,
	const Transform& playerTrans)
	: pos_(pos),stageTrans_(stageTrans),playerTrans_(playerTrans)
{
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
	for (auto it = chickens_.begin(); it != chickens_.end(); ) {
		// �^�[�Q�b�g�ʒu�擾
		(*it)->SetTarget(playerTrans_.pos);

		// �X�V����
		(*it)->Update();

		//�Փ˔���
		(*it)->CollisionStage(stageTrans_);

		// �`�L���̍폜
		if ((*it)->GetState() == ChickenBase::STATE::END) {
			it = chickens_.erase(it); // �폜���A���̗v�f�ɐi��
		}
		else {
			++it; // �폜���Ȃ��ꍇ�͎��̗v�f�ɐi��
		}
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