#pragma once

class Fader
{

public:

	// �ʒm���̃A���t�@�l
	static constexpr int NOTICE_ALPHA = 150;
	static constexpr float NOTICE_SPEED_ALPHA = 2.0f;

	// �t�F�[�h���i�ޑ���
	static constexpr float SPEED_ALPHA = 5.0f;

	// ���
	enum class STATE
	{
		NONE, 
		FADE_OUT,		// ���X�ɈÓ]
		FADE_IN,		// ���X�ɖ��]	
		FADE_NOTICE		//���x���A�b�v�ʒm�p
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

	// ���(STATE)��ۂ����܂܏I��������s�����߁A
	// Update->Draw->Update��1�t���[������p
	bool isPreEnd_;

	// �t�F�[�h�����̏I������
	bool isEnd_;

};
