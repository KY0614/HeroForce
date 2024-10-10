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
	//�`��
	void Draw(void)override;

	/// <summary>
	/// �_���[�W
	/// </summary>
	/// <param name="_damage">�_���[�W��</param>
	//void Damage(float _damage);

private:
	
	STATE state_;	//���݂̏��

	float atkCdt_;			//�U���̃N�[���_�E���J�E���^

	std::vector<ATK> atk_;	//�U�����

	//�X�V(�ʏ�)
	void UpdateNml(void);
	//�X�V(�U��)
	void UpdateAtk(void);
	//�X�V(���S)
	void UpdateDead(void);

	//�ړ�
	void Move(void);
	//�ړ�����
	void ProcessMove(const float _moveSpeed, const float _deg);
	//�����]��
	void Turn(const float _deg, const VECTOR _axis);

	//�G�̍U������
	void Attack(void);

	//�U���̐���
	void CreateAtk(void);

	//�L���ȍU�����擾
	const ATK GetValidAtk(void);
};

