#pragma once
#include <functional>
#include"../Utility/AsoUtility.h"
#include"Common/Transform.h"
#include "../Common/Vector2.h"
#include "./UnitBase.h"
#include "../Scene/SelectScene.h"

//class SelectScene;
class SelectPlayer;

class SelectImage : public UnitBase
{
public:
	//�摜�֘A
	static constexpr float POINT_SCALE_RATE = 2.0f;	//�g�嗦
	static constexpr int POINT_SCALE = 52;			//���摜�̑傫��(�����`)
	static constexpr int LEFT_POS_X = Application::SCREEN_SIZE_X / 2 - (POINT_SCALE * 5);
	static constexpr int RIGHT_POS_X = Application::SCREEN_SIZE_X / 2 + (POINT_SCALE * 5);
	static constexpr int POINT_POS_Y = Application::SCREEN_SIZE_Y / 2 ;

	//���_�֘A�i�S�_)
	static constexpr int VERTEX_NUM = 4;			//���_��
	static constexpr float VERTEX_ROTSPEED = 1.0f;	//���_����]�����鑬�x

	static constexpr float VERTEX_LEFT_X = -50.0f;	//�摜(���_)����X���W
	static constexpr float VERTEX_RIGHT_X = 50.0f;	//�摜(���_)�E��X���W

	static constexpr float VERTEX_UNDER_Y = 70.0f;	//�摜����Y���W
	static constexpr float VERTEX_TOP_Y = 170.0f;	//�摜���Y���W

	static constexpr float VERTEX_Z = -350.0f;		//���_Z���W
	static constexpr float VERTEX_UNDER_Z = -338.0f;		//���_Z���W

	static constexpr float ROLE_LEFT_X = -90.0f;
	static constexpr float ROLE_RIGHT_X = 0.0f;

	static constexpr float ROLE_UNDER_Y = 50.0f;
	static constexpr float ROLE_TOP_Y = 190.0f;

	static constexpr float ROLE_VERTEX_Z = -338.0f;		//���_Z���W

	//�L�[���͊֘A
	static constexpr float SELECT_TIME = 1.0f;		//�L�[�����o�ߎ���
	static constexpr float INTERVAL_TIME = 0.6f;	//�C���^�[�o�����

	//�\����-----------------------------------------------------------------------
	
	//���b�V��
	struct Mesh {
		VERTEX3D vertex_[VERTEX_NUM];		//���_���

		Mesh() : vertex_(){}
		Mesh(int img) : vertex_(){}

		void DrawTwoMesh(int handle);
	};

	//���
	struct Point {
		Vector2 pos;	//���W
		int w, h;		//w:���,h:����	
		bool isToggle_;	//�I���A�I�t�̐؂�ւ��p
		Mesh mesh_;

		//������
		Point() : pos(0, 0), w(0), h(0), isToggle_(false),mesh_() {}
		Point(int x, int y, int inw, int inh, bool isT,Mesh& mesh) :
			pos(x, y), w(inw), h(inh), isToggle_(isT) , mesh_(mesh)  {}

		void PointDraw(void);//����`�悷��
	};

	//------------------------------------------------------------------------------

	//�R���X�g���N�^
	SelectImage(SelectScene& select, std::shared_ptr<SelectPlayer> player);

	//�f�X�g���N�^
	~SelectImage(void) = default;

	//���
	virtual void Destroy(void);

	//������
	virtual void Init(void)override;
	//�X�V
	virtual void Update(void)override;
	//�`��
	virtual void Draw(void)override;

	void MoveVertexPos(void);

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

	int GetRole(void) { return role_; };	//�I��ł����E���擾

	/// <summary>
	/// ���b�V���̒��_�����擾
	/// </summary>
	/// <param name="i">vertex�z��̎w��</param>
	/// <returns>�w�肵��vertex�̒��_���</returns>
	VERTEX3D GetMeshVertex(int i);

	bool GetReady(void) { return isReady_; };

	VECTOR GetVerPos(void) { return mesh_.vertex_[0].pos; }
	// �Z�b�^�[�@--------------------------------------------------

	/// <summary>
	/// ���b�V�����W��ݒ肷��
	/// </summary>
	/// <param name="pos">�ݒ肷����W</param>
	/// <param name="i">�ݒ肷��vertex�z��̎w��</param>
	void RotMeshPos(VECTOR pos, int i) { mesh_.vertex_[i].pos = pos; }

	void SetPrevPos(VECTOR pos, int i) { prevPos_[i] = pos; }

private:

	//��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;
	//��ԊǗ�(��ԑJ�ڎ���������)
	std::map<SelectScene::SELECT, std::function<void(void)>> stateChanges_;

	//�I�𒆂̎��
	SelectScene::SELECT state_;

	//���b�V��
	Mesh mesh_;	//�l���I���Ƒ���I���̎��̃��b�V��
	
	//�摜�n���h��
	int* imgPlayerNum_;		//�l���I���摜
	int* imgLeftPoint_;		//�������̖��摜
	int* imgRightPoint_;		//�E�����̖��摜

	//���b�V���̒��_���W�p�i4�̒��_�j
	VECTOR leftTop_;		//����
	VECTOR leftBottom_;		//����
	VECTOR rightTop_;		//�E��	
	VECTOR rightBottom_;	//�E��
	VECTOR prevPos_[SceneManager::PLAYER_NUM];
	float angle_;		

	//���̍\����
	Point pointL_;		//��
	Point pointR_;		//�E

	//�v���C���[�l��
	int playerNum_;

	//1P�̓��̓^�C�v
	bool isPad_;	

	//�E��
	int role_;

	//�����I�b�P�[���ǂ���
	bool isReady_;

	//�L�[�����b�����Ă��邩
	float keyPressTime_;
	
	//�L�[�̔�����P�񂾂��擾����p
	bool press_;

	//�l�������Ԋu�ŉ��Z���Ă������߂̃C���^�[�o���p����(���Z���Ď����Z����܂ł̊�)
	float interval_;

	//�C���X�^���X
	SelectScene& selectScene_;
	std::shared_ptr<SelectPlayer> player_;

	VECTOR target_[SceneManager::PLAYER_NUM];

	float lerpTime_;

	//�֐�-------------------------------------------------------------------------------------

	void Load(void);	//�ǂݍ��ݗp

	void InitVertex(void);
	VECTOR RotateVertex(VECTOR pos, VECTOR center, float angle);

	//��ԑJ��
	void ChangeStateNumber(void);
	void ChangeStateOperation(void);
	void ChangeStateRole(void);

	//�X�V�����֘A-----------------------------------------------

	void NumberUpdate(void);		//�l���I�𒆂̏���

	void OperationUpdate(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleUpdate(void);			//��E�I�𒆂̏���

	//�`�揈���֘A-----------------------------------------------

	void NumberDraw(void);			//�l���I�𒆂̏���

	void OperationDraw(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleDraw(void);			//��E�I�𒆂̏���

	void PointsDraw(void);			//���i�Q�Ƃ��j�`��

	//-----------------------------------------------------------
};