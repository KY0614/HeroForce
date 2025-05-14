#pragma once
#include<DxLib.h>
#include"../Manager/Generic/SceneManager.h"

class DataBank
{
	//cpp��Scene���܂���������񓙂�ۑ�����
public:
	/// <summary>
	/// �v���C���[���
	/// </summary>
	struct PLAYER_INFO
	{
		SceneManager::CNTL cntrol_;
		SceneManager::PLAY_MODE mode_;
		SceneManager::ROLE role_;
	};

	/// <summary>
	/// �e����
	/// </summary>
	enum class INFO
	{
		NONE,
		CNTL,
		MODE,
		ROLE,
		USER_NUM,
		DHISPLAY_NUM,
		FAZE_DUNK_ENEMY,
		GAME_DUNK_ENEMY,
		ALIVE_CHICKEN
	};

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static DataBank& GetInstance(void);

	void Init(void);

#pragma region ����
	/// <summary>
	/// �R���g���[�����@�̓���
	/// </summary>
	/// <param name="_cntl">�R���g���[���̕��@(SceneManager::CNTL)���Q��</param>
	/// <param name="_playerNum">�v���C���[�ԍ��i��P��)</param>
	void Input(const SceneManager::CNTL _cntl, const int _playerNum);


	/// <summary>
	/// ���[�U�[��CPU���̓���
	/// </summary>
	/// <param name="_mode">���[�U�[��CPU��(SceneManager::PLAY_MODE)���Q��</param>
	/// <param name="_playerNum">�v���C���[�ԍ�</param>
	void Input(const SceneManager::PLAY_MODE _mode, const int _playerNum);

	/// <summary>
	/// ��E�̓���
	/// </summary>
	/// <param name="_role">��E(SceneManager::ROLE)�Q��</param>
	/// <param name="_playerNum">�v���C���[�ԍ�</param>
	void Input(const SceneManager::ROLE _role, const int _playerNum);

	/// <summary>
	/// �V�X�e���Ɋւ��鐔�l���� �����[�U�[���E�f�B�X�v���C���ȊO�Ŏg�p���Ȃ����ƁI�I
	/// </summary>
	/// <param name="_info">�ݒ�Ώ�
	/// (INFO��USER��DISPLAY�̂�)</param>
	/// <param name="_num">���l</param>
	void Input(const INFO _info, const int _num);
#pragma endregion

#pragma region �o��
	/// <summary>
	/// �v���C���[���̏o��
	/// </summary>
	/// <param name="_playerNum">�v���C���[�ԍ�</param>
	/// <returns>�v���C���[�Ɋւ�����</returns>
	const PLAYER_INFO Output(const int _playerNum);

	/// <summary>
	/// �V�X�e���Ɋւ��鐔�l�o�́i��ʐ��E���[�U�[���p�j
	/// </summary>
	/// <param name="_info">�Ώ�
	/// (INFO��USER��DISPLAY�̂�)</param>
	/// <returns>�V�X�e���Ɋւ�����</returns>
	const int Output(const INFO _info);
#pragma endregion

	// ���\�[�X�̔j��
	void Destroy(void);

private:
	//�e�L�����N�^�[�̑���^�C�v��ۑ�
	PLAYER_INFO playerInfo_[SceneManager::PLAYER_NUM];

	//���[�U�[�̐l��(�̂��̂��E�B���h�E�̐��Ƃ��Ă����p����)
	int userNum_;

	//�f�B�X�v���C�̐�
	int displayNum_;

	//���U���g���
	int fazeDunk_;		//�t�F�[�Y���Ƃ̓|�����G
	int gameDunk_;		//�|�����G����
	int aliveChikenNum_;	//�j���g��������

	// �ÓI�C���X�^���X
	static DataBank* instance_;

	//�R���X�g���N�^�y�уf�X�g���N�^
	DataBank() = default;
	~DataBank() = default;
};


