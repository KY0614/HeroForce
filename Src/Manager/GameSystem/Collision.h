#pragma once

class UnitBase;
class PlayerBase;
class Enemy;

class Collision
{
public:
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static Collision& GetInstance(void);


	void Init(void);
	/// <summary>
	/// 攻撃が当たったか
	/// </summary>
	/// <param name="_chaser">攻撃をする人</param>
	/// <param name="_target">攻撃を受ける人</param>
	/// <returns></returns>
	const bool IsHitAtk(const UnitBase& _chaser, const UnitBase& _target);

	//索敵
	// 引数は（索敵した人、索敵対象）の順
	const bool Search(const UnitBase& _chaser, const UnitBase& _target);

	/// <summary>
	/// 目的の範囲に対象が入ったかを返す
	/// </summary>i5
	/// <param name="_myPos">自身の座標</param>
	/// <param name="_targetPos">対象の座標</param>
	/// <param name="_rangeRadius">範囲用の半径</param>
	/// <returns></returns>
	const bool Search(const VECTOR _myPos, const VECTOR _targetPos, const float _rangeRadius)const;

	/// <summary>
	/// ステージモデルとキャラクターの衝突判定
	/// </summary>
	/// <param name="_modelId"></param>ステージモデル
	/// <param name="_pos"></param>キャラクターの座標
	/// <param name="_radius"></param>キャラクターの半径
	/// <returns></returns>
	const bool IsHitUnitStageObject(const int& _modelId, const VECTOR& _pos, const float& _radius)const;

private:
	//コンストラクタ＆デストラクタ
	Collision(void) = default;
	~Collision(void) = default;
	//インスタンス用
	static Collision* instance_;
};

