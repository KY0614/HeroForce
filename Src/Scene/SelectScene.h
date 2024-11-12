#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"


class SelectScene :public SceneBase
{
public:

	//�I�����Ă�����
	enum class SELECT 
	{
		NUMBER,		//�l��
		ROLE		//��E
	};

	//���̓f�o�C�X
	enum class DEVICE
	{
		NONE,
		KEY,
		PAD
	};

	// �L�[�R���t�B�O
	enum class  KEY_CONFIG
	{
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		DECIDE
	};

	// �R���X�g���N�^
	SelectScene(void);

	// �f�X�g���N�^
	~SelectScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	//�l���I�𒆂̏���
	void NumberUpdate(void);

	//��E�I�𒆂̏���
	void RoleUpdate(void);

	//�f�o�b�O�`��
	void DrawDebug(void);

	//�I��������̂̎�ނ�ς���
	void ChangeSelect(SELECT select);

	void ProcessSelect(void);

	//�L�[���͂ƃR���g���[�����͂����ʉ�
	void KeyConfigSetting(void);

	//�J�[�\���ړ�����
	void ProcessCursor(void);

	//�J�[�\���̈ړ���
	void MoveCursor(float pow ,VECTOR dir);

	//���g�p���Ă�����̓f�o�C�X���擾����
	DEVICE GetDevice(void);

	//�v���C���[���Q������Ƃ��Ɏg�p�����f�o�C�X
	DEVICE GetJoinDevice(void);

	void SetDevice(DEVICE device);

private:
	int playerNum_[SceneManager::PLAYER_NUM];

	SELECT select_;

	DEVICE device_;

	KEY_CONFIG key;

	VECTOR pos_;

	int img_;

	//�ǂݍ��ݗp�֐�
	void Load(void);
};

