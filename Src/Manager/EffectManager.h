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

	/// <summary>
	/// エフェクトの追加
	/// </summary>
	/// <param name="_efc">エフェクト種類名</param>
	/// <param name="_data">エフェクトのデータ</param>
	void Add(const EFFECT& _efc,int _data);

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="_efc">エフェクト種類名</param>
	/// <param name="_pos">再生位置</param>
	/// <param name="_qua">角度</param>
	/// <param name="_size">大きさ</param>
	void Play(const EFFECT& _efc, const VECTOR& _pos, const Quaternion& _qua, const float& _size);

	/// <summary>
	/// エフェクトの再生停止
	/// </summary>
	/// <param name="_efc">エフェクト種類名</param>
	void Stop(const EFFECT& _efc);

	//解放処理
	void Release(void);

private:
	std::unordered_map<EFFECT,int> effectRes_;
};

