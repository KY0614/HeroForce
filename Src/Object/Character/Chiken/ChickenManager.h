#pragma once
#include <vector>
#include <random>
#include <DxLib.h>
#include "../../Common/Transform.h"
#include "../../../Utility/AsoUtility.h"
#include "../PlayableChara/PlayerBase.h"
#include "ChickenBase.h"


class ChickenBase;

class ChickenManager
{
public:

	//チキン生成数
	static constexpr int CREATE_COUNT = 10;

	ChickenManager(std::vector<VECTOR> pos);	//生成位置をもらう
	~ChickenManager();

	void Init();
	void Update();
	void Draw();

	//生成位置の情報をシャッフルする
	void ShufflePos();

	//プレイヤー位置を取得(1Pので良い)
	void SetTargetPos(const VECTOR pos);

	//衝突判定
	void CollisionStage(const Transform& stageTrans, std::shared_ptr<ChickenBase> cheken);

	//生存総数取得
	const int GetAliveNum(void)const;

private:

	//リスポーン位置用座標配列
	std::vector<VECTOR> pos_;

	//ターゲット位置
	VECTOR targetPos_;

	//プレイヤー用トランスフォーム
	Transform playerTrans_;

	//チキンインスタンス
	std::vector<std::shared_ptr<ChickenBase>> chickens_;
};

