#pragma once
#include <functional>
#include "SceneBase.h"
#include "../Manager/Generic/SceneManager.h"

class SkyDome;
class StageManager;
class SelectCharacter;
class WaitEnemy;
class SelectImage;

class SelectScene :public SceneBase
{
public:

	//�T�E���h�֘A
	static constexpr int BGM_VOLUME = 128;

	//�L�����N�^�[�֘A
	static constexpr int PLAYER_NUM = SceneManager::PLAYER_NUM;	//�ő�v���C�l��

	static constexpr int ROLE_NUM = SceneManager::PLAYER_NUM;	//�ő��E��

	static constexpr float CHARACTER_SCALE = 0.5f;				//�L�����N�^�[�̊g�嗦

	//�J�����֘A
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };		//�J�����̍��W
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//�J�����̒����_���W


	//�摜�֘A
	static constexpr int IMAGE_POS_Y = 60;		//�摜��Y���W
	static constexpr int WAITIMAGE_POS_Y = 60;	//�ҋ@���摜��Y���W

	//�I�����Ă�����
	enum class SELECT 
	{
		DISPLAY,	//�f�B�X�v���C��
		NUMBER,		//�l��
		OPERATION,	//1P���L�[�{�[�h����ɂ��邩�ǂ���
		ROLE,		//��E
		MAX
	};

	//�L�[�R���t�B�O
	enum class  KEY_CONFIG
	{
		NONE,		//�������͂��Ă��Ȃ�

		UP,			//��
		DOWN,		//��
		LEFT,		//��
		RIGHT,		//�E
					
		DECIDE,		//����
		CANCEL		//�L�����Z��
	};

	//�f�o�C�X���
	struct Device
	{
		SceneManager::CNTL cntl_;	//���͂���f�o�C�X
		KEY_CONFIG config_;			//�L�[�R���t�B�O
	};	

	// �R���X�g���N�^
	SelectScene(void);

	// �f�X�g���N�^
	~SelectScene(void);

	// ����������
	virtual void Init(void) override;
	// �X�V�X�e�b�v
	virtual void Update(void) override;
	// �`�揈��
	virtual void Draw(void) override;
	// �������
	virtual void Release(void) override;

	/// <summary>
	/// ��ԑJ��
	/// </summary>
	/// <param name="_state">�J�ڂ�����</param>
	void ChangeSelect(const SELECT _state);

	/// <summary>
	/// 1P�̃L�[���͂ƃR���g���[�����͂̑��쏈��
	/// </summary>
	/// <param name=""></param>
	void Process1PInput(void);

	/// <summary>
	/// �L�[�{�[�h���쏈��
	/// </summary>
	void KeyBoardProcess(void);

	/// <summary>
	/// �R���g���[���[���쏈��
	/// </summary>
	void PadProcess(void);

	/// <summary>
	/// 1P�̓��̓f�o�C�X��ݒ肷��
	/// </summary>
	/// <param name="cntl">�f�o�C�X�̎��</param>
	void Change1PDevice(SceneManager::CNTL cntl){ input_[0].cntl_ = cntl; }

	/// <summary>
	/// 1P�̓��̓f�o�C�X�̐���
	/// </summary>
	void Control1PDevice(void);

	/// <summary>
	/// �S���������������ǂ����Ԃ�
	/// </summary>
	/// <returns>true:�S���������� false:�N����l�ł������ł͂Ȃ��ꍇ</returns>
	bool IsAllReady(void);

	//�Q�b�^�[(�擾�p�֐�)----------------------------------------------------------

	/// <summary>
	/// 1P�̓��̓f�o�C�X���擾����
	/// </summary>
	/// <returns>1P�̓��̓f�o�C�X</returns>
	SceneManager::CNTL Get1PDevice(void) const{ return input_[0].cntl_; }

	/// <summary>
	/// 1P���̓L�[�R���t�B�O���擾
	/// </summary>
	/// <returns>1P�̓��̓L�[�R���t�B�O</returns>
	KEY_CONFIG Get1PConfig(void) const{ return input_[0].config_; }

	/// <summary>
	/// ���݂̑I���t�F�[�Y���擾
	/// </summary>
	/// <returns>���݂�select�t�F�[�Y</returns>
	SELECT GetSelect(void) const{ return select_; }

private:

	//��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;
	std::function<void(void)> stateDraw_;
	//��ԊǗ�(��ԑJ�ڎ���������)
	std::map<SELECT, std::function<void(void)>> stateChanges_;

	//�X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;

	//�L�����N�^�[
	std::unique_ptr<SelectCharacter>characters_[SceneManager::PLAYER_NUM];

	// �摜
	std::unique_ptr<SelectImage>images_[SceneManager::PLAYER_NUM];

	//�ҋ@���ɏo���G
	std::unique_ptr<WaitEnemy>enemys_[SceneManager::PLAYER_NUM];

	//�w�i�̃X�e�[�W
	std::unique_ptr<StageManager> stage_;

	//�I�𒆂̎��
	SELECT select_;

	//�L�[�R���t�B�O
	KEY_CONFIG key_;

	//���ꂼ��̃v���C���[�̃f�o�C�X�Ɠ���
	Device input_[SceneManager::PLAYER_NUM];

	//�v���C���[�������������ǂ���(true:�������� false:����������)
	bool isOk_[SceneManager::PLAYER_NUM];

	//�摜�֘A
	int imgDisplay_;		//�f�B�X�v���C���I���𑣂��摜
	int imgPlayer_;			//�v���C�l���I���𑣂��摜
	int imgOperation_;		//�d�l�f�o�C�X�I���𑣂��摜
	int imgRole_;			//��E�I���𑣂��摜
	int imgWait_;			//�ҋ@���̉摜

	//��ԑJ��
	void ChangeStateDisplay(void);
	void ChangeStateNumber(void);
	void ChangeStateOperation(void);
	void ChangeStateRole(void);
	void ChangeStateMax(void);

	//�X�V�����֘A-----------------------------------------------

	void DisplayUpdate(void);		//�f�B�X�v���C���I�𒆂̏���

	void NumberUpdate(void);		//�l���I�𒆂̏���

	void OperationUpdate(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleUpdate(void);			//��E�I�𒆂̏���

	void MaxUpdate(void);			//��

	//�`�揈���֘A-----------------------------------------------

	void DisplayDraw(void);			//�f�B�X�v���C���I�𒆂̏���

	void NumberDraw(void);			//�l���I�𒆂̏���

	void OperationDraw(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleDraw(void);			//��E�I�𒆂̏���

	void MaxDraw(void);				//��
};

