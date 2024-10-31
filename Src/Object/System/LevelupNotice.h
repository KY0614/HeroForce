#pragma once
#include "../../Common/ImageFader.h"
#include "LevelBase.h"

class LevelupNotice
{
public:

	//�摜�Ɋւ���`��̏��
	enum class STATE
	{
		NONE,
		FADE_IN,	//�t�F�[�h�C��
		MAINTAIN,	//�ێ�
		FIN,		//�I��
	};

	LevelupNotice();
	~LevelupNotice();

	void Init();
	void Update();
	void Draw();

	void Reset();

	//��Ԃ̐ݒ�
	void SetState(const STATE state);

	//��Ԃ̎󂯎��
	STATE GetState() const { return state_; }

private:

	//���
	STATE state_;

	//�摜
	int img_;

	//�J�E���g
	int cnt_;

	//�傫��
	float scl_;

	//�C���X�^���X
	LevelBase* level_;
	ImageFader* imgFader_;

};

