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

	//HP�o�[��ޗ�
	static constexpr int HP_BAR_MAX = static_cast<int>(HPBAR_TYPE::MAX);

	GamaUIBase();
	~GamaUIBase();

	void Init();
	virtual void Update();
	virtual void Draw();

	//�����ݒ�
	virtual void SetParam();

	//�ǂݍ���
	virtual void Load();

	//���W�ݒ�
	void SetPos(const VECTOR pos);

	//HP�ݒ�
	void SetHP(const int hp);

protected:

	//�摜
	int imgHpGage_;
	int imgHpBar_;

	//HP�o�[���
	HPBAR_TYPE typeHpBar_;

	//���W
	VECTOR pos_;

	//HP
	int hp_;

};

