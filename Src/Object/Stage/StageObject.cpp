#include <DxLib.h>
#include <memory>
#include <fstream>
#include "../../Utility/AsoUtility.h"
#include "../../Manager/Generic/ResourceManager.h"

#include "StageObject.h"

StageObject::StageObject(const json& data, int model)
{
	// モデル制御の基本情報
	transform_.SetModel(MV1DuplicateModel(
		model));

	int i = SCALE;

	transform_.scl = {
		data["scl"]["x"] / static_cast<float>(i) * static_cast<float>(i),
		data["scl"]["y"] / static_cast<float>(i) * static_cast<float>(i),
		data["scl"]["z"] / static_cast<float>(i) * static_cast<float>(i)
	};

	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.pos = { 
		data["pos"]["x"]* static_cast<float>(i) ,
		data["pos"]["y"] * static_cast<float>(i) - i,
		data["pos"]["z"] * static_cast<float>(i)
	};

	VECTOR rot = AsoUtility::VECTOR_ZERO;
	rot.x = data["rot"]["x"];
	rot.y = data["rot"]["y"];
	rot.z = data["rot"]["z"];

	transform_.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(rot.x),
		AsoUtility::Deg2RadF(rot.y),
		AsoUtility::Deg2RadF(rot.z)	
	);

	// ローカル回転
	transform_.quaRotLocal = Quaternion();
	transform_.quaRotLocal = Quaternion::AngleAxis(
		AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);

	transform_.Update();
}

void StageObject::Draw()
{
	MV1DrawModel(transform_.modelId);
}

void StageObject::Release()
{
	MV1DeleteModel(transform_.modelId);
}

Transform StageObject::GetTrans() const
{
	return transform_;
}



