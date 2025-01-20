#pragma once
#include <vector>
#include <random>
#include <DxLib.h>
#include "../../Common/Transform.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/GameSystem/Collision.h"
#include "../../Stage/StageManager.h"
#include "../PlayerBase.h"
#include "ChickenBase.h"


class ChickenBase;

class ChickenManager
{
public:

	//チキン生成数
	static constexpr int CREATE_COUNT = 10;

	/// <summary>
	/// チキンマネージャー
	/// </summary>
	/// <param name="pos"></param>	チキンの生成位置を受け取る
	/// <param name="stageTrans"></param>	ステージのトランスフォーム
	/// <param name="playerTrans"></param>	プレイヤーのトランスフォーム
	ChickenManager(
		std::vector<VECTOR> pos,
		const Transform& stageTrans,
		const Transform& playerTrans);
	~ChickenManager();

	void Init();
	void Update();
	void Draw();

	//生成位置の情報をシャッフルする
	void ShufflePos();

	//プレイヤー位置を取得(1Pので良い)
	void SetTargetPos(const VECTOR pos);

private:

	//リスポーン位置用座標配列
	std::vector<VECTOR> pos_;

	//ターゲット位置
	VECTOR targetPos_;

	//プレイヤー用トランスフォーム
	const Transform &playerTrans_;
	const Transform &stageTrans_;

	//チキンインスタンス
	std::vector<std::shared_ptr<ChickenBase>> chickens_;
};

