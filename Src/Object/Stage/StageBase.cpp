#include <string>
#include <memory>
#include <fstream>
#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/InputManager.h"
#include "../../Lib/nlohmann/json.hpp"
#include "../../Utility/AsoUtility.h"
#include "StageBase.h"
#include "StageObject.h"

// �����̂�namespace�̏ȗ�
using json = nlohmann::json;


StageBase::StageBase(void)
{
}

StageBase::~StageBase(void)
{
}

void StageBase::Destroy(void)
{
}

void StageBase::Init(void)
{
	ModelLoad();

	JsonLoad();
}

void StageBase::Update(void)
{
}

void StageBase::Draw(void)
{
	for (auto& object : objs_)
	{
		object->Draw();
	}

	DebugDraw();
}

void StageBase::Release(void)
{
	for (auto& object : objs_)
	{
		object->Release();
	}
}

void StageBase::ModelLoad()
{
	mdlId_.resize(STAGE1_MODELS);
	int i = 0;
	auto& res = ResourceManager::GetInstance();

	// �O���t�@�C���̂R�c���f�������[�h
	mdlId_[i] =
		res.LoadModelDuplicate(ResourceManager::SRC::TERRAIN);

	i++;
	mdlId_[i] =
		res.LoadModelDuplicate(ResourceManager::SRC::BUSH_03);

	i++;
	mdlId_[i] =
		res.LoadModelDuplicate(ResourceManager::SRC::FLOWERS_02);

	i++;
	mdlId_[i] =
		res.LoadModelDuplicate(ResourceManager::SRC::ROCK_01);

	i++;
	mdlId_[i] =
		res.LoadModelDuplicate(ResourceManager::SRC::ROCK_04);

	i++;
	mdlId_[i] =
		res.LoadModelDuplicate(ResourceManager::SRC::STUMP_01);

	i++;
	mdlId_[i] =
		res.LoadModelDuplicate(ResourceManager::SRC::TREE_05);
}

void StageBase::JsonLoad()
{
	// �O���t�@�C���̓ǂݍ���
	std::ifstream ifs("Data/Json/ExportData.json");
	json stageData;
	if (ifs)
	{
		ifs >> stageData;
	}
	else { return; }

	//���f���Ǘ��ԍ�(�����l0)
	int i = 0;

	//0.�n�`
	std::vector<StageObject*>terrains;
	const auto& terrain = stageData["terrain"];
	for (const json& Data : terrain)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
 		objs_.emplace_back(std::move(stageObj));
 		terrains.emplace_back(std::move(stageObj));
	}

	stageMdls_.emplace(MODEL_TYPE::TERRAIN, terrains);

	i++;
	//1.��
	std::vector<StageObject*>bushs;
	const auto& bush = stageData["bush03"];
	for (const json& Data : bush)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
		bushs.emplace_back(std::move(stageObj));

	}

	stageMdls_.emplace(MODEL_TYPE::BUSH, bushs);

	i++;
	//2.��
	std::vector<StageObject*>flowers;
	const auto& flower = stageData["flowers02"];
	for (const json& Data : flower)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
		flowers.emplace_back(std::move(stageObj));
	}

	stageMdls_.emplace(MODEL_TYPE::FLOWWERS, flowers);

	i++;
	//3.��1
	std::vector<StageObject*>rocks1;
	const auto& rock1 = stageData["rock01"];
	for (const json& Data : rock1)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
		rocks1.emplace_back(std::move(stageObj));
	}

	stageMdls_.emplace(MODEL_TYPE::ROCK_01, rocks1);

	i++;
	//4.��2
	std::vector<StageObject*>rocks2;
	const auto& rock2 = stageData["rock04"];
	for (const json& Data : rock2)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
		rocks2.emplace_back(std::move(stageObj));
	}

	stageMdls_.emplace(MODEL_TYPE::ROCK_02, rocks2);

	i++;
	//5.�؂芔
	std::vector<StageObject*>stumps;
	const auto& stump = stageData["stump01"];
	for (const json& Data : stump)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
		stumps.emplace_back(std::move(stageObj));
	}

	stageMdls_.emplace(MODEL_TYPE::STUMP, stumps);

	i++;
	//6.��
	std::vector<StageObject*>trees;
	const auto& tree = stageData["tree05"];
	for (const json& Data : tree)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
		trees.emplace_back(std::move(stageObj));
	}

	stageMdls_.emplace(MODEL_TYPE::TREE, trees);

	ifs.close();
}

void StageBase::DebugDraw()
{
	int divNum = 20;
	int color = 0xff0000;
	bool fill = false;

	//��01
	std::vector<Transform> rocks1 = GetTtans(MODEL_TYPE::ROCK_01);
	for (auto rock1 : rocks1)
	{
		DrawSphere3D(rock1.pos, ROCK01_COLI_RADIUS, divNum, color, color, fill);
	}

	//��02
	std::vector<Transform> rocks2 = GetTtans(MODEL_TYPE::ROCK_02);
	for (auto rock2 : rocks2)
	{
		DrawSphere3D(rock2.pos, ROCK02_COLI_RADIUS, divNum, color, color, fill);
	}

	//�؂芔
	std::vector<Transform> stumps = GetTtans(MODEL_TYPE::STUMP);
	for (auto stump : stumps)
	{
		DrawSphere3D(stump.pos, STUMP_COLI_RADIUS, divNum, color, color, fill);
	}

	//�؁X
	std::vector<Transform> trees = GetTtans(MODEL_TYPE::TREE);
	for (auto tree : trees)
	{
		DrawSphere3D(tree.pos, TREE_COLI_RADIUS, divNum, color, color, fill);
	}
	
}

std::vector<Transform> StageBase::GetTtans(MODEL_TYPE type)
{
	std::vector<Transform> ret;
	for (auto& obj : stageMdls_[type])
	{
		ret.push_back(obj->GetTrans());
	}
	return ret;
}
