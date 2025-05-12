#include "../../Application.h"
#include "../Lib/nlohmann/json.hpp"
#include "CharacterParamData.h"

using json = nlohmann::json;
CharacterParamData* CharacterParamData::instance_ = nullptr;

void CharacterParamData::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new CharacterParamData();
	}
	instance_->Init();
}

CharacterParamData& CharacterParamData::GetInstance()
{
	return *instance_;
}

void CharacterParamData::Init()
{	
	json paramData;
	try {
		//JSONファイルを開く
		std::ifstream ifs((Application::PATH_JSON + "CharacterParamData.json").c_str());
	
		if (ifs) {
			paramData = json::parse(ifs, nullptr, false);
		}
		else
		{
			return;
		}						//開けない場合処理を終了
		ifs.close();
	}
	catch (const json::parse_error& e) {
		std::cerr << "JSON 解析エラー: " << e.what() << std::endl;
		std::cerr << "エラーの場所: " << e.byte << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "例外: " << e.what() << std::endl;
	}
	//名前の登録
	std::string names[TYPE_MAX];
	names[static_cast<int>(UNIT_TYPE::KNIGHT)] = "Knight";
	names[static_cast<int>(UNIT_TYPE::AXEMAN)] = "Axeman";
	names[static_cast<int>(UNIT_TYPE::ARCHER)] = "Archer";
	names[static_cast<int>(UNIT_TYPE::MAGE)] = "Mage";
	names[static_cast<int>(UNIT_TYPE::E_KNIGHT)] = "EnemyKnight";
	names[static_cast<int>(UNIT_TYPE::E_AXEMAN)] = "EnemyAxeman";
	names[static_cast<int>(UNIT_TYPE::E_ARCHER)] = "EnemyArcher";
	names[static_cast<int>(UNIT_TYPE::E_MAGE)] = "EnemyMage";
	names[static_cast<int>(UNIT_TYPE::BOSS)] = "EnemyBoss";
	names[static_cast<int>(UNIT_TYPE::CHICKEN)] = "Chicken";

	for (int i = 0; i < TYPE_MAX; i++)
	{
		//種類の設定
		UNIT_TYPE type = static_cast<UNIT_TYPE>(i);

		//取り出すデータを決める
		const auto& jsonArray = paramData[names[i]];

		// 配列の最初の要素を取得
		const auto& data = jsonArray[0];

		//情報の取得
		PARAM p;
		try {
			p.atk_ = data.at("attack").get<float>();
			p.def_ = data.at("defense").get<float>();
			p.speed_ = data.at("speed").get<float>();
			p.hp_ = data.at("hp").get<int>();
			p.radius_ = data.at("radius").get<float>();
			p.skills_[0] = data.at("skill1").get<int>();
			p.skills_[1] = data.at("skill2").get<int>();
			p.skills_[2] = data.at("skill3").get<int>();
			p.skills_[3] = data.at("skill4").get<int>();
			p.exp_ = data.at("exp").get<float>();
		}
		catch (const nlohmann::json::exception& e) {
			std::cerr << "JSON 解析エラー: " << e.what() << std::endl;
			continue;
		}

		//情報の格納
		charParam_.emplace(type, p);
	}
}

CharacterParamData::CharacterParamData()
{
}
