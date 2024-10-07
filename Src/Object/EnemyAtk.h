#pragma once
#include"Common/Transform.h"

class Enemy;

class EnemyAtk
{
public:
	
	static constexpr float ATK_DURATION = 10.0f;	//�U���̎�������

	//�R���X�g���N�^
	EnemyAtk();
	
	//�f�X�g���N�^
	~EnemyAtk() = default;

	//���
	void Destroy(void);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_parent">�e�̃C���X�^���X</param>
	void Init(Enemy* _parent);
	//�X�V
	void Update(void);
	//�`��
	void Draw(void);

	//��������(true:������)
	const bool IsAlive(void) const { return atkDuration_ < ATK_DURATION; };

private:

	Enemy* enemy_;	//�e�Q�Ɨp

	Transform trans_;	//�ʒu���֌W
	float atkDuration_;	//�U���̎������ԃJ�E���^
};

