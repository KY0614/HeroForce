#pragma once
#include<unordered_map>

class SoundManager
{
public:
	//�Đ��̎�ޕ�������Ƃ��Ɏg��
	enum class TYPE {
		NONE,
		BGM,
		SE
	};

	enum class SOUND {
		//�����Ɏg�p���鉹�y����ʉ��Ȃǂ𗅗�
		NONE,
	};

	struct SOUND_DATA
	{
		int data;		//�����f�[�^�i�[
		TYPE type;		//�����f�[�^�̎��	�����̗v�f�͍폜���邩��
		int playMode;	//�����f�[�^�̍Đ��^�C�v
	};

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SoundManager& GetInstance(void);

	/// <summary>
	/// �T�E���h�̒ǉ�
	/// </summary>
	/// <param name="_type">���̎�ޕ���(SE��BGM��)</param>
	/// <param name="_sound">��̓I�ȗp�r</param>
	/// <param name="_data">���̃f�[�^</param>
	void Add(const TYPE _type, const SOUND _sound, const int _data);

	void Play(const SOUND _sound);

	void Stop(const SOUND _sound);

	void Release(void);
private:
	//�C���X�^���X�p
	static SoundManager* instance_;

	//�f�[�^�i�[�p
	std::unordered_map<SOUND, SOUND_DATA>sounds_;

	//�R���X�g���N�^���f�X�g���N�^
	SoundManager() = default;
	~SoundManager() = default;
};

