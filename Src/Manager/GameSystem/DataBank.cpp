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
/// コントロール方法の入力
/// </summary>
/// <param name="_cntl">コントロールの方法(SceneManager::CNTL)を参照</param>
/// <param name="_playerNum">プレイヤー番号（何Pか)</param>
void DataBank::Input(const SceneManager::CNTL _cntl, const int _playerNum)
{
	playerInfo_[_playerNum - 1].cntrol_ = _cntl;
}

/// <summary>
/// ユーザーかCPU科の入力
/// </summary>
/// <param name="_mode">ユーザーかCPUか(SceneManager::PLAY_MODE)を参照</param>
/// <param name="_playerNum">プレイヤー番号</param>
void DataBank::Input(const SceneManager::PLAY_MODE _mode, const int _playerNum)
{
	playerInfo_[_playerNum - 1].mode_ = _mode;
}

/// <summary>
/// 役職の入力
/// </summary>
/// <param name="_role">役職(SceneManager::ROLE)参照</param>
/// <param name="_playerNum">プレイヤー番号</param>
void DataBank::Input(const SceneManager::ROLE _role, const int _playerNum)
{
	playerInfo_[_playerNum - 1].role_ = _role;
}

/// <summary>
/// システムに関する数値入力 ※ユーザー数・ディスプレイ数以外で使用しないこと！！
/// </summary>
/// <param name="_info">設定対象
/// (INFOのUSERとDISPLAYのみ)</param>
/// <param name="_num">数値</param>
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

/// <summary>
/// プレイヤー情報の出力
/// </summary>
/// <param name="_playerNum">プレイヤー番号</param>
/// <returns></returns>
const DataBank::PLAYER_INFO DataBank::Output(const int _playerNum)
{
	return playerInfo_[_playerNum - 1];
}

/// <summary>
/// システムに関する数値出力（画面数・ユーザー数用）
/// </summary>
/// <param name="_info">対象
/// (INFOのUSERとDISPLAYのみ)</param>
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
