#pragma once
#include "../Common/Vector2.h"

class Carsor
{
public:

	static constexpr int MOVE_POW = 5;

	static constexpr int RADIUS = 10;

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

	void Init();
	void Update();
	void Draw();

	//����������
	void Reset();

	//�摜�ǂݍ���
	void Load();

	//����֘A�̐ݒ�
	void SetContllorKey(const int right, const int left, const int up, const int down, const int decide);

	//���W�Ԃ�
	Vector2 GetPos() const { return pos_; }

	bool IsDecide() const { return decide_; }


private:

	int img_;	//�摜

	Vector2 pos_;	//���W

	bool decide_;	//����

	Key key_;	//�L�[�Ǘ�
};