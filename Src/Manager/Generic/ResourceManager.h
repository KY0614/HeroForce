#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	//�J�[�\���摜�p�萔
	static constexpr int CARSORS_NUM_X = 4;
	static constexpr int CARSORS_NUM_Y = 1;
	static constexpr int CARSORS_SIZE = 64;

	//�����摜�p�萔
	static constexpr int NUMBERS_NUM_X = 5;
	static constexpr int NUMBERS_NUM_Y = 2;
	static constexpr int NUMBERS_SIZE = 128;

	//���x���A�b�v��ʃG�t�F�N�g�p�萔
	static constexpr int LV_EFE_NUM_X = 10;
	static constexpr int LV_EFE_NUM_Y = 6;
	static constexpr int LV_EFE_SIZE = 256;

	// ���\�[�X��
	enum class SRC
	{
		TITLE,
		GAMEOVER,
		RESTART,
		NEXT,
		//���f��
		STAGE_1,		//�X�e�[�W�@
		PLAYER_MAGE,	//���@�g���i�v���C���[�j
		PLAYER_KNIGHT,	//�R�m�i�v���C���[�j
		PLAYER_ARCHER,	//�|�g���i�v���C���[�j
		PLAYER_AXEMAN,	//���g���i�v���C���[�j
		ENEMY_MAGE,		//���@�g���i�G�j
		ENEMY_BRIGANT,	//�R���i�G�j
		ENEMY_ARCHER,	//�|�g���i�G�j
		ENEMY_AXEMAN,	//���g���i�G�j
		ENEMY_GOLEM,	//�S�[����(�G)

		//�X�e�[�W
		STAGE_01,
		DECO_01,
		SKY_DOME,

		//UI
		HP_PLAYER,
		HP_ENEMY,
		HP_CPU,
		HP_GAGE,
		CIRCLE_GAGE,
		CIRCLE_EXP_GAGE,
		NUMBERS,
		STATUS_MAGE,	//���@�g��
		STATUS_KNIGHT,	//�R�m
		STATUS_ARCHER,	//�|�g��
		STATUS_AXEMAN,	//���g��
		TO_TITLE,
		START,
		TIME,

		//���x���ʒm�p
		LEVEL_UP,
		SELECT_UI,
		CARSOLS,
		LEVEL_SCREEN_EFE,
		LEVEL_UP_BACK,

		//����
		ATTACK_UP_UI,
		DEFENCE_UP_UI,
		SPEED_UP_UI,
		LIFE_UP_UI,
		ATTACK_UP_EFE,
		DEFENCE_UP_EFE,
		SPEED_UP_EFE,
		LIFE_UP_EFE,

		//CPU
		CHICKEN,
		HELP,

		//�I����ʗp
		PLAYER_NUM,		//�l���I��
		RIGHT_POINT,	//�E���
		LEFT_POINT,		//�����
		READY,			//����
		ROLE,			//��E
		DEVICE,			//�f�o�C�X

		//���U���g
		RESULT,
		RANK_S,
		RANK_A,
		RANK_B,
		RANK_C,
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);
	//�V�[�����ƂɃf�[�^��ǂݍ��ނ��Ƃɂ���
	void InitTitle(void);
	void InitSelect(void);
	void InitGame(void);
	void InitResult(void);
	void InitGameOver(void);

	void ResourcePlayer(void);
	void ResourceEnemy(void);
	void ResourceStage(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	Resource Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource*> loadedMap_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);

	// �f�X�g���N�^�����l
	~ResourceManager(void) = default;

	// �������[�h
	Resource* _Load(SRC src);

};

