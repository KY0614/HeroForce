#pragma once
class CharacterParamData
{
public:

	//�C���X�^���X�̐���
	static void CreateInstance(void);

	//�C���X�^���X�̎擾
	static CharacterParamData& GetInstance(void);

	//������
	void Init();

private:

	//�ÓI�C���X�^���X
	static CharacterParamData* instance_;

	CharacterParamData();
	~CharacterParamData() = default;

};

