#pragma once
#include<DxLib.h>

class DataBank
{
	//cppやSceneをまたぎ扱う情報等を保存する
public:

	static constexpr int PLAYER_NUM = 4;

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static DataBank& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// リソースの破棄
	void Destroy(void);
private:
	
	// 静的インスタンス
	static DataBank* instance_;

	//コンストラクタ及びデストラクタ
	DataBank() = default;
	~DataBank() = default;
};

