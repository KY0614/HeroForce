#include "../../Application.h"
#include "../../Lib/nlohmann/json.hpp"
#include "UnitPositionLoad.h"

// 長いのでnamespaceの省略
using json = nlohmann::json;

UnitPositionLoad::UnitPositionLoad()
{
}

UnitPositionLoad::~UnitPositionLoad()
{
}

void UnitPositionLoad::Init()
{
	//JSONファイル設定
	SetJsonFile();
	
	//JSON読み込み
	JsonLoad();
}

const std::vector<VECTOR> UnitPositionLoad::GetPos(const UNIT_TYPE _type)
{
	return pos_[_type];
}

const std::vector<VECTOR> UnitPositionLoad::GetRot(const UNIT_TYPE _type)
{
	return rot_[_type];
}

void UnitPositionLoad::SetJsonFile()
{
	//JSONファイルネーム
	jsonFile_ = Application::PATH_JSON + "CharacterPosData.json";
}

void UnitPositionLoad::JsonLoad()
{
	//JSONファイルを開く
	std::ifstream ifs(jsonFile_);
	json posData;
	if (ifs) { ifs >> posData; }	//開けたら情報を取得
	else 
	{ 
		return; 
	}						//開けない場合処理を終了

	//名前の登録
	std::string names[TYPE_MAX];
	names[static_cast<int>(UNIT_TYPE::PLAYER)] = "player";
	names[static_cast<int>(UNIT_TYPE::ENEMY)] = "enemy";
	names[static_cast<int>(UNIT_TYPE::CPU)] = "cpu";

	for (int i = 0; i< TYPE_MAX; i++)
	{
		//種類の設定
		UNIT_TYPE type = static_cast<UNIT_TYPE>(i);

		//取り出すデータを決める
		const auto& datas = posData[names[i]];

		//json格納用の変数
		std::vector<VECTOR> pos;	//座標
		std::vector<VECTOR> rot;	//角度

		for (const json& data : datas)
		{
			//座標の取得
			VECTOR ret = {
				data["pos"]["x"] * SCALE,
				data["pos"]["y"] * SCALE + DOWN_POS_Y,
				data["pos"]["z"] * SCALE };
			pos.push_back(ret);

			//角度の受け取り
			ret = {
				data["rot"]["x"],
				data["rot"]["y"],
				data["rot"]["z"] };
			rot.push_back(ret);
		}
		pos_.emplace(type, pos);
		pos_.emplace(type, rot);
	}
	ifs.close();
}