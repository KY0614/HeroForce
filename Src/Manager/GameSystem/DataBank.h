#pragma once
#include<DxLib.h>
#include"../Manager/Generic/SceneManager.h"

class DataBank
{
	//cpp��Scene���܂���������񓙂�ۑ�����
public:

	struct PLAYER_INFO
	{
		SceneManager::CNTL cntrol_;
		SceneManager::PLAY_MODE mode_;
		SceneManager::ROLE role_;
	};

	struct SCREEN_SIZE
	{
		int x_;
		int y_;
	};

	enum class INFO
	{
		NONE,
		CNTL,
		MODE,
		ROLE,
		USER_NUM,
		DHISPLAY_NUM,
		SCREEN_SIZE,
	};

	enum class GET_SIZE {
		NONE,
		X,
		Y
	};

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static DataBank& GetInstance(void);

	void Init(void);
	
	//�f�[�^����
	void Input(const SceneManager::CNTL _cntl,const int _playerNum);		//�R���g���[�����@�̓���
	void Input(const SceneManager::PLAY_MODE _mode,const int _playerNum);	//���[�U�[��CPU�Ȃ̓���
	void Input(const SceneManager::ROLE _role, const int _playerNum);		//��E�̓���
	void Input(const INFO _info, const int _num);							//�V�X�e���Ɋւ��鐔�l���́i��ʐ��E���[�U�[���p�j
	void Input(const INFO _info, const int _x, const int _y);				//�E�B���h�E�̃T�C�Y�ݒ�
	
	//�f�[�^�o��
	const PLAYER_INFO Output(const int _playerNum);	//�v���C���[�Ɋւ������Ԃ�
	const int Output(const INFO _info);				//�V�X�e���Ɋւ��鐔�l�o�́i��ʐ��E���[�U�[���p�j
	const int Output(const INFO _info,const GET_SIZE _pattern);			//�X�N���[���̃T�C�Y�擾(�ǂ��炩�Е��̂�)

	// ���\�[�X�̔j��
	void Destroy(void);

private:
	//�e�L�����N�^�[�̑���^�C�v��ۑ�
	PLAYER_INFO playerInfo_[SceneManager::PLAYER_NUM];

	//���[�U�[�̐l��(�̂��̂��E�B���h�E�̐��Ƃ��Ă����p����)
	int userNum_;

	//�f�B�X�v���C�̐�
	int displayNum_;

	//�X�N���[���T�C�Y��ێ�
	SCREEN_SIZE size_;

	// �ÓI�C���X�^���X
	static DataBank* instance_;

	//�R���X�g���N�^�y�уf�X�g���N�^
	DataBank() = default;
	~DataBank() = default;
};

