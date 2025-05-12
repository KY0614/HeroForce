#pragma once
#include "../../../UnitBase.h"
#include "OtherScenePlayerBase.h"

class OverPlayers : public OtherScenePlayerBase
{
public:

	//アニメーションスピード
	static constexpr float ANIM_SPEED = 20.0f;

	//アイドルアニメ番号
	static constexpr int IDLE_ANIM = 24;
	static constexpr int DEATH_ANIM = 23;

	//コンストラクタ
	OverPlayers();

	//デストラクタ
	~OverPlayers() = default;

private:

	//アニメーション初期化
	void InitAnim()override;

	//アニメーションの終了
	void FinishAnim()override;
};

