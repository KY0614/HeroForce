#pragma once
#include <DxLib.h>

class GamaUIBase
{
public:

	enum HPBAR_TYPE
	{
		PLAYER,
		ENEMY,
		CPU,
		MAX
	};

	//HPバー種類量
	static constexpr int HP_BAR_MAX = static_cast<int>(HPBAR_TYPE::MAX);

	GamaUIBase();
	~GamaUIBase();

	void Init();
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

