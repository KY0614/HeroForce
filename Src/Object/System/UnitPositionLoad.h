#pragma once
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <DxLib.h>

class UnitPositionLoad
{
public:

	enum class UNIT_TYPE
	{
		PLAYER,
		ENEMY,
		CPU,
		MAX
	};


	static constexpr float SCALE = 100.0f;
	static constexpr int TYPE_MAX = static_cast<int>(UNIT_TYPE::MAX);

	UnitPositionLoad();
	~UnitPositionLoad();

	void Init();

	//Jsonファイルの設定
	void SetJsonFile();

	//Json読み込み
	void JsonLoad();

	//ゲッター
	std::vector<VECTOR>GetPos(UNIT_TYPE type);
	std::vector<VECTOR>GetRot(UNIT_TYPE type);

protected:

	//JSONファイル
	std::string jsonFile_;

private:

	//座標
	std::map < UNIT_TYPE, std::vector<VECTOR>> pos_;

	//角度
	std::map < UNIT_TYPE, std::vector<VECTOR>> rot_;

};

