#pragma once
#include <vector>
#include <string>
#include "../Lib/nlohmann/json.hpp"
#include "../Common/Transform.h"
#include "StageBase.h"

// �����̂�namespace�̏ȗ�
using json = nlohmann::json;

class StageObject
{
public:
	StageObject(const json& data, int model);

	void Draw();
	void Release();

	Transform GetTrans() const;


private:

	Transform transform_;


};
