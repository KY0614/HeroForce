#pragma once
class SoundManager
{
public:
	//何かしら種類分けするときに使うかも？
	//必要なさそうなら消していい
	enum class TYPE {
		NONE,
		BGM,
		SE
	};

	enum class SOUND {
		//ここに使用する音楽や効果音などを羅列
	};


	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SoundManager& GetInstance(void);

	/// <summary>
	/// サウンドの追加
	/// </summary>
	/// <param name="_type">種類</param>
	/// <param name="_data">データ</param>
	void Add(const SOUND _type, const int _data);

	void Play(const SOUND _type);

	void Stop(const SOUND _type);

	void Release(void);
private:
	//インスタンス用
	static SoundManager* instance_;

	//コンストラクタ＆デストラクタ
	SoundManager() = default;
	~SoundManager() = default;
};

