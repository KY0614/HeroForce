#pragma once
#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>

class CharacterParamData
{
public:

	//キャラクター種類
	enum class UNIT_TYPE
	{
		KNIGHT,
		AXEMAN,
		MAGE,
		ARCHER,
		E_KNIGHT,
		E_AXEMAN,
		E_MAGE,
		E_ARCHER,
		BOSS,
		CHICKEN,
		MAX
	};
	
	//読み込みスキル数
	static constexpr int TYPE_MAX = static_cast<int>(UNIT_TYPE::MAX);
	static constexpr int SKILLS = 4;

	//パラメーター要素
	struct PARAM
	{
		float atk_;
		float def_;
		float speed_;
		int hp_;
		int skills_[SKILLS];
		float radius_;
		float exp_;
	};

	//インスタンスの生成
	static void CreateInstance();

	/// <summary>
	/// インスタンスを返す
	/// </summary>
	/// <returns></returns>インスタンス
	static CharacterParamData& GetInstance();

	//初期化
	void Init();

	/// <summary>
	/// 情報の受け取り
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	inline const PARAM GetParamData(const UNIT_TYPE type)const { return charParam_.at(type); }

private:

	//静的インスタンス
	static CharacterParamData* instance_;

	//受け取り用各種変数
	std::map < UNIT_TYPE, PARAM> charParam_;

	CharacterParamData();
	~CharacterParamData() = default;
};

