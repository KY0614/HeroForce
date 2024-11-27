#pragma once

class Fader
{

public:

	// �ʒm���̃A���t�@�l
	static constexpr int NOTICE_ALPHA = 150;
	static constexpr float NOTICE_SPEED_ALPHA = 2.0f;

	// �t�F�[�h���i�ޑ���
	static constexpr float SPEED_SCENE = 5.0f;	//�V�[���J��
	static constexpr float SPEED_PHASE = 2.0f;	//�t�F�[�Y�J��
	

	// ���
	enum class STATE
	{
		NONE,			//�������Ă��Ȃ�
		FADE_OUT,		// ���X�ɈÓ]
		FADE_IN,		// ���X�ɖ��]	
		FADE_NOTICE	,	//���x���A�b�v�ʒm�p
		FADE_KEEP,		//���݂̏�Ԃ�ۂ�
	};

	// ��Ԃ̎擾
	STATE GetState(void) const;

	// �t�F�[�h�������I�����Ă��邩
	bool IsEnd(void) const;

	// �w��t�F�[�h���J�n����
	void SetFade(STATE state);


	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);

protected:

	// ���
	STATE state_;

	// �����x
	float alpha_;

	//�t�F�[�h���x
	float speed_;

	// ���(STATE)��ۂ����܂܏I��������s�����߁A
	// Update->Draw->Update��1�t���[������p
	bool isPreEnd_;

	// �t�F�[�h�����̏I������
	bool isEnd_;

};
