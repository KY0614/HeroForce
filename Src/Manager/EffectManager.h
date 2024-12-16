#pragma once
#include<DxLib.h>
#include"SoundManager.h"
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

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static EffectManager& GetInstance(void);

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
	/// <param name="_sound">効果音</param>
	void Play(const EFFECT& _efc,
		const VECTOR& _pos, const Quaternion& _qua, const float& _size,
		const SoundManager::SOUND _sound = SoundManager::SOUND::NONE);

	/// <summary>
	/// エフェクトの再生停止
	/// </summary>
	/// <param name="_efc">エフェクト種類名</param>
	void Stop(const EFFECT& _efc);

	//解放処理
	void Release(void);

private:
	//インスタンス用
	static EffectManager* instance_;

	//エフェクトデータ格納用
	std::unordered_map<EFFECT,int> effectRes_;	//初期データ
	std::unordered_map<EFFECT,int> effectPlay_;	//再生データ

	//コンストラクタ＆デストラクタ
	EffectManager() = default;
	~EffectManager() = default;
};

