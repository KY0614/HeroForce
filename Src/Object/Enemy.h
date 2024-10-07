#pragma once
#include<vector>
#include "UnitBase.h"

class EnemyAtk;

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

	//�G�̍U�����擾
	std::vector<EnemyAtk*> GetAtk(void);

private:

	std::vector<EnemyAtk*> atk_;	//�U���p�C���X�^���X
	
	STATE state_;	//���݂̏��

	float atkCdt_;	//�U���̃N�[���_�E���J�E���^

	//�ړ�
	void Move(const float _moveSpeed);

	//�G�̍U������
	void Attack(void);

	//�U���̐�������
	void CreateAtk(void);

	/// <summary>
	/// �L���ȍU���̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns>�g�p���ł͂Ȃ��̈�</returns>
	EnemyAtk* GetValidAtk(void);
};

