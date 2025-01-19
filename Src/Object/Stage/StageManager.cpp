#include "../../Application.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Lib/nlohmann/json.hpp"
#include "../../Utility/AsoUtility.h"
#include "StageManager.h"
#include "StageObject.h"

// 長いのでnamespaceの省略
using json = nlohmann::json;


StageManager::StageManager(void)
{
}

StageManager::~StageManager(void)
{
}

void StageManager::Destroy(void)
{
}

void StageManager::Init(void)
{
	SetJsonFile();

	ModelLoad();

	JsonLoad();
}

void StageManager::Update(void)
{
}

void StageManager::Draw(void)
{
	for (auto& object : objs_)
	{
		object->Draw();
	}

	DebugDraw();
}

void StageManager::Release(void)
{
	for (auto& object : objs_)
	{
		object->Release();
	}
}

void StageManager::SetJsonFile()
{
	jsonFile_ = "Data/Json/stage01.json";
}

void StageManager::ModelLoad()
{
	mdlId_.resize(MODELS);
	auto& res = ResourceManager::GetInstance();
	ResourceManager::SRC src = ResourceManager::SRC::STAGE_BARREL;
	using RES_T = ResourceManager::SRC;

	// 外部ファイルの３Ｄモデルをロード
	for (int i = 0; i < MODELS; i++)
	{
		MODEL_TYPE type = static_cast<MODEL_TYPE>(i);
		HIT_TYPE hitType = HIT_TYPE::NONE;
		std::string name = "";
		
		VECTOR radius = AsoUtility::VECTOR_ZERO;

		switch (type)
		{
		case MODEL_TYPE::BARREL:
			src = RES_T::STAGE_BARREL;
			name = "barrel";
			radius = BARREL_COLI_RADIUS;
			hitType = HIT_TYPE::SPHERE;
			break;

		case MODEL_TYPE::BENCH:
			src = RES_T::STAGE_BENCH;
			name = "bench";
			radius = BENCH_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		case MODEL_TYPE::FENCES_LONG:
			src = RES_T::STAGE_FENCES_LONG;
			name = "fencesLong";
			radius = FENCES_LONG_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		case MODEL_TYPE::FENCES_SHORT:
			src = RES_T::STAGE_FENCES_SHORT;
			name = "fencesShort";
			radius = FENCES_SHORT_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		case MODEL_TYPE::GROUND:
			src = RES_T::STAGE_GROUND;
			name = "ground";
			break;

		case MODEL_TYPE::HOUSE_01:
			src = RES_T::STAGE_HOUSE_01;
			name = "house01";
			radius = HOUSE_01_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		case MODEL_TYPE::HOUSE_02:
			src = RES_T::STAGE_HOUSE_02;
			name = "house02";
			radius = HOUSE_02_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		case MODEL_TYPE::HOUSE_03:
			src = RES_T::STAGE_HOUSE_03;
			name = "house03";
			radius = HOUSE_03_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		case MODEL_TYPE::HOUSE_04:
			src = RES_T::STAGE_HOUSE_04;
			name = "house04";
			radius = HOUSE_04_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		case MODEL_TYPE::HOUSE_05:
			src = RES_T::STAGE_HOUSE_05;
			name = "house05";
			radius = HOUSE_05_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		case MODEL_TYPE::HUNGER_01:
			src = RES_T::STAGE_HUNGER_01;
			name = "hunger01";
			radius = HUNGER_01_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		case MODEL_TYPE::HUNGER_02:
			src = RES_T::STAGE_HUNGER_02;
			name = "hunger02";
			radius = HUNGER_02_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		case MODEL_TYPE::ROCK_01:
			src = RES_T::STAGE_ROCK_01;
			name = "rock01";
			radius = ROCK_01_COLI_RADIUS;
			hitType = HIT_TYPE::SPHERE;
			break;

		case MODEL_TYPE::ROCK_02:
			src = RES_T::STAGE_ROCK_02;
			name = "rock02";
			radius = ROCK_02_COLI_RADIUS;
			hitType = HIT_TYPE::SPHERE;
			break;

		case MODEL_TYPE::ROCK_03:
			src = RES_T::STAGE_ROCK_03;
			name = "rock03";
			radius = ROCK_03_COLI_RADIUS;
			hitType = HIT_TYPE::SPHERE;
			break;

		case MODEL_TYPE::SACK:
			src = RES_T::STAGE_SACK;
			name = "sack";
			radius = SACK_COLI_RADIUS;
			hitType = HIT_TYPE::SPHERE;
			break;

		case MODEL_TYPE::TABLE:
			src = RES_T::STAGE_TABLE;
			name = "table";
			radius = TABLE_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		case MODEL_TYPE::TREE_01:
			src = RES_T::STAGE_TREE_01;
			name = "tree01";
			radius = TREE_01_COLI_RADIUS;
			hitType = HIT_TYPE::SPHERE;
			break;

		case MODEL_TYPE::TREE_02:
			src = RES_T::STAGE_TREE_02;
			name = "tree02";
			radius = TREE_02_COLI_RADIUS;
			hitType = HIT_TYPE::SPHERE;
			break;

		case MODEL_TYPE::TREE_03:
			src = RES_T::STAGE_TREE_03;
			name = "tree03";
			radius = TREE_03_COLI_RADIUS;
			hitType = HIT_TYPE::SPHERE;
			break;

		case MODEL_TYPE::TREE_04:
			src = RES_T::STAGE_TREE_04;
			name = "tree04";
			radius = TREE_04_COLI_RADIUS;
			hitType = HIT_TYPE::SPHERE;
			break;

		case MODEL_TYPE::TREE_05:
			src = RES_T::STAGE_TREE_05;
			name = "tree05";
			radius = TREE_05_COLI_RADIUS;
			hitType = HIT_TYPE::SPHERE;
			break;

		case MODEL_TYPE::WAGON:
			src = RES_T::STAGE_WAGON;
			name = "wagon";
			radius = WAGON_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		case MODEL_TYPE::WELL:
			src = RES_T::STAGE_WELL;
			name = "well";
			radius = WELL_COLI_RADIUS;
			hitType = HIT_TYPE::SPHERE;
			break;

		case MODEL_TYPE::WOOD:
			src = RES_T::STAGE_WOOD;
			name = "wood";
			radius = WOOD_COLI_RADIUS;
			hitType = HIT_TYPE::MODEL;
			break;

		default:
			break;
		}

		//情報の設定
		mdlId_[i] = res.LoadModelDuplicate(src);
		hitTypes_.push_back(hitType);
		names_.push_back(name);
		radius_.push_back(radius);
	}
}

void StageManager::JsonLoad()
{
	// 外部ファイルの読み込み
	std::ifstream ifs(jsonFile_);
	json stageData;
	if (ifs)
	{
		ifs >> stageData;
	}
	else { return; }

	for (int i = 0; i < MODELS; i++)
	{
		MODEL_TYPE type = static_cast<MODEL_TYPE>(i);
		std::vector<StageObject*> models;
		const auto& model = stageData[names_[i].c_str()];
		for (const json& Data : model)
		{
			auto stageObj = new StageObject(Data, mdlId_[i]);
			objs_.emplace_back(std::move(stageObj));
			models.emplace_back(std::move(stageObj));
		}
		stageMdls_.emplace(type, models);
	}

	ifs.close();
}

std::vector<Transform> StageManager::GetTtans(MODEL_TYPE type)
{
	std::vector<Transform> ret;
	for (auto& obj : stageMdls_[type])
	{
		ret.push_back(obj->GetTrans());
	}
	return ret;
}

StageManager::HIT_TYPE StageManager::GetHitType(MODEL_TYPE type)
{
	return hitTypes_[static_cast<int>(type)];
}

float StageManager::GetRadius(MODEL_TYPE type)
{
	int i = static_cast<int>(type);
	return radius_[i].x;

}

VECTOR StageManager::GetHitCapsulePos(VECTOR pos, MODEL_TYPE type, Transform trans, LR lr)
{	
	int i = static_cast<int>(type);
	VECTOR ret = { 0,0,0 };
	float radius = radius_[i].z;

	if (hitTypes_[i] != HIT_TYPE::CAPSULE)
	{
		return ret;
	}switch (type)
	{
	case MODEL_TYPE::HOUSE_01:
	case MODEL_TYPE::HOUSE_02:
	case MODEL_TYPE::HOUSE_03:
	case MODEL_TYPE::HOUSE_04:
	case MODEL_TYPE::HOUSE_05:
		switch (lr)
		{
		case LR::L:
			ret = VAdd(pos, VScale(trans.quaRot.GetForward(), radius));
			break;

		case LR::R:
			ret = VAdd(pos, VScale(trans.quaRot.GetBack(), radius));
			break;
		}
		break;

	default:
		switch (lr)
		{
		case LR::L:
			ret = VAdd(pos, VScale(trans.quaRot.GetLeft(), radius));
			break;

		case LR::R:
			ret = VAdd(pos, VScale(trans.quaRot.GetRight(), radius));
			break;
		}
		break;
	}

	return ret;
}

void StageManager::DebugDraw()
{
	int divNum = 20;
	int color = 0xff0000;
	bool fill = false;
	VECTOR rPos;
	VECTOR lPos;


	for (int i = 0; i < MODELS; i++)
	{
		MODEL_TYPE type = static_cast<MODEL_TYPE>(i);
		std::vector<Transform> objs = GetTtans(type);

		//中身がない場合
		if (objs.size() <= 0)
		{
			continue;	//処理の終了
		}

		for (auto obj : objs)
		{	
			VECTOR pos = obj.pos;
			pos.y = 0.0f;
			switch (hitTypes_[i])
			{
			case HIT_TYPE::SPHERE:
				DrawSphere3D(pos, radius_[i].x, divNum, color, color, fill);
				break;

			case HIT_TYPE::CAPSULE:
				rPos = GetHitCapsulePos(pos, type, obj, LR::R);
				lPos = GetHitCapsulePos(pos, type, obj, LR::L);
				DrawCapsule3D(rPos, lPos, radius_[i].x, 10, 0xff0000, 0xff0000, false);
				break;

			default:
				break;
			}
		}
	}	
}