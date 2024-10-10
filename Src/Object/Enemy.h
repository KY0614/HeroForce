#pragma once
#include<vector>
#include "UnitBase.h"

class Enemy : public UnitBase
{
public:

	static constexpr float ATK_CDT = 50.0f;	//�U���̃N�[���_�E���̎���

	//���ݏ��
	enum class STATE
	{
		NML		//�ʏ�
		,ATK	//�U��
		,DEAD	//���S
		,MAX
	};

	//�U���̎��
	enum class ATK_PAT
	{

	};

	//�R���X�g���N�^
	Enemy();
	//�f�X�g���N�^
	~Enemy() = default;

	//���
	void Destroy(void)override;

	//������
	void Init(void)override;
	//�X�V
	void Update(void)override;
	void UpdateNml(void);
	void UpdateAtk(void);
	void UpdateDead(void);

	//�`��
	void Draw(void)override;

	/// <summary>
	/// �_���[�W
	/// </summary>
	/// <param name="_damage">�_���[�W��</param>
	//void Damage(float _damage);

	//�G�̍U���̐�������
	bool IsAtk(void);

private:
	
	STATE state_;	//���݂̏��

	float atkCdt_;						//�U���̃N�[���_�E���J�E���^

	std::vector<Transform> atkTrans_;	//�U���̈ʒu�ˑ��֌W

	//�ړ�
	void Move(void);
	//�ړ�����
	void ProcessMove(const float _moveSpeed, const float _deg);
	//�����]��
	void Turn(float _deg, VECTOR _axis);

	//�G�̍U������
	void Attack(void);
};

