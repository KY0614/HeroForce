#pragma once
#include <DxLib.h>

class GamaUIBase
{
public:

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

	GamaUIBase();
	~GamaUIBase();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	//初期設定
	virtual void SetParam();

	//読み込み
	virtual void Load();

	//座標設定
	void SetPos(const VECTOR pos);

	//HP設定
	void SetHP(const int hp);

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

};

