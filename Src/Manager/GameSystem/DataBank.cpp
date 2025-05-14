#include<cassert>
#include "DataBank.h"

DataBank* DataBank::instance_ = nullptr;

void DataBank::CreateInstance(void)
{
	if (instance_ == nullptr) {
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


void DataBank::Input(const SceneManager::CNTL _cntl, const int _playerNum)
{
	playerInfo_[_playerNum - 1].cntrol_ = _cntl;
}

void DataBank::Input(const SceneManager::PLAY_MODE _mode, const int _playerNum)
{
	playerInfo_[_playerNum - 1].mode_ = _mode;
}


void DataBank::Input(const SceneManager::ROLE _role, const int _playerNum)
{
	playerInfo_[_playerNum - 1].role_ = _role;
}

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
		//総数に追加
		gameDunk_ += _num;
		break;

	case INFO::ALIVE_CHICKEN:
		aliveChikenNum_ = _num;
		break;

	default:
		//デバッグ用
		assert("DataBankのInputの使用方法に間違いがあります。");
		break;
	}
}

const DataBank::PLAYER_INFO DataBank::Output(const int _playerNum)
{
	return playerInfo_[_playerNum - 1];
}

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

	case INFO::FAZE_DUNK_ENEMY:
		return fazeDunk_;
		break;

	case INFO::GAME_DUNK_ENEMY:
		return gameDunk_;
		break;

	case INFO::ALIVE_CHICKEN:
		return aliveChikenNum_;
		break;
		//デバッグ用
		assert("DataBankのInputの使用方法に間違いがあります。");
		break;
	}
}



void DataBank::Destroy(void)
{
}
