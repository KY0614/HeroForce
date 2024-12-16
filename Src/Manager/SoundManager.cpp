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
	//�A�z�z����ɂ��łɗv�f�������Ă��邩������
	//�����Ă����珈���I��
	if (sounds_.find(_sound) != sounds_.end())return;


	//�Đ�����Ƃ��f�[�^�̎�ނɂ����
	//���[�v�Đ����P�����𔻒f����
	int mode = -1;
	if (_type == TYPE::BGM) mode = DX_PLAYTYPE_LOOP;
	else mode = DX_PLAYTYPE_BACK;

	//�V�K�f�[�^�̂��ߏ���ǉ�
	sounds_.emplace(_sound, SOUND_DATA{ _data,_type ,mode});
}

void SoundManager::Play(const SOUND _sound)
{
	//�Đ�����
	PlaySoundMem(sounds_[_sound].data, sounds_[_sound].playMode);
}

void SoundManager::Stop(const SOUND _sound)
{
	StopSoundMem(sounds_[_sound].data);
}

void SoundManager::Release(void)
{
	//�z��v�f�P�폜
	sounds_.clear();
}

