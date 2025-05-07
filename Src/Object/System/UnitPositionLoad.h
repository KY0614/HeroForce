#pragma once
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <DxLib.h>

class UnitPositionLoad
{
public:

	//ユニット種類
	enum class UNIT_TYPE
	{
		PLAYER,
		ENEMY,
		CPU,
		MAX
	};

	//スケール
	static constexpr float SCALE = 100.0f;

	//読み込んだ位置のY値を少し下げる
	static constexpr float DOWN_POS_Y = -SCALE * 1.5f;

	//種類数
	static constexpr int TYPE_MAX = static_cast<int>(UNIT_TYPE::MAX);

	//コンストラクタ
	UnitPositionLoad();

	//デストラクタ
	~UnitPositionLoad();

	//初期化
	void Init();

	/// <summary>
	/// 指定したユニットの座標配列を返す
	/// </summary>
	/// <param name="_type"></param>ユニットの種類
	/// <returns></returns>指定したユニットの座標配列
	const std::vector<VECTOR>GetPos(const UNIT_TYPE _type);

	/// <summary>
	/// 指定したユニットの角度配列を返す
	/// </summary>
	/// <param name="_type"></param>ユニットの種類
	/// <returns></returns>指定したユニットの角度配列
	const std::vector<VECTOR>GetRot(const UNIT_TYPE _type);

private:	
	
	//JSONファイル
	std::string jsonFile_;

	//座標
	std::map < UNIT_TYPE, std::vector<VECTOR>> pos_;

	//角度
	std::map < UNIT_TYPE, std::vector<VECTOR>> rot_;	
	
	//JSONファイルの設定
	void SetJsonFile();

	//Json読み込み
	void JsonLoad();

};

