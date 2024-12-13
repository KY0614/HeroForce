#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"
#include "../Common/Vector2.h"

class SkyDome;
class StageManager;
class SelectPlayer;
class SelectImage;

class SelectScene :public SceneBase
{
public:

	//#define DEBUG_RECT

	//�O�p�`�p�̒萔
	static constexpr int TRI_SCALE = 150;	//�傫��
	static constexpr int TRI_POS_X = Application::SCREEN_SIZE_X / 2;	//X���W
	static constexpr int TRI_POS_Y = Application::SCREEN_SIZE_Y / 2;	//Y���W

	//�l�p�`�̑傫��
	static constexpr int RECT_SCALE = 300;

	//�}�`���m�̊Ԋu
	static constexpr int PRI_SPACE = 100;


	////�L�[�����o�ߎ���
	//static constexpr float SELECT_TIME = 1.0f;

	////�C���^�[�o�����
	//static constexpr float INTERVAL_TIME = 0.5f;

	static constexpr int PLAYER_NUM = SceneManager::PLAYER_NUM;
	static constexpr int ROLE_NUM = SceneManager::PLAYER_NUM;

	//�J�����֘A
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };		//�J�����̍��W
	static constexpr VECTOR DEFAULT_TARGET_ONE = { 0.0f, 150.0f, -100.0f };		//�J�����̒����_���W
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//�J�����̒����_���W
	static constexpr VECTOR DEFAULT_TARGET_TWO = { 100.0f, 150.0f, -100.0f };		//�J�����̒����_���W
	static constexpr VECTOR DEFAULT_TARGET_THREE = { 0.0f, 150.0f, 100.0f };		//�J�����̒����_���W
	static constexpr VECTOR DEFAULT_TARGET_FOUR = { -100.0f, 150.0f, -100.0f };		//�J�����̒����_���W

	static constexpr float CHARACTER_SCALE = 0.5f;

	//�l�p�`��`�悷�邽�߂ɕK�v�Ȃ���
	struct Rect {
		Vector2 pos;
		int w, h;
		int color_;

		Rect() : pos(0, 0), w(0), h(0), color_(0) {}
		Rect(float x, float y, int inw, int inh, int col) :
			pos(x, y), w(inw), h(inh) , color_(col) {}

		int Left() { return pos.x - w / 2; }
		int Top() { return pos.y - h / 2; }
		int Right() { return pos.x + w / 2; }
		int Bottom() { return pos.y + h / 2; }

		void Draw(unsigned int color);//�����̋�`��`�悷��
	};

	//�O�p�`
	struct Tri {
		Vector2 pos;
		int w, h;	//w:���,h:����	
		bool isToggle_;
		int color_;

		//�������q
		Tri() : pos(0, 0), w(0), h(0) ,isToggle_(false), color_(0) {}
		Tri(float x, float y, int inw, int inh,bool isT,int col) :
			pos(x, y), w(inw), h(inh), isToggle_(isT) ,color_(col) {}

		//���̎O�_��x,y���W
		int LeftX_L() { return pos.x + h / 2;}
		int LeftY_L() { return pos.y + w / 2;}
		int TopX_L()  { return pos.x - h / 2;}
		int TopY_L()  { return pos.y; }
		int RightX_L(){ return pos.x + h / 2;}
		int RightY_L(){ return pos.y - w / 2;}

		//�E�̎O�_��x,y���W
		int LeftX_R() { return pos.x - h / 2; }
		int LeftY_R() { return pos.y + w / 2; }
		int TopX_R() { return pos.x + h / 2; }
		int TopY_R() { return pos.y; }
		int RightX_R() { return pos.x - h / 2; }
		int RightY_R() { return pos.y - w / 2; }

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

	void InitModel(void);

	//�X�V�����֘A-----------------------------------------------
	
	void NumberUpdate(void);		//�l���I�𒆂̏���
	
	void OperationUpdate(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleUpdate(void);			//��E�I�𒆂̏���

	//�`�揈���֘A-----------------------------------------------

	void NumberDraw(void);			//�l���I�𒆂̏���

	void OperationDraw(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleDraw(void);			//��E�I�𒆂̏���

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

	KEY_CONFIG GetKeyConfig(void) { return key_; }	//���̓L�[���擾

	SELECT GetSelect(void) { return select_; }		//���݂̑I���t�F�[�Y���擾

	//�f�o�b�O�֘A--------------------------------------------------------

	//�f�o�b�O�`��
	void DrawDebug(void);

	//--------------------------------------------------------------------

private:
	//�X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;

	//�v���C���[
	std::unique_ptr<SelectPlayer>players_[SceneManager::PLAYER_NUM];

	// �摜
	std::unique_ptr<SelectImage>images_[SceneManager::PLAYER_NUM];
	std::unique_ptr<SelectImage>image_;

	//�w�i�̃X�e�[�W
	StageManager* stage_;

	Transform trans_;
	Transform tests_[SceneManager::PLAYER_NUM];

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
	//int playerNum_;
	
	//bool isPad_;	//1P�̓��̓^�C�v
	SceneManager::CNTL selectedCntl_;
	
	//int role_;	//�E��

	////�L�[�����b�����Ă��邩
	//float keyPressTime_;

	//bool press_;

	////�l�������Ԋu�ŉ��Z���Ă������߂̃C���^�[�o���p����(���Z���Ď����Z����܂ł̊�)
	//float interval_;

};
