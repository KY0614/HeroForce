#pragma once
class LevelBase
{
public:

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

	//�`��
	virtual void Draw(void);

	//�ǂݍ���
	void Load(void);

	//�o���l�̐ݒ�
	void SetExp(float value);

	//�Q�[�W�̐ݒ�
	void SetGage(int level);
	
	//�o���l�̏�Ԋm�F
	void CheckExp();

	//�Q�b�^�[
	const float GetExp(void)const { return exp_; };

	//�f�o�b�O�@�\
	void Debag();

protected:

	//���݂̃��x��
	int nowLevel_;

	//�o���l
	float exp_;

	//�Q�[�W(���̃��x���܂ł̌o���l��)
	float gage_;

};

