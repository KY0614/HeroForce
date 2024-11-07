#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"


class SelectScene :public SceneBase
{
public:

	//選択している種類
	enum class SELECT 
	{
		NUMBER,		//人数
		ROLE		//役職
	};

	enum class CONTROL
	{
		KEY,
		PAD
	};

	// キーコンフィグ
	enum class  KEY_CONFIG
	{
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		DECIDE
	};

	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	//人数選択中の処理
	void NumberUpdate(void);

	//役職選択中の処理
	void RoleUpdate(void);

	//デバッグ描画
	void DrawDebug(void);

	//選択するものの種類を変える
	void ChangeSelect(SELECT select);

	void ProcessSelect(void);

	void KeyKonfigSetting(void);

	void Move(VECTOR pos);

private:

	SELECT select_;

	CONTROL ctrl_;

	KEY_CONFIG key;

	VECTOR pos_;
};

