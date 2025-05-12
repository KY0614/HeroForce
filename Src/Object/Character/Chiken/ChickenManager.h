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

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos"></param>配置座標配列
	/// <param name="stageTrans"></param>ステージの情報
	/// <param name="playerTrans"></param>プレイヤー情報(1pのみ)
	ChickenManager(
		std::vector<VECTOR> pos,
		const Transform& stageTrans,
		const Transform& playerTrans);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ChickenManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーの座標を受け取る
	/// </summary>
	/// <param name="pos"></param>1Pのプレイヤー座標
	//void SetTargetPos(const VECTOR pos);

	/// <summary>
	/// チキン生存数
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>生存数
	const int GetAliveNum()const;

	/// <summary>
	/// チキンを返す
	/// </summary>
	/// <param name="_num"></param>チキンの番号
	/// <returns></returns>指定した番号のチキン
	std::shared_ptr<ChickenBase> GetChicken(const int _num);

	/// <summary>
	/// チキン配列の要素数を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>配列の要素数
	const int GetChickenAllNum()const;

private:

	//リスポーン位置用座標配列
	std::vector<VECTOR> pos_;

	//ターゲット位置
	VECTOR targetPos_;

	//プレイヤー用トランスフォーム
	const Transform& playerTrans_;

	//ステージ用トランスフォーム
	const Transform& stageTrans_;

	//チキンインスタンス
	std::vector<std::shared_ptr<ChickenBase>> chickens_;	
	
	//生成位置の情報をシャッフルする
	void ShufflePos();	
	
	//衝突判定
	void CollisionStage(const Transform& stageTrans, std::shared_ptr<ChickenBase> cheken);
};

