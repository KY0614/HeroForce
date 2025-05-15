#pragma once
#include <functional>
#include "./UnitBase.h"
#include "../Scene/SelectScene.h"


class SelectCharacter;

class SelectImage : public UnitBase
{
public:
	//#define DRAW_DEBUG

	//�摜�֘A
	static constexpr float POINT_SCALE = 52.0f;		//���摜�̑傫��
	static constexpr float IMAGE_SCALE = 50.0f;		//�摜�̑傫��

	//���_�֘A�i�S�_)--------------------------------------------------------------

	//�l���I���Ȃǂ̕\���Ɏg�����b�V��
	static constexpr int VERTEX_NUM = 4;			//���_��
	static constexpr float VERTEX_ROTSPEED = 1.0f;	//���_����]�����鑬�x

	static constexpr float VERTEX_LEFT_X = -25.0f;	//�摜(���_)����X���W
	static constexpr float VERTEX_RIGHT_X = 25.0f;	//�摜(���_)�E��X���W
	
	static constexpr float VERTEX_TOP_Y = 150.0f;	//�摜���Y���W	
	static constexpr float VERTEX_UNDER_Y = 90.0f;	//�摜����Y���W 

	static constexpr float VERTEX_Z = -400.0f;		//�摜���Z���W
	static constexpr float VERTEX_UNDER_Z = -392.0f;//�摜����Z���W

	//���(���̂ق��͂��̂܂܂ŉE�̏ꍇ��LEFT��RIGHT�����ւ��ă}�C�i�X�l��������)
	static constexpr float POINT_LEFT_X = -50.0f;	//�摜(���_)����X���W
	static constexpr float POINT_RIGHT_X = -28.0f;	//�摜(���_)�E��X���W
	static constexpr float POINT_TOP_Y = 130.0f;	//�摜(���_)����Y���W
	static constexpr float POINT_UNDER_Y = 110.0f;	//�摜(���_)���Y���W
													
	static constexpr float POINT_TOP_Z = VERTEX_Z;	//�摜���Z���W
	static constexpr float POINT_UNDER_Z = -402.0f;	//�摜����Z���W

	static constexpr float ROLE_MESH_LEFT_X = -55.0f;
	static constexpr float ROLE_MESH_RIGHT_X = 15.0f;

	static constexpr float ROLE_MESH_TOP_Z = 170.0f;
	static constexpr float ROLE_MESH_UNDER_Z = 100.0f;

	static constexpr int BLEND_PARAM = 128;			//�u�����h���[�h�̋���

	//���������p���b�V���֘A
	static constexpr VECTOR READYMESH_LEFT_TOP = { 80.0f, 70.0f, -408.0f };
	static constexpr VECTOR READYMESH_LEFT_BOT = { -80.0f, 70.0f, -408.0f };
	static constexpr VECTOR READYMESH_RIGHT_TOP = { 80.0f, 160.0f, -410.0f };
	static constexpr VECTOR READYMESH_RIGHT_BOT = { 80.0f, 70.0f, -408.0f };

	//�L�[���͊֘A
	static constexpr float SELECT_TIME = 1.0f;		//�L�[�����o�ߎ���
	static constexpr float INTERVAL_TIME = 0.6f;	//�C���^�[�o�����

	//�\����-----------------------------------------------------------------------
	
	//���b�V��
	struct Mesh {
		VERTEX3D vertex_[VERTEX_NUM];	//���_���

		//������
		Mesh() : vertex_(){}

		/// <summary>
		/// ���b�V����`�悷��
		/// </summary>
		/// <param name="handle">�摜�n���h��</param>
		void DrawTwoMesh(int handle);		
	};

	//���
	struct Point {
		bool isToggle_;	//�I���A�I�t�̐؂�ւ��p
		Mesh mesh_;		//���p�̃��b�V��

		//������
		Point() : isToggle_(false),mesh_() {}
		Point(const bool isToggle,Mesh& mesh) : isToggle_(isToggle) , mesh_(mesh)  {}
	};

	//------------------------------------------------------------------------------

	//�R���X�g���N�^
	SelectImage(SelectScene& select);

	//�f�X�g���N�^
	~SelectImage(void) = default;

	//���
	void Destroy(void);

	//������
	void Init(void)override;
	//�X�V
	void Update(void)override;
	//�`��
	void Draw(void)override;

	/// <summary>
	/// ��E�I���̍ۂɕ\�����郁�b�V���̍��W�ֈړ�������
	/// </summary>
	void MoveVertexPos(void);

	/// <summary>
	/// ComingSoon�摜�p�̃��b�V���k���p
	/// </summary>
	void ReductionVertexPos(void);

	/// <summary>
	/// �I�����Ă���I�u�W�F�N�g��ύX����
	/// </summary>
	/// <param name="input">���̓f�o�C�X</param>
	/// <param name="i">���̓f�o�C�X�ɉ������v���C���[�i���o�[</param>
	void ChangeObject(SelectScene::Device& input,int i);

	/// <summary>
	/// ��ԑJ��
	/// </summary>
	/// <param name="_state">�J�ڂ�����</param>
	void ChangeSelect(const SelectScene::SELECT _state);

	//�Q�b�^�[�@---------------------------------------------------

	/// <summary>
	/// �I�����Ă����E���擾
	/// </summary>
	/// <returns>�I�𒆂̖�E</returns>
	int GetRole(void)const { return role_; };	

	/// <summary>
	/// ���b�V���̒��_�����擾
	/// </summary>
	/// <param name="index">vertex�z��̗v�f�w��</param>
	/// <returns>�w�肵���v�f�̒��_���</returns>
	VERTEX3D GetMeshVertexAtIndex(int index)const{ return mesh_.vertex_[index]; };

	/// <summary>
	/// ���������摜�p�̃��b�V�����_�����擾
	/// </summary>
	/// <param name="index">vertex�z��̗v�f�w��</param>
	/// <returns>�w�肵���v�f�̒��_���</returns>
	VERTEX3D GetReadyMeshVertexAtIndex(int index) const{ return readyMesh_.vertex_[index]; };

	/// <summary>
	/// �����摜�p�̃��b�V�����_�����擾
	/// </summary>
	/// <param name="index">vertex�z��̗v�f�w��</param>
	/// <returns>�w�肵���v�f�̒��_���</returns>
	VERTEX3D GetPointLMeshVertexAtIndex(int index)const{ return pointL_.mesh_.vertex_[index]; };

	/// <summary>
	/// �E���摜�p�̃��b�V�����_�����擾
	/// </summary>
	/// <param name="index">vertex�z��̗v�f�w��</param>
	/// <returns>�w�肵���v�f�̒��_���</returns>
	VERTEX3D GetPointRMeshVertexAtIndex(int index)const{ return pointR_.mesh_.vertex_[index]; };

	/// <summary>
	/// �����������ǂ����擾����
	/// </summary>
	/// <returns>true:�������� false:������</returns>
	bool GetReady(void)const { return isReady_; };

	// �Z�b�^�[�@--------------------------------------------------

	/// <summary>
	/// ���b�V�����W��ݒ肷��
	/// </summary>
	/// <param name="pos">�ݒ肷����W</param>
	/// <param name="index">�ݒ肷��vertex�z��̗v�f</param>
	void RotMeshPos(VECTOR pos, int index) { mesh_.vertex_[index].pos = pos; }

	/// <summary>
	/// �E���摜�p�̃��b�V�����W��ݒ肷��
	/// </summary>
	/// <param name="pos">�ݒ肷����W</param>
	/// <param name="index">�ݒ肷��vertex�z��̗v�f</param>
	void RotReadyMeshPos(VECTOR pos, int index) { readyMesh_.vertex_[index].pos = pos; }

	/// <summary>
	/// �E���摜�p�̃��b�V�����W��ݒ肷��
	/// </summary>
	/// <param name="pos">�ݒ肷����W</param>
	/// <param name="index">�ݒ肷��vertex�z��̗v�f</param>
	void RotPointLMeshPos(VECTOR pos, int index) { pointL_.mesh_.vertex_[index].pos = pos; }

	/// <summary>
	/// �E���摜�p�̃��b�V�����W��ݒ肷��
	/// </summary>
	/// <param name="pos">�ݒ肷����W</param>
	/// <param name="index">�ݒ肷��vertex�z��̗v�f</param>
	void RotPointRMeshPos(VECTOR pos, int index) { pointR_.mesh_.vertex_[index].pos = pos; }

private:

	//��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;
	//��ԊǗ�(��ԑJ�ڎ���������)
	std::map<SelectScene::SELECT, std::function<void(void)>> stateChanges_;

	SelectScene& selectScene_;			//�Z���N�g�V�[��

	//�I�𒆂̎��
	SelectScene::SELECT state_;

	//���b�V��
	Mesh mesh_;			//�l���I���Ƒ���I���̎��̃��b�V��
	Mesh readyMesh_;	//���������̎��̃��b�V��
	
	//�摜�n���h��
	int* imgPlayerNum_;		//�l���I���摜
	int* imgDisplayNum_;	//�f�B�X�v���C���I���摜
	int* imgLeftPoint_;		//�������̖��摜
	int* imgRightPoint_;	//�E�����̖��摜
	int* imgReady_;			//���������摜
	int* imgRoleNum_;		//��E�̉摜
	int* imgDeviceNum_;		//��E�̉摜
	int* imgComingSoon_;	//ComingSoon�̉摜

	//���b�V���̒��_���W�p�i4�̒��_�j
	VECTOR leftTop_;		//����
	VECTOR leftBottom_;		//����
	VECTOR rightTop_;		//�E��	
	VECTOR rightBottom_;	//�E��
	float angle_;		

	//���̍\����
	Point pointL_;		//��
	Point pointR_;		//�E

	//�f�B�X�v���C��
	int displayNum_;

	//�v���C���[�l��
	int playerNum_;

	//1P�̓��̓^�C�v
	bool isPad_;	

	//�E��
	int role_;

	//�����I�b�P�[���ǂ���
	bool isReady_;

	//�L�[��������
	float keyPressTime_;
	
	//�L�[��������Ă��邩
	bool isPress_;

	//���Ԋu�ŉ��Z���Ă������߂̃C���^�[�o���p����(���Z���Ď����Z����܂ł̊�)
	float addIntervalTime_;

	//�J�����p�̃^�[�Q�b�g���W
	VECTOR target_[SceneManager::PLAYER_NUM];

	//�֐�-------------------------------------------------------------------------------------

	//�ǂݍ��ݗp
	void LoadImages(void);	
	void LoadSounds(void);	

	//���_���W������
	void InitVertex(void);

	//��ԑJ��
	void ChangeStateDisplay(void);
	void ChangeStateNumber(void);
	void ChangeStateOperation(void);
	void ChangeStateRole(void);

	//�X�V�����֘A-----------------------------------------------

	void DisplayUpdate(void);		//�f�B�X�v���C���I�𒆂̏���

	void NumberUpdate(void);		//�l���I�𒆂̏���

	void OperationUpdate(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleUpdate(void);			//��E�I�𒆂̏���

	//�`�揈���֘A--------------------------------------------

	void DisplayDraw(void);			//�f�B�X�v���C���I�𒆂̏���

	void NumberDraw(void);			//�l���I�𒆂̏���

	void OperationDraw(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleDraw(void);			//��E�I�𒆂̏���

	void PointsDraw(void);			//���i�Q�Ƃ��j�`��

	//-----------------------------------------------------------
};