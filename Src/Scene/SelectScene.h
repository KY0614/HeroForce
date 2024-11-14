#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"
#include "../Common/Vector2.h"


class SelectScene :public SceneBase
{
public:

	#define DEBUG_RECT

	//�l�p�`��`�悷�邽�߂ɕK�v�Ȃ���
	struct Rect {
		Vector2 pos;
		int w, h;
		Rect() : pos(0, 0), w(0), h(0) {}
		Rect(float x, float y, int inw, int inh) :
			pos(x, y), w(inw), h(inh) {}

		float Left() { return pos.x - w / 2; }
		float Top() { return pos.y - h / 2; }
		float Right() { return pos.x + w / 2; }
		float Bottom() { return pos.y + h / 2; }

		int color_;

		void Draw(unsigned int color);//�����̋�`��`�悷��
	};

	//�I�����Ă�����
	enum class SELECT 
	{
		NUMBER,		//�l��
		ROLE		//��E
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

	//�I���������(�l��or��E)�̎�ނ�ς���
	void ChangeSelect(SELECT select);

	void ProcessSelect(void);

	//�L�[���͂ƃR���g���[�����͂����ʉ�
	void KeyConfigSetting(void);

	//�J�[�\���ړ�����
	void ProcessCursor(void);

	//�J�[�\���̈ړ���
	void MoveCursor(float pow ,VECTOR dir);

	//���g�p���Ă�����̓f�o�C�X���擾����
	SceneManager::CNTL GetDevice(void);

	//�v���C���[���Q������Ƃ��Ɏg�p�����f�o�C�X
	bool GetJoinDevice(void);

	//���̓f�o�C�X�ύX(����������Ƃ����������@�����肻��)
	void ChangeDevice(SceneManager::CNTL device);

	//��`�Ɖ~�̓����蔻��(�f�o�b�O�p)
	bool IsHitRect(Rect& rc, Vector2 pos , int r);

	//�L�[���͂ƃp�b�h���͂̐���
	void ControllKey(void);

private:
	//std::vector<SceneManager::ROLE> playerNum_;

	SELECT select_;

	SceneManager::CNTL device_;

	KEY_CONFIG key;

	//���W
	VECTOR kPos_;	//�L�[�J�[�\���p���W
	VECTOR cPos_;	//�R���g���[���[�J�[�\���p���W

	//�l�p�`
	Rect rc[SceneManager::PLAYER_NUM];

	int color_;
	int num;
	int role;

	//�ǂݍ��ݗp�֐�
	void Load(void);
};

