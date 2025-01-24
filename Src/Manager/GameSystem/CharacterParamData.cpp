#include "../../Lib/nlohmann/json.hpp"
#include "CharacterParamData.h"

using json = nlohmann::json;
CharacterParamData* CharacterParamData::instance_ = nullptr;

void CharacterParamData::CreateInstance(void)
{
	if (instance_ == nullptr) {
		instance_ = new CharacterParamData();
	}
	instance_->Init();
}

CharacterParamData& CharacterParamData::GetInstance(void)
{
	return *instance_;
}

void CharacterParamData::Init(void)
{
}

CharacterParamData::CharacterParamData()
{
}
