#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include"../Common/Transform.h"


class StageObject;

class StageManager
{
public:

	//�Փ˔���̎d��
	enum class HIT_TYPE
	{
		NONE,
		SPHERE,
		CAPSULE,
		MODEL,
	};

	//�J�v�Z�����W�p
	enum class LR
	{
		L,
		R
	};

	//���f�����
	enum class MODEL_TYPE
	{
		BARREL,
		BENCH,
		FENCES_LONG,
		FENCES_SHORT,
		GROUND,
		HOUSE_01,
		HOUSE_02,
		HOUSE_03,
		HOUSE_04,
		HOUSE_05,
		HUNGER_01,
		HUNGER_02,
		ROCK_01,
		ROCK_02,
		ROCK_03,
		SACK,
		TABLE,
		TREE_01,
		TREE_02,
		TREE_03,
		TREE_04,
		TREE_05,
		WAGON,
		WELL,
		WOOD,
		MAX
	};
	
	// �X�e�[�W���f����
	static constexpr int MODELS = static_cast<int>(MODEL_TYPE::MAX);

	//�Փ˔���p�萔
	//���̂Ŕ��肷��I�u�W�F�N�g
	static constexpr VECTOR BARREL_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };				//�M
	static constexpr VECTOR BENCH_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };				//�x���`
	static constexpr VECTOR FENCES_LONG_COLI_RADIUS = { 30.0f, 0.0f, 3200.0f };			//�t�F���X�����O
	static constexpr VECTOR FENCES_SHORT_COLI_RADIUS = { 30.0f, 0.0f, 1450.0f };		//�t�F���X�V���[�g
	static constexpr VECTOR HOUSE_01_COLI_RADIUS = { 470.0f, 0.0f, 260.0f };			//��1
	static constexpr VECTOR HOUSE_02_COLI_RADIUS = { 700.0f, 0.0f, 50.0f };				//��2
	static constexpr VECTOR HOUSE_03_COLI_RADIUS = { 700.0f, 0.0f, 50.0f };				//��3
	static constexpr VECTOR HOUSE_04_COLI_RADIUS = { 380.0f, 0.0f, 200.0f };			//��4
	static constexpr VECTOR HOUSE_05_COLI_RADIUS = { 580.0f, 0.0f, 100.0f };			//��5
	static constexpr VECTOR HUNGER_01_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };			//������1
	static constexpr VECTOR HUNGER_02_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };			//������2
	static constexpr VECTOR ROCK_01_COLI_RADIUS = { 140.0f, 0.0f, 0.0f };				//��1
	static constexpr VECTOR ROCK_02_COLI_RADIUS = { 140.0f, 0.0f, 0.0f };				//��2
	static constexpr VECTOR ROCK_03_COLI_RADIUS = { 140.0f, 0.0f, 0.0f };				//��3
	static constexpr VECTOR SACK_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };				//���
	static constexpr VECTOR TABLE_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };				//��
	static constexpr VECTOR TREE_01_COLI_RADIUS = { 60.0f, 0.0f, 0.0f };				//��1
	static constexpr VECTOR TREE_02_COLI_RADIUS = { 60.0f, 0.0f, 0.0f };				//��2
	static constexpr VECTOR TREE_03_COLI_RADIUS = { 60.0f, 0.0f, 0.0f };				//��3
	static constexpr VECTOR TREE_04_COLI_RADIUS = { 120.0f, 0.0f, 0.0f };				//��4
	static constexpr VECTOR TREE_05_COLI_RADIUS = { 120.0f, 0.0f, 0.0f };				//��5
	static constexpr VECTOR WAGON_COLI_RADIUS = { 100.0f, 0.0f,30.0f };					//�׎�
	static constexpr VECTOR WELL_COLI_RADIUS = { 100.0f, 0.0f, 100.0f };				//���
	static constexpr VECTOR WOOD_COLI_RADIUS = { 100.0f, 0.0f, 50.0f };					//�ۑ�

	//�R���X�g���N�^
	StageManager(void);

	//�f�X�g���N�^
	~StageManager(void);

	//���
	virtual void Destroy(void);

	//��{�����̂S�͉��z�֐�������̂ł�������I�[�o�[���C�h����悤��
	virtual void Init(void);	//������
	virtual void Update(void);	//�X�V
	virtual void Draw(void);	//�`��
	virtual void Release(void);	//���

	//�ǂݍ���JSON�t�@�C����I��
	void SetJsonFile();

	//���f���ǂݍ���
	void ModelLoad();

	//JSON�ǂݍ���
	void JsonLoad();

	//�f�o�b�O�p�`��
	void DebugDraw();

	//�Q�b�^�[
	std::vector<Transform>GetTtans(MODEL_TYPE type);								//�I�u�W�F�N�g���Ƃ̃g�����X�t�H�[��
	float GetRadius(MODEL_TYPE type);												//�Փ˔���p���a
	VECTOR GetHitCapsulePos(VECTOR pos, MODEL_TYPE type, Transform trans, LR lr);	//�J�v�Z�����W(LR:���E�̂ǂ��炩�w��)

protected:

	//JSON�t�@�C��
	std::string jsonFile_;

	//JSON���O
	std::vector<std::string> names_;

	//�Փ˔���p���a
	std::vector<VECTOR> radius_;

	//���f���ۑ�
	std::vector<int> mdlId_;

	//�Փ˔�����@
	std::vector<HIT_TYPE> hitTypes_;	

	std::vector<StageObject*> objs_;
	std::map < MODEL_TYPE, std::vector<StageObject*>> stageMdls_;

};

