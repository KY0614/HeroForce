#pragma once
#include <functional>
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
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//�J�����̒����_���W

	static constexpr VECTOR DEFAULT_TARGET_TWO = { 400.0f, 150.0f, 0.0f };		//�J�����̒����_���W
	static constexpr VECTOR DEFAULT_TARGET_THREE = { 0.0f, 150.0f, 100.0f };	//�J�����̒����_���W
	static constexpr VECTOR DEFAULT_TARGET_FOUR = { -100.0f, 150.0f, -100.0f };	//�J�����̒����_���W

	static constexpr float CHARACTER_SCALE = 0.5f;

	//�I�����Ă�����
	enum class SELECT 
	{
		NUMBER,		//�l��
		OPERATION,	//1P���L�[�{�[�h����ɂ��邩�ǂ���
		ROLE,		//��E
		MAX
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

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void Release(void) override;

	/// <summary>
	/// ��ԑJ��
	/// </summary>
	/// <param name="_state">�J�ڂ�����</param>
	void ChangeSelect(const SELECT _state);

	////�I���������(�l��or��E)�̎�ނ�ς���
	//void ChangeSelect(SELECT select);

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

	void DrawDebug(void);	//�f�o�b�O�`��

	//--------------------------------------------------------------------

private:

	//��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;
	//��ԊǗ�(��ԑJ�ڎ���������)
	std::map<SELECT, std::function<void(void)>> stateChanges_;

	//�X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;

	//�v���C���[
	std::shared_ptr<SelectPlayer>players_[SceneManager::PLAYER_NUM];

	// �摜
	std::unique_ptr<SelectImage>images_[SceneManager::PLAYER_NUM];
	std::unique_ptr<SelectImage>image_;

	//�w�i�̃X�e�[�W
	StageManager* stage_;

	//---------------------------------------------
	Transform trans_;
	Transform tests_[SceneManager::PLAYER_NUM];
	//----------------------------------------------

	//�I�𒆂̎��
	SELECT select_;

	//�f�o�C�X
	SceneManager::CNTL device_;
	SceneManager::CNTL devices_[SceneManager::PLAYER_NUM];

	//�L�[�R���t�B�O
	KEY_CONFIG key_;

	//��ԑJ��
	void ChangeStateNumber(void);
	void ChangeStateOperation(void);
	void ChangeStateRole(void);
	void ChangeStateMax(void);

	//�X�V�����֘A-----------------------------------------------

	void NumberUpdate(void);		//�l���I�𒆂̏���

	void OperationUpdate(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleUpdate(void);			//��E�I�𒆂̏���

	void MaxUpdate(void);			//��

	//�`�揈���֘A-----------------------------------------------

	void NumberDraw(void);			//�l���I�𒆂̏���

	void OperationDraw(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleDraw(void);			//��E�I�𒆂̏���

	//-----------------------------------------------------------

	//�f�o�b�O�֘A-------------------------------
	SceneManager::CNTL selectedCntl_;
	
};

