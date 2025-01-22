#include<cassert>
#include "DataBank.h"

DataBank* DataBank::instance_ = nullptr;

void DataBank::CreateInstance(void)
{
	if (instance_ == nullptr){
		instance_ = new DataBank();
	}
	instance_->Init();
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
	playerInfo_[_playerNum - 1].cntrol_ = _cntl;
}

/// <summary>
/// ���[�U�[��CPU�Ȃ̓���
/// </summary>
/// <param name="_mode">���[�U�[��CPU��(SceneManager::PLAY_MODE)���Q��</param>
/// <param name="_playerNum">�v���C���[�ԍ�</param>
void DataBank::Input(const SceneManager::PLAY_MODE _mode, const int _playerNum)
{
	playerInfo_[_playerNum - 1].mode_ = _mode;
}

/// <summary>
/// ��E�̓���
/// </summary>
/// <param name="_role">��E(SceneManager::ROLE)�Q��</param>
/// <param name="_playerNum">�v���C���[�ԍ�</param>
void DataBank::Input(const SceneManager::ROLE _role, const int _playerNum)
{
	playerInfo_[_playerNum - 1].role_ = _role;
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

	case INFO::FAZE_DUNK_ENEMY:
		fazeDunk_ = _num;
		//�����ɒǉ�
		gameDunk_ += _num;
		break;

	case INFO::ALIVE_CHICKEN:
		userNum_ = _num;
		break;

	default:
		//�f�o�b�O�p
		assert("DataBank��Input�̎g�p���@�ɊԈႢ������܂��B");
		break;
	}
}

void DataBank::Input(const INFO _info, const int _x, const int _y)
{
	if (_info != INFO::SCREEN_SIZE)return;

	size_.x_ = _x;
	size_.y_ = _y;
}

/// <summary>
/// �v���C���[���̏o��
/// </summary>
/// <param name="_playerNum">�v���C���[�ԍ�</param>
/// <returns></returns>
const DataBank::PLAYER_INFO DataBank::Output(const int _playerNum)
{
	return playerInfo_[_playerNum - 1];
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

const int DataBank::Output(const INFO _info, const GET_SIZE _pattern)
{
	if (_info != INFO::SCREEN_SIZE || _pattern == GET_SIZE::NONE)return 0;

	if (_pattern == GET_SIZE::X)return size_.x_;
	return size_.y_;
}



void DataBank::Destroy(void)
{
}
