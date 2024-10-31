#pragma once

class UnitBase;
class PlayerBase;
class Enemy;

class Collision
{
public:
	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static Collision& GetInstance(void);


	void Init(void);
	/// <summary>
	/// �U��������������
	/// </summary>
	/// <param name="_chaser">�U��������l</param>
	/// <param name="_target">�U�����󂯂�l</param>
	/// <returns></returns>
	const bool IsHitAtk(const UnitBase* _chaser, const UnitBase* _target);

	//���G
	// �����́i���G�����l�A���G�Ώہj�̏�
	const bool Search(const UnitBase* _chaser, const UnitBase* _target);

	/// <summary>
	/// �ړI�͈̔͂ɑΏۂ�����������Ԃ�
	/// </summary>
	/// <param name="_myPos">���g�̍��W</param>
	/// <param name="_targetPos">�Ώۂ̍��W</param>
	/// <param name="_rangeRadius">�͈͗p�̔��a</param>
	/// <returns></returns>
	const bool Search(const VECTOR _myPos, const VECTOR _targetPos, const float _rangeRadius)const;

private:
	//�R���X�g���N�^���f�X�g���N�^
	Collision(void) = default;
	~Collision(void) = default;
	//�C���X�^���X�p
	static Collision* instance_;
};
