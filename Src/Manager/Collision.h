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


	//攻撃が当たったか
	// 引数は（攻撃した人、攻撃対象）の順
	//プレイヤー
	const bool IsHitAtk(const PlayerBase& _player, const Enemy& _enemy);
	//敵
	const bool IsHitAtk(const Enemy& _enemy, const PlayerBase& _player);

	//索敵
	// 引数は（索敵した人、索敵対象）の順
	const bool Search(const UnitBase& _chaser, const UnitBase& _target);

private:
	//コンストラクタ＆デストラクタ
	Collision(void) = default;
	~Collision(void) = default;


};

