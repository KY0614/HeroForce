#pragma once
#include<vector>
#include"../../Utility/AsoUtility.h"
#include "../UnitBase.h"

class Enemy : public UnitBase
{
public:

#define DEBUG_ENEMY

	//****************************************************************
	//定数(キャラ共通)
	//****************************************************************

	//アニメーション番号
	static constexpr int ANIM_IDLE = 14;	//待機アニメーション
	static constexpr int ANIM_WALK = 93;	//歩きアニメーション
	static constexpr int ANIM_RUN = 54;		//走りアニメーション
	static constexpr int ANIM_DAMAGE = 39;	//ダメージアニメーション
	static constexpr int ANIM_DEATH = 24;	//やられアニメーション
	static constexpr int ANIM_ENTRY = 74;	//出現アニメーション

	//****************************************************************
	//列挙型
	//****************************************************************

	//現在状態
	enum class STATE
	{
		NORMAL			//通常
		,ALERT			//攻撃前(警告)
		,ATTACK			//攻撃
		,BREAK			//休憩
		,MAX
	};

	//自分の種類のラベル分け　※作るかも！！

	//****************************************************************
	//メンバ関数
	//****************************************************************

	//コンストラクタ
	Enemy() = default;
	//デストラクタ
	~Enemy() = default;

	//解放
	void Destroy(void)override;

	//初期化
	void Init(void)override;
	//更新
	virtual void Update(void)override;
	//描画
	virtual void Draw(void)override;

	//警告時間中かどうかを返す(純粋仮想関数)
	virtual const bool IsAlertTime(void)const = 0;
	//休憩時間中かどうかを返す(純粋仮想関数)
	virtual const bool IsBreak(void)const = 0;

	//スタン中かどうかを返す
	const bool IsStun(void)const { return stunDef_ > stunDefMax_; }

	//現在のスキルの全配列を返す
	const std::vector<ATK> GetAtks(void)const { return nowSkill_; }

	//索敵範囲を返す
	const float GetSearchRange(void) { return searchRange_; }

	//攻撃開始範囲を返す
	const float GetAtkStartRange(void) { return atkStartRange_; }

	/// <summary>
	/// 移動状態を変更
	/// </summary>
	/// <param name="_isMove">移動するどうか(true:移動する)</param>
	void SetIsMove(const bool _isMove) { isMove_ = _isMove; }

	/// <summary>
	/// 標的の座標を変更
	/// </summary>
	/// <param name="_targetPos">標的の座標</param>
	void SetTargetPos(const VECTOR _targetPos) { targetPos_ = _targetPos; }

	//標的の方向に向く
	void LookTargetVec(void);

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="_damage">ダメージ量</param>
	/// <param name="_stunPow">スタン攻撃力</param>
	void Damage(const int _damage, const int _stunPow);

	//現在状態を返す
	const STATE GetState(void) { return state_; }

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="_state">遷移する状態</param>
	void ChangeState(const STATE _state);

protected:

	//****************************************************************
	//メンバ変数
	//****************************************************************

	STATE state_;	//現在の状態

	float alertCnt_;			//攻撃の警告時間カウンタ
	float breakCnt_;			//攻撃の休憩時間カウンタ

	float walkSpeed_;		//敵ごとの歩く速度
	float runSpeed_;		//敵ごとの走る速度

	std::vector<ATK> skills_;			//スキルの種類
	std::vector<ATK> nowSkill_;			//現在のスキル

	std::vector<ANIM> skillAnims_;		//スキルに対応したアニメーション
	ANIM nowSkillAnim_;					//現在のスキルアニメーション

	VECTOR localCenterPos_;	//敵中央の相対座標
	VECTOR colPos_;			//敵自身の当たり判定用の相対座標

	float moveSpeed_;		//移動量
	bool isMove_;			//移動しているかどうか(true:移動中)

	VECTOR targetPos_;		//標的の座標

	float searchRange_;		//索敵範囲
	float atkStartRange_;	//攻撃開始範囲

	int stunDefMax_;	//気絶防御値の最大値
	int stunDef_;		//気絶防御値

	//****************************************************************
	//メンバ関数
	//****************************************************************

	//キャラ固有設定
	virtual void SetParam(void) = 0;

	//アニメーション番号の初期化
	virtual void InitAnimNum(void);

	//スキルの初期化
	virtual void InitSkill(void) = 0;

	//敵の攻撃処理
	virtual void Attack(void) = 0;

	//スキル1
	virtual void Skill_One(void);

	//スキルのランダム生成
	virtual void RandSkill(void);

	//アニメーション終了時の動き
	virtual void FinishAnim(void)override;

	//状態遷移における初期化処理
	virtual void InitChangeState(void);

	//更新(通常)
	void UpdateNml(void);
	//更新(攻撃警告)
	void UpdateAlert(void);
	//更新(攻撃)
	void UpdateAtk(void);
	//更新(休憩)
	virtual void UpdateBreak(void);

	/// <summary>
	/// 標的までのベクトル速度を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>標的への方向ベクトル</returns>
	const VECTOR GetMovePow2Target(void)const;

	//移動
	void Move(void);
};

