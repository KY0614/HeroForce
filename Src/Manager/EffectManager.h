#pragma once
#include<DxLib.h>
#include"../Common/Quaternion.h"
#include<unordered_map>
#include<string>
class EffectManager
{
public:
	enum class EFFECT
	{
		//使用するエフェクトを羅列
	};

	void Add(const EFFECT& _efc,int _data);
	void Play(const EFFECT& _efc, const VECTOR& _pos, const Quaternion& _qua, const float& _size);
	void Stop(const EFFECT& _efc);
	void Release(void);

private:
	std::unordered_map<EFFECT,int> effectRes_;
};

