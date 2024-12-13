#pragma once
#include "../Manager/InputManager.h"
#include "../Common/Vector2.h"

class Carsor
{
public:

	//�����ʒu
	static constexpr int DEFAULT_POS_X = 150;
	static constexpr int DEFAULT_POS_Y = 150;

	//�v���C���[�ő�l��
	static constexpr int NUM_MAX = 4;

	//�ړ���
	static constexpr int MOVE_POW = 5;

	//���a
	static constexpr int RADIUS = 32;

	//�p�x
	static constexpr float ANGLE = 315.0f;

	//�L�[����
	struct Key
	{
		int right_;
		int left_;
		int up_;
		int down_;

		int stickX_;
		int stickY_;
		int decide_;
	};
	// ����������
	Carsor();
	~Carsor();

	void Init(const int plNum,const int img);//�v���C���[�i���o�[�Ɖ摜��ނ��󂯎��
	void Update();
	void Draw();

	//����������
	void Reset();

	//����֘A�̐ݒ�
	void SetContllorKey(const int right, const int left, const int up, const int down, const int decide);

	//����̐ݒ�
	void SetDecide(const bool value);

	//���W�Ԃ�
	Vector2 GetPos() const { return pos_; }

	//����ς݂��m�F
	bool IsDecide() const { return decide_; }


private:

	//�v���C���[�i���o�[
	int playerNum_;

	//�摜
	int img_;

	//�g�嗦
	float rate_;

	//���W
	Vector2 pos_;	

	//����
	bool decide_;	

	//�L�[�Ǘ�
	Key key_;	

	//�p�b�h���
	InputManager::JOYPAD_NO pad_;
};