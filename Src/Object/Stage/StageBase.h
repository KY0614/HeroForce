#pragma once
#include <vector>
#include <map>
#include <string>
#include"../Common/Transform.h"


class StageObject;

class StageBase
{
public:

	enum class MODEL_TYPE
	{
		BUSH,
		FLOWWERS,
		ROCK_01,
		ROCK_02,
		STUMP,
		TERRAIN,
		TREE,
		MAX
	};

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

	//���f���ǂݍ���
	void ModelLoad();

	//JSON�ǂݍ���
	void JsonLoad();

	//�Q�b�^�[
	std::vector<Transform>
		GetTtans(MODEL_TYPE type);

protected:

	std::vector<int> mdlId_;			//���f���ۑ�
	std::vector<Transform> trans_;		//�ʒu���֌W

	std::vector<StageObject*> objs_;

	std::map < MODEL_TYPE, std::vector<StageObject*>> stageMdls_;

};

