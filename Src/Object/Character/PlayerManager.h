#pragma once
#include"../../Manager/GameSystem/DataBank.h"
#include"../../Manager/Generic/SceneManager.h"

class PlayerBase;

class PlayerManager
{
public:

	//�v���C���[�ɂ��Ă̍\����
	struct PLAYER {
		PlayerBase* ins;	//�������L�����N�^�[�̃C���X�^���X
		DataBank::PLAYER_INFO info;	//���̑����(�ڍׂ�DataBank�Q��)
	};

	//�v���C���[�̐�
	static constexpr int PLAYER_NUM = 4;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	//�v���C���[�̊Ǘ�
	PLAYER players_[PLAYER_NUM];

	//�p�b�h�̎g�p�䐔���Ǘ�
	int padNum_;
	
	PlayerBase* CreateKeybordPlayer(const SceneManager::ROLE _role)const;
	PlayerBase* CreatePadPlayer(const SceneManager::ROLE _role);
	PlayerBase* CreateCpuPlayer(const SceneManager::ROLE _role);
};

