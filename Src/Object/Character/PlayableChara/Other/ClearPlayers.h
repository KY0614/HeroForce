#pragma once
#include "OtherScenePlayerBase.h"
#include "../../../UnitBase.h"

class ClearPlayers : public OtherScenePlayerBase
{
public:

	//アニメーションスピード
	static constexpr float ANIM_SPEED = 20.0f;

	//アイドルアニメ番号
	static constexpr int IDLE_ANIM = 61;

	//コンストラクタ
	ClearPlayers();

	//デストラクタ
	~ClearPlayers() = default;

private:

	void InitAnim() override;

};