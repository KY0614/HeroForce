#pragma once
#include <vector>
#include "../Common/Vector2.h"

class Carsor;

class PowerSelect
{
public:

	//���
	enum class STATE
	{
		NONE,
		EXPANSION,
		SELECT,
		FIN, 
	};

	//�����v�f�̎��
	enum class TYPE
	{
		ATTACK,		//�U���͂̏㏸
		DEFENSE,	//�h��͂̏㏸
		SPEED,		//�ړ����x�̏㏸
		LIFE,		//�ő�̗͂̏㏸
		EVENT1,		//�p�����[�^�ȊO�̌��ʗp(��:�{�X�펞�{�X��ŏ�Ԃɂ���Ȃ�)
		MAX
	};

	//�����v�f��
	static constexpr int POWER_ELEMENT = 4;

	PowerSelect();
	~PowerSelect();

	void Init();
	void Update();
	void Draw();

	//�ϐ��̏�����
	void Reset();

	//�ǂݍ���
	void Load();

	//�������W�̐ݒ�
	void SetFirstPos();

	//��Ԃ̐ݒ�
	void SetState(const STATE state);

	//��Ԃ̎󂯎��
	STATE GetState() const { return state_; }

private:

	//�I��p�摜
	int img_;

	//�����v�f��
	int element_;

	//��������
	struct Element
	{
		//�g�嗦
		float scl_;

		//���W
		Vector2 pos_;

		//���
		TYPE type_;
	};

	//�Փ˔���

	//�G�������g
	std::vector<Element> ele_;

	//���
	STATE state_;

	//�J�[�\��
	std::vector<Carsor> carsors_;

};

