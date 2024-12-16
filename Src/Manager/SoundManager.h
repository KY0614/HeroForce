#pragma once
#include<unordered_map>

class SoundManager
{
public:
	//再生の種類分けするときに使う
	enum class TYPE {
		NONE,
		BGM,
		SE
	};

	enum class SOUND {
		//ここに使用する音楽や効果音などを羅列
		NONE,
	};

	struct SOUND_DATA
	{
		int data;		//音声データ格納
		TYPE type;		//音声データの種類	※この要素は削除するかも
		int playMode;	//音声データの再生タイプ
	};

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SoundManager& GetInstance(void);

	/// <summary>
	/// サウンドの追加
	/// </summary>
	/// <param name="_type">音の種類分け(SEかBGMか)</param>
	/// <param name="_sound">具体的な用途</param>
	/// <param name="_data">音のデータ</param>
	void Add(const TYPE _type, const SOUND _sound, const int _data);

	void Play(const SOUND _sound);

	void Stop(const SOUND _sound);

	void Release(void);
private:
	//インスタンス用
	static SoundManager* instance_;

	//データ格納用
	std::unordered_map<SOUND, SOUND_DATA>sounds_;

	//コンストラクタ＆デストラクタ
	SoundManager() = default;
	~SoundManager() = default;
};

