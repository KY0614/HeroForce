#pragma once
#include <DxLib.h>

class GamaUIBase
{
public:

	//HPバーの種類
	enum class HPBAR_TYPE
	{
		PLAYER,
		ENEMY,
		CPU,
		MAX
	};

	//HPバー種類量
	static constexpr int HP_BAR_MAX = static_cast<int>(HPBAR_TYPE::MAX);

	//HPバーサイズ
	static constexpr int HP_BAR_SIZE_X = 60;
	static constexpr int HP_BAR_SIZE_Y = 17;

	//HPゲージサイズ
	static constexpr int HP_GAGE_SIZE_X = 120;
	static constexpr int HP_GAGE_SIZE_Y = 45;

	//コンストラクタ
	GamaUIBase();

	//デストラクタ
	~GamaUIBase() = default;

	//初期化
	virtual void Init();

	//描画
	virtual void Draw();

	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="_pos"></param>座標
	inline void SetPos(const VECTOR _pos) { pos_ = _pos; }

	/// <summary>
	/// 体力設定
	/// </summary>
	/// <param name="_hp"></param>体力
	inline void SetHP(const int _hp) { hp_ = _hp; if (hp_ <= 0) { hp_ = 0; }}

protected:

	//画像
	int imgHpGage_;
	int imgHpBar_;

	//HPバー種類
	HPBAR_TYPE typeHpBar_;

	//座標
	VECTOR pos_;

	//HP
	int hp_;	
	
	//読み込み
	virtual void Load();

};

