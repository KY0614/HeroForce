#include<cassert>
#include "DataBank.h"

DataBank* DataBank::instance_ = nullptr;

void DataBank::CreateInstance(void)
{
	
}

DataBank& DataBank::GetInstance(void)
{
	return *instance_;
}

void DataBank::Init(void)
{
}

/// <summary>
/// �R���g���[�����@�̓���
/// </summary>
/// <param name="_cntl">�R���g���[���̕��@(SceneManager::CNTL)���Q��</param>
/// <param name="_playerNum">�v���C���[�ԍ��i��P��)</param>
void DataBank::Input(const SceneManager::CNTL _cntl, const int _playerNum)
{
	playerInfo_[_playerNum].cntrol_ = _cntl;
}

/// <summary>
/// ���[�U�[��CPU�Ȃ̓���
/// </summary>
/// <param name="_mode">���[�U�[��CPU��(SceneManager::PLAY_MODE)���Q��</param>
/// <param name="_playerNum">�v���C���[�ԍ�</param>
void DataBank::Input(const SceneManager::PLAY_MODE _mode, const int _playerNum)
{
	playerInfo_[_playerNum].mode_ = _mode;
}

/// <summary>
/// ��E�̓���
/// </summary>
/// <param name="_role">��E(SceneManager::ROLE)�Q��</param>
/// <param name="_playerNum">�v���C���[�ԍ�</param>
void DataBank::Input(const SceneManager::ROLE _role, const int _playerNum)
{
	playerInfo_[_playerNum].role_ = _role;
}

/// <summary>
/// �V�X�e���Ɋւ��鐔�l���� �����[�U�[���E�f�B�X�v���C���ȊO�Ŏg�p���Ȃ����ƁI�I
/// </summary>
/// <param name="_info">�ݒ�Ώ�
/// (INFO��USER��DISPLAY�̂�)</param>
/// <param name="_num">���l</param>
void DataBank::Input(const INFO _info, const int _num)
{
	switch (_info)
	{
	case INFO::DHISPLAY_NUM:
		displayNum_ = _num;
		break;

	case INFO::USER_NUM:
		userNum_ = _num;
		break;
	default:
		//�f�o�b�O�p
		assert("DataBank��Input�̎g�p���@�ɊԈႢ������܂��B");
		break;
	}
}

/// <summary>
/// �v���C���[���̏o��
/// </summary>
/// <param name="_playerNum">�v���C���[�ԍ�</param>
/// <returns></returns>
const DataBank::PLAYER_INFO DataBank::Output(const int _playerNum)
{
	return playerInfo_[_playerNum];
}

/// <summary>
/// �V�X�e���Ɋւ��鐔�l�o�́i��ʐ��E���[�U�[���p�j
/// </summary>
/// <param name="_info">�Ώ�
/// (INFO��USER��DISPLAY�̂�)</param>
/// <returns></returns>
const int DataBank::Output(const INFO _info)
{
	switch (_info)
	{
	case INFO::DHISPLAY_NUM:
		return displayNum_;
		break;

	case INFO::USER_NUM:
		return userNum_;
		break;
	default:
		//�f�o�b�O�p
		assert("DataBank��Input�̎g�p���@�ɊԈႢ������܂��B");
		break;
	}
}



void DataBank::Destroy(void)
{
}