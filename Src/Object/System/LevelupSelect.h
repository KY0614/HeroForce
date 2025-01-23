#pragma once
#include <vector>
#include "../Common/Vector2.h"
#include "LevelScreenManager.h"

class Carsor;

class LevelupSelect
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

	//�I�����ڐ�
	static constexpr int SELECT_ELEMENT = 4;

	LevelupSelect();
	~LevelupSelect();

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

	//��ނ̎󂯎��
	inline LevelScreenManager::TYPE GetType() { return selectType_; }
	
	inline LevelScreenManager::TYPE GetRandType();

private:

	//�I��p�摜
	int img_;

	//�����v�f��
	int element_;

	LevelScreenManager::TYPE selectType_;

	//��������
	struct Element
	{
		//�g�嗦
		float scl_;

		//���W
		Vector2 pos_;

		//���
		LevelScreenManager::TYPE type_;
	};

	//�G�������g
	std::vector<Element> ele_;

	//���
	STATE state_;

	//�J�[�\��
	std::vector<Carsor> carsors_;

};

