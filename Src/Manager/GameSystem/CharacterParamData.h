#pragma once
class CharacterParamData
{
public:

	//インスタンスの生成
	static void CreateInstance(void);

	//インスタンスの取得
	static CharacterParamData& GetInstance(void);

	//初期化
	void Init();

private:

	//静的インスタンス
	static CharacterParamData* instance_;

	CharacterParamData();
	~CharacterParamData() = default;

};

