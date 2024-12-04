#pragma once
#include <vector>
#include <string>
#include "../Lib/nlohmann/json.hpp"
#include "../Common/Transform.h"
#include "StageManager.h"

// í∑Ç¢ÇÃÇ≈namespaceÇÃè»ó™
using json = nlohmann::json;

class StageObject
{
public:

	static constexpr float SCALE = 100.0f;

	StageObject(const json& data, int model);

	void Draw();
	void Release();

	Transform GetTrans() const;

private:

	Transform transform_;

};
