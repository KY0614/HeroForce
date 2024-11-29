#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"
#include "../Common/Vector2.h"


class SelectScene :public SceneBase
{
public:

	#define DEBUG_RECT

	//�O�p�`�p�̒萔
	static constexpr int TRI_SCALE = 150;	//�傫��
	static constexpr int TRI_POS_Y = 450;	//Y���W

	//�l�p�`�̑傫��
	static constexpr int RECT_SCALE = 300;

	//�}�`���m�̊Ԋu
	static constexpr int PRI_SPACE = 100;


	//�L�[�����o�ߎ���
	static constexpr float SELECT_TIME = 2.0f;

	//�C���^�[�o�����
	static constexpr float INTERVAL_TIME = 1.0f;

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

	//�O�p�`
	struct Tri {
		Vector2 pos;
		int w, h;	//w:���,h:����	
		bool isToggle_;

		//�������q
		Tri() : pos(0, 0), w(0), h(0) ,isToggle_(false) {}
		Tri(float x, float y, int inw, int inh,bool isT) :
			pos(x, y), w(inw), h(inh), isToggle_(isT) {}

		//���̎O�_��x,y���W
		float LeftX_L() { return pos.x + h / 2;}
		float LeftY_L() { return pos.y + w / 2;}
		float TopX_L()  { return pos.x - h / 2;}
		float TopY_L()  { return pos.y; }
		float RightX_L(){ return pos.x + h / 2;}
		float RightY_L(){ return pos.y - w / 2;}

		//�E�̎O�_��x,y���W
		float LeftX_R() { return pos.x - h / 2; }
		float LeftY_R() { return pos.y + w / 2; }
		float TopX_R() { return pos.x + h / 2; }
		float TopY_R() { return pos.y; }
		float RightX_R() { return pos.x - h / 2; }
		float RightY_R() { return pos.y - w / 2; }

		int color_;

		void LeftDraw(unsigned int color);//�O�p�`��`�悷��
		void RightDraw(unsigned int color);//�O�p�`��`�悷��
	};

	//�I�����Ă�����
	enum class SELECT 
	{
		NUMBER,		//�l��
		OPERATION,	//1P���L�[�{�[�h����ɂ��邩�ǂ���
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

		UP_TRG,
		DOWN_TRG,
		LEFT_TRG,
		RIGHT_TRG,

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

	//�X�V�����֘A-----------------------------------------------
	
	void NumberUpdate(void);		//�l���I�𒆂̏���
	
	void OperationUpdate(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleUpdate(void);			//��E�I�𒆂̏���

	//-----------------------------------------------------------

	//�I���������(�l��or��E)�̎�ނ�ς���
	void ChangeSelect(SELECT select);

	//�L�[���͂ƃR���g���[�����͂����ʉ�
	void KeyConfigSetting(void);

	//���g�p���Ă�����̓f�o�C�X���擾����
	SceneManager::CNTL GetDevice(void);

	//���̓f�o�C�X�ύX(����������Ƃ����������@�����肻��)
	void ChangeDevice(SceneManager::CNTL device);	

	//�L�[���͂ƃp�b�h���͂̐���
	void ControllDevice(void);

	//�Q�b�^�[	----------------------------------------------------------

	KEY_CONFIG GetKeyConfig(void);	//���̓L�[���擾

	//�f�o�b�O�֘A--------------------------------------------------------

	//�f�o�b�O�`��
	void DrawDebug(void);

	//��`�Ɖ~�̓����蔻��(�f�o�b�O�p)
	bool IsHitRect(Rect& rc, Vector2 pos, int r);

	//--------------------------------------------------------------------

private:
	//�I�𒆂̎��
	SELECT select_;

	//�f�o�C�X
	SceneManager::CNTL device_;

	//�L�[�R���t�B�O
	KEY_CONFIG key_;

	//�f�o�b�O�֘A-------------------------------

	//�l�p�`
	Rect rc;

	//�O�p�`
	Tri triL;
	Tri triR;
	
	//�v���C���[�l��
	int playerNum_;
	
	bool isPad_;	//1P�̓��̓^�C�v
	SceneManager::CNTL selectedCntl_;
	
	int role_;	//�E��

	//�L�[�����b�����Ă��邩
	float keyPressTime_;

	bool press_;

	//�l�������Ԋu�ŉ��Z���Ă������߂̃C���^�[�o���p����(���Z���Ď����Z����܂ł̊�)
	float interval_;

};

