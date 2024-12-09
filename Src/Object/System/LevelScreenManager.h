#pragma once

class Fader;
class LevelupNotice;
class LevelupSelect;

class LevelScreenManager
{
public:

	//��ԊǗ�
	enum class STATE
	{
		NONE,		//�Ȃ�
		NOTICE,		//�ʒm
		SELECT,		//�I��
		FIN
	};	

	//�����v�f�̎��
	enum class TYPE
	{
		ATTACK,		//�U���͂̏㏸
		DEFENSE,	//�h��͂̏㏸
		SPEED,		//�ړ����x�̏㏸
		LIFE,		//�ő�̗͂̏㏸
	  /*TRAP_FLOOR,
		TRAP_CANNON,
		TRAP_POW_UP_F,
		TRAP_POW_UP_C,*/
		MAX
	};
	
	//�Q�[�W�ő�l
	static constexpr float CONSTANT_GAGE = 100.0f;

	//�A���t�@�l�ő�
	static constexpr float ALPHA_MAX = 150.0f;

	//���߃X�s�[�h
	static constexpr float ALPHA_SPEED = 3.0f;

	//��ލő�
	static constexpr int TYPE_MAX = static_cast<int>(TYPE::MAX);

	//�R���X�g���N�^
	LevelScreenManager(void);

	//�f�X�g���N�^
	~LevelScreenManager(void);



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

	//����������
	void Reset();

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
	inline float GetExp(void)const { return exp_; };
	inline TYPE GetType(void)const { return type_; };
	inline STATE GetState(void)const { return state_; };

	//�f�o�b�O�@�\
	void Debag();

protected:

	//���
	STATE state_;

	//���
	TYPE type_;

	//���݂̃��x��
	int nowLevel_;

	//�o���l
	float exp_;

	//�Q�[�W(���̃��x���܂ł̌o���l��)
	float gauge_;

	//�A���t�@�l
	float alpha_;

	//�C���X�^���X
	Fader* fader_;
	LevelupNotice* notice_;
	LevelupSelect* select_;

};