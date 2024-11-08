#pragma once
class DataBank
{
	//cpp��Scene���܂���������񓙂�ۑ�����cpp�ĂԂ񂾂�
public:
	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static DataBank& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// ���\�[�X�̔j��
	void Destroy(void);
private:
	//�v���C���[�̈ʒu���W



	
	// �ÓI�C���X�^���X
	static DataBank* instance_;

	//�R���X�g���N�^�y�уf�X�g���N�^
	DataBank() = default;
	~DataBank() = default;
};

