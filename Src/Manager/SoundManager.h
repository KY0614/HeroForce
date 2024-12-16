#pragma once
class SoundManager
{
public:
	//���������ޕ�������Ƃ��Ɏg�������H
	//�K�v�Ȃ������Ȃ�����Ă���
	enum class TYPE {
		NONE,
		BGM,
		SE
	};

	enum class SOUND {
		//�����Ɏg�p���鉹�y����ʉ��Ȃǂ𗅗�
	};


	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SoundManager& GetInstance(void);

	/// <summary>
	/// �T�E���h�̒ǉ�
	/// </summary>
	/// <param name="_type">���</param>
	/// <param name="_data">�f�[�^</param>
	void Add(const SOUND _type, const int _data);

	void Play(const SOUND _type);

	void Stop(const SOUND _type);

	void Release(void);
private:
	//�C���X�^���X�p
	static SoundManager* instance_;

	//�R���X�g���N�^���f�X�g���N�^
	SoundManager() = default;
	~SoundManager() = default;
};

