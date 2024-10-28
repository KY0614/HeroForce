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


	//�U��������������
	// �����́i�U�������l�A�U���Ώہj�̏�
	//�v���C���[
	const bool IsHitAtk(const PlayerBase& _player, const Enemy& _enemy);
	//�G
	const bool IsHitAtk(const Enemy& _enemy, const PlayerBase& _player);

	//���G
	// �����́i���G�����l�A���G�Ώہj�̏�
	const bool Search(const UnitBase& _chaser, const UnitBase& _target);

private:
	//�R���X�g���N�^���f�X�g���N�^
	Collision(void) = default;
	~Collision(void) = default;


};

