#pragma once
#include"../Utility/AsoUtility.h"
#include"Common/Transform.h"
#include "../Common/Vector2.h"
#include "./UnitBase.h"

class SelectScene;

class SelectImage : public UnitBase
{
public:
	//�摜�֘A
	static constexpr float POINT_SCALE_RATE = 2.0f;	//�g�嗦
	static constexpr int POINT_SCALE = 52;			//���摜�̑傫��(�����`)

	//�L�[���͊֘A
	static constexpr float SELECT_TIME = 1.0f;		//�L�[�����o�ߎ���
	static constexpr float INTERVAL_TIME = 0.5f;	//�C���^�[�o�����

	//���
	struct Point {
		Vector2 pos;
		int w, h;	//w:���,h:����	
		bool isToggle_;

		//�������q
		Point() : pos(0, 0), w(0), h(0), isToggle_(false) {}
		Point(float x, float y, int inw, int inh, bool isT) :
			pos(x, y), w(inw), h(inh), isToggle_(isT) {}

		void PointDraw(unsigned int color);//����`�悷��
	};

	//�R���X�g���N�^
	SelectImage(SelectScene& select);

	//�f�X�g���N�^
	~SelectImage(void) = default;

	//���
	virtual void Destroy(void);

	//��{�����̂S�͉��z�֐�������̂ł�������I�[�o�[���C�h����悤��
	//������
	virtual void Init(void)override;
	//�X�V
	virtual void Update(void)override;
	//�`��
	virtual void Draw(void)override;

private:
	
	//�摜�n���h��
	int* imgPlayerNum_;
	int imgLeftPoint_;
	int imgRightPoint_;

	//�v���C���[�l��
	int playerNum_;

	//�L�[�����b�����Ă��邩
	float keyPressTime_;
	
	//�L�[�̔�����P�񂾂��擾����p
	bool press_;

	//�l�������Ԋu�ŉ��Z���Ă������߂̃C���^�[�o���p����(���Z���Ď����Z����܂ł̊�)
	float interval_;

	SelectScene& selectScene_;

	void Load(void);	//�ǂݍ��ݗp

	//�X�V�����֘A-----------------------------------------------

	void NumberUpdate(void);		//�l���I�𒆂̏���

	void OperationUpdate(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleUpdate(void);			//��E�I�𒆂̏���
};

