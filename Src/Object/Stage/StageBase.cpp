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
	const auto& terrain = stageData["terrain"];
	for (const json& Data : terrain)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
 		objs_.emplace_back(std::move(stageObj));
	}

	i++;
	//1.草
	const auto& bush = stageData["bush03"];
	for (const json& Data : bush)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
	}

	i++;
	//2.花
	const auto& flowers = stageData["flowers02"];
	for (const json& Data : flowers)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
	}

	i++;
	//3.岩1
	const auto& rock1 = stageData["rock01"];
	for (const json& Data : rock1)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
	}

	i++;
	//4.岩2
	const auto& rock2 = stageData["rock04"];
	for (const json& Data : rock2)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
	}

	i++;
	//5.切り株
	const auto& stump = stageData["stump01"];
	for (const json& Data : stump)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
	}

	i++;
	//6.木
	const auto& tree = stageData["tree05"];
	for (const json& Data : tree)
	{
		auto stageObj = new StageObject(Data, mdlId_[i]);
		objs_.emplace_back(std::move(stageObj));
	}

	ifs.close();
}