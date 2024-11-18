#pragma once

class Fader
{

public:

	// ��ʂ������Â�����悤�̃A���t�@�l
	static constexpr int LITTLE_ALPHA = 150;

	// ��ʂ������Â����鎞�̃X�s�[�h
	static constexpr float LITTLE_FADE_OUT_SPEED = 2.0f;

	// �t�F�[�h���i�ޑ���
	static constexpr float SPEED_ALPHA = 5.0f;

	// ���
	enum class STATE
	{
		NONE, 
		FADE_OUT,		// ���X�ɈÓ]
		FADE_IN,		// ���X�ɖ��]	
		SET_FADE_OUT	// �A���t�@�l�����߂ĈÓ]
	};

	// ��Ԃ̎擾
	STATE GetState(void) const;

	// �t�F�[�h�������I�����Ă��邩
	bool IsEnd(void) const;

	// �w��t�F�[�h���J�n����
	void SetFade(STATE state);

	// �ǂꂭ�炢�Â����邩�̃A���t�@�l�����߂�
	void SetAlpha(float alpha);


	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);

protected:

	// ���
	STATE state_;

	// �Ó]�E���]�p�����x
	float alpha_;

	// �����x�̎w��l�p
	float alphaMax_;

	// ���(STATE)��ۂ����܂܏I��������s�����߁A
	// Update->Draw->Update��1�t���[������p
	bool isPreEnd_;

	// �t�F�[�h�����̏I������
	bool isEnd_;

};
