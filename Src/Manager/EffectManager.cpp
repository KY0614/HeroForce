#include "EffectManager.h"

void EffectManager::Add(const EFFECT& _efc, int _data)
{
	effectRes_.emplace(_efc, std::move(_data));
}
