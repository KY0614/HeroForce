#pragma once
#include <string>
#include "../../Common/ImageFader.h"

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

	//�g�嗦
	static constexpr float EXPANSION_RATE = 1.0f;

	//�g��ő�
	static constexpr float EXPANSION_MAX = 1.2f;

	//�ێ��b��
	static constexpr float MAINTAIN_SECONDS = 1.5f;

	//�t�H���g�֘A
	static constexpr int FONT_SIZE = 128;
	static constexpr int FONT_THICK = 3;

	LevelupNotice();
	~LevelupNotice();

	void Init();
	void Update();
	void Draw();
	void Release();

	//�ϐ��̏�����
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

	//�t�H���g
	int font_;

	//�e�L�X�g
	std::string text_;

	//�C���X�^���X
	ImageFader* imgFader_;

};

