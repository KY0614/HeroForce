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
	//�U��������������
	// �����́i�U�������l�A�U���Ώہj�̏�
	const bool IsHitAtk(const UnitBase* _chaser, const UnitBase* _target);

	//���G
	// �����́i���G�����l�A���G�Ώہj�̏�
	const bool Search(const UnitBase* _chaser, const UnitBase* _target);

private:
	//�R���X�g���N�^���f�X�g���N�^
	Collision(void) = default;
	~Collision(void) = default;
	//�C���X�^���X�p
	static Collision* instance_;
};

