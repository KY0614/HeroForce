#pragma once

class Fader;
class LevelupNotice;
class PowerSelect;

class LevelBase
{
public:

	//��ԊǗ�
	enum class STATE
	{
		NONE,		//�Ȃ�
		NOTICE,		//�ʒm
		SELECT		//�I��
	};

	//�R���X�g���N�^
	LevelBase(void);

	//�f�X�g���N�^
	~LevelBase(void);

	//�萔
	static constexpr float CONSTANT_GAGE = 100;

	//���
	virtual void Destroy(void);

	//��{�����̂S�͉��z�֐�������̂ł�������I�[�o�[���C�h����悤��
	//������
	virtual void Init(void);

	//�X�V
	virtual void Update(void);
	void NoticeUpdate(void);	//�ʒm�p�̍X�V����
	void PowerUpdate(void);		//�����I���̍X�V����

	//�`��
	virtual void Draw(void);

	//���
	virtual void Release(void);

	//�ǂݍ���
	void Load(void);

	//�o���l�̐ݒ�
	void SetExp(const float value);

	//�Q�[�W�̐ݒ�
	void SetGage(const int level);

	//�X�e�[�g�̐ݒ�
	void SetState(const STATE state);
	
	//�o���l�̏�Ԋm�F
	void CheckExp();

	//�Ó]
	void FaderDraw();

	//�Q�b�^�[
	const float GetExp(void)const { return exp_; };

	//�f�o�b�O�@�\
	void Debag();

protected:

	//���
	STATE state_;

	//���݂̃��x��
	int nowLevel_;

	//�o���l
	float exp_;

	//�Q�[�W(���̃��x���܂ł̌o���l��)
	float gauge_;

	//�C���X�^���X
	Fader* fader_;
	LevelupNotice* notice_;
	PowerSelect* power_;

};