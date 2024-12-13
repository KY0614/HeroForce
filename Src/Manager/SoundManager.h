#pragma once
class SoundManager
{
public:
	enum class TYPE {
		NONE,
		BGM,
		SE
	};

	enum class SOUND {
		//ここに使用する音楽や効果音などを羅列
	};

	/// <summary>
	/// サウンドの追加
	/// </summary>
	/// <param name="_type">種類</param>
	/// <param name="_data">データ</param>
	void Add(const SOUND _type, const int _data);

	
private:

};

