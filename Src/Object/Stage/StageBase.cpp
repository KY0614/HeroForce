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

// 長いのでnamespaceの省略
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

	// 外部ファイルの３Ｄモデルをロード
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
	// 外部ファイルの読み込み
	std::ifstream ifs("Data/Json/ExportData.json");
	json stageData;
	if (ifs)
	{
		ifs >> stageData;
	}
	else { return; }

	//モデル管理番号(初期値0)
	int i = 0;


	//0.地形
	std::vector<StageObject*>terrains;
	const auto& terrain = stageData["terrain"];
	for (const json& Data : terrain)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
 		objs_.emplace_back(std::move(stageObj));
 		terrains.emplace_back(std::move(stageObj));
	}

	stageMdls_.emplace(MODEL_TYPE::TREE, terrains);

	i++;
	//1.草
	std::vector<StageObject*>bushs;
	const auto& bush = stageData["bush03"];
	for (const json& Data : bush)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
		bushs.emplace_back(std::move(stageObj));

	}

	stageMdls_.emplace(MODEL_TYPE::TREE, bushs);

	i++;
	//2.花
	std::vector<StageObject*>flowers;
	const auto& flower = stageData["flowers02"];
	for (const json& Data : flower)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
		flowers.emplace_back(std::move(stageObj));
	}

	stageMdls_.emplace(MODEL_TYPE::TREE, flowers);

	i++;
	//3.岩1
	std::vector<StageObject*>rocks1;
	const auto& rock1 = stageData["rock01"];
	for (const json& Data : rock1)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
		rocks1.emplace_back(std::move(stageObj));
	}

	stageMdls_.emplace(MODEL_TYPE::TREE, rocks1);

	i++;
	//4.岩2
	std::vector<StageObject*>rocks2;
	const auto& rock2 = stageData["rock04"];
	for (const json& Data : rock2)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
		rocks2.emplace_back(std::move(stageObj));
	}

	stageMdls_.emplace(MODEL_TYPE::TREE, rocks2);

	i++;
	//5.切り株
	std::vector<StageObject*>stumps;
	const auto& stump = stageData["stump01"];
	for (const json& Data : stump)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
		stumps.emplace_back(std::move(stageObj));
	}

	stageMdls_.emplace(MODEL_TYPE::TREE, stumps);

	i++;
	//6.木
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

std::vector<Transform> StageBase::GetTtans(MODEL_TYPE type)
{
	std::vector<Transform> ret;
	for (auto& obj : stageMdls_[type])
	{
		ret.push_back(obj->GetTrans());
	}
	return ret;
}
