#pragma once
#include <vector>
#include <string>
#include "../Lib/nlohmann/json.hpp"
#include "../Common/Transform.h"
#include "StageBase.h"

// ’·‚¢‚Ì‚Ånamespace‚ÌÈ—ª
using json = nlohmann::json;

class StageObject
{
public:
	StageObject(const json& data, int model);

	void Draw();
	void Release();

	void SetRadius(const float radius);
	float GetRadius() const;
	Transform GetTrans() const;

private:

	Transform transform_;
	float radius_;

};
