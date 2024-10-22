#pragma once
#include <vector>
#include"../Common/Transform.h"


class StageObject;

class StageBase
{
public:

	// �X�e�[�W���f����
	static constexpr int STAGE1_MODELS = 7;

	//�R���X�g���N�^
	StageBase(void);
	//�f�X�g���N�^
	~StageBase(void);
	//���
	virtual void Destroy(void);

	//��{�����̂S�͉��z�֐�������̂ł�������I�[�o�[���C�h����悤��
	//������
	virtual void Init(void);
	//�X�V
	virtual void Update(void);
	//�`��
	virtual void Draw(void);
	//���
	virtual void Release(void);

	void ModelLoad();

	void JsonLoad();

protected:

	std::vector<int> mdlId_;			//���f���ۑ�
	Transform trans_;					//�ʒu���֌W

	std::vector<StageObject*> objs_;

};

