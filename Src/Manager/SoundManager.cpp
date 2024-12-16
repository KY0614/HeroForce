#include<DxLib.h>
#include "SoundManager.h"

SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::CreateInstance(void)
{
	if (instance_ == nullptr) {
		instance_ = new SoundManager();
	}
}

SoundManager& SoundManager::GetInstance(void)
{
	return *instance_;
}

void SoundManager::Add(const TYPE _type, const SOUND _sound, const int _data)
{
	//連想配列内にすでに要素が入っているかを検索
	//入っていたら処理終了
	if (sounds_.find(_sound) != sounds_.end())return;


	//再生するときデータの種類によって
	//ループ再生か単発かを判断する
	int mode = -1;
	if (_type == TYPE::BGM) mode = DX_PLAYTYPE_LOOP;
	else mode = DX_PLAYTYPE_BACK;

	//新規データのため情報を追加
	sounds_.emplace(_sound, SOUND_DATA{ _data,_type ,mode});
}

void SoundManager::Play(const SOUND _sound)
{
	//再生処理
	PlaySoundMem(sounds_[_sound].data, sounds_[_sound].playMode);
}

void SoundManager::Stop(const SOUND _sound)
{
	StopSoundMem(sounds_[_sound].data);
}

void SoundManager::Release(void)
{
	//配列要素善削除
	sounds_.clear();
}

