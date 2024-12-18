#pragma once
#include"../Utility/AsoUtility.h"
#include"Common/Transform.h"
#include "../Common/Vector2.h"
#include "./UnitBase.h"

class SelectScene;
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

	static constexpr float VERTEX_LEFT_X = -50.0f;	//
	static constexpr float VERTEX_RIGHT_X = 50.0f;	//

	static constexpr float VERTEX_UNDER_Y = 70.0f;
	static constexpr float VERTEX_TOP_Y = 170.0f;

	static constexpr float VERTEX_Z = -350.0f;

	//�L�[���͊֘A
	static constexpr float SELECT_TIME = 1.0f;		//�L�[�����o�ߎ���
	static constexpr float INTERVAL_TIME = 0.6f;	//�C���^�[�o�����

	//���
	struct Point {
		Vector2 pos;	//���W
		int w, h;		//w:���,h:����	
		bool isToggle_;	//�I���A�I�t�̐؂�ւ��p
		int imgHandle_;	//�摜�n���h��

		//������
		Point() : pos(0, 0), w(0), h(0), isToggle_(false),imgHandle_(-1) {}
		Point(float x, float y, int inw, int inh, bool isT,int img) :
			pos(x, y), w(inw), h(inh), isToggle_(isT) ,imgHandle_(img) {}

		void PointDraw(void);//����`�悷��
	};

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

	//�֐��|�C���^�g���Ă������肳������
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

private:
	
	//�摜�n���h��
	int* imgPlayerNum_;		//�l���I���摜
	int imgLeftPoint_;		//�������̖��摜
	int imgRightPoint_;		//�E�����̖��摜

	//���b�V���̒��_�f�[�^�i4�̒��_�j
	VERTEX3D vertices_[4];
	VERTEX3D triangle1_[3];
	VERTEX3D triangle2_[3];
	VECTOR leftTop_;
	VECTOR leftBottom_;
	VECTOR rightTop_;
	VECTOR rightBottom_;
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

	//�L�[�����b�����Ă��邩
	float keyPressTime_;
	
	//�L�[�̔�����P�񂾂��擾����p
	bool press_;

	//�l�������Ԋu�ŉ��Z���Ă������߂̃C���^�[�o���p����(���Z���Ď����Z����܂ł̊�)
	float interval_;

	SelectScene& selectScene_;
	std::shared_ptr<SelectPlayer> player_;

	void Load(void);	//�ǂݍ��ݗp

	void InitVertex(void);
	VECTOR RotateVertex(VECTOR pos, VECTOR center, float angle);
};

