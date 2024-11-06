#pragma once
#include<vector>
#include"../../Utility/AsoUtility.h"
#include "../UnitBase.h"

class Enemy : public UnitBase
{
public:

#define DEBUG_ENEMY

	//アニメーション番号
	static constexpr int ANIM_IDLE = 14;	//待機アニメーション
	static constexpr int ANIM_WALK = 93;	//歩きアニメーション
	static constexpr int ANIM_RUN = 54;		//走りアニメーション
	static constexpr int ANIM_SKILL_1 = 9;	//スキル1アニメーション※仮
	static constexpr int ANIM_SKILL_2 = 11;	//スキル2アニメーション※仮
	static constexpr int ANIM_DAMAGE = 39;	//ダメージアニメーション
	static constexpr int ANIM_DEATH = 24;	//やられアニメーション
	static constexpr int ANIM_ENTRY = 74;	//出現アニメーション

	//アニメーション速度
	static constexpr float DEFAULT_SPEED_ANIM = 20.0f;	//デフォルトのアニメーション速度

	//モデル関係
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,100.0f * CHARACTER_SCALE,0.0f };	//モデルの中心座標への相対座標
	
	//敵自身の当たり判定半径
	static constexpr float MY_COL_RADIUS = 100.0f * CHARACTER_SCALE;

	//攻撃関係
	static constexpr float ALERT_TIME = 120.0f;	//攻撃の警告時間
	static constexpr float BREAK_TIME = 100.0f;	//攻撃の休憩時間

	//速度関係
	static constexpr float WALK_SPEED = 2.0f;	//歩きの速度
	static constexpr float RUN_SPEED = 4.0f;	//走りの速度

	//範囲関係
	static constexpr float SEARCH_RANGE = 1000.0f * CHARACTER_SCALE;		//索敵判定の大きさ
	static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;		//攻撃開始判定の大きさ

	//スキルの当たり判定半径
	static constexpr float SKILL_1_COL_RADIUS = 10.0f;	//スキル１
	static constexpr float SKILL_2_COL_RADIUS = 24.0f;	//スキル２

	//スキル関係
	static constexpr ATK SKILL_1 = { AsoUtility::VECTOR_ZERO,SKILL_1_COL_RADIUS,1.0f,60.0f,120.0f,0.0f };	//スキル１
	static constexpr ATK SKILL_2 = { AsoUtility::VECTOR_ZERO,SKILL_2_COL_RADIUS,5.0f,180.0f,300.0f,0.0f };	//スキル２

	//現在状態
	enum class STATE
	{
		NORMAL			//通常
		,ALERT			//攻撃前(警告)
		,ATTACK			//攻撃
		,BREAK			//休憩
		,MAX
	};

	//コンストラクタ
	Enemy();
	//デストラクタ
	~Enemy() = default;

	//解放
	void Destroy(void)override;

	//初期化
	void Init(void)override;
	//更新
	void Update(void)override;
	//描画
	void Draw(void)override;

	//警告時間中かどうかを返す
	const bool IsAlertTime(void)const { return alertCnt_ < ALERT_TIME; }
	//休憩時間中かどうかを返す
	const bool IsBreak(void)const { return breakCnt_ < BREAK_TIME; }
	//スタン中かどうかを返す
	const bool IsStun(void)const { return stunDef_ > stunDefMax_; }

	//現在のスキルの全配列を返す
	const std::vector<ATK> GetAtks(void)const { return nowSkill_; }

	//取得経験値を返す
	const float GetExp(void)const { return exp_; }

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
	/// 標的の座標を取得
	/// </summary>
	/// <param name="_targetPos">標的の座標</param>
	void SetTargetPos(const VECTOR _targetPos) { targetPos_ = _targetPos; }

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

	//キャラ固有設定
	virtual void SetParam(void);

	//アニメーション番号の初期化
	virtual void InitAnimNum(void);

	//スキルの初期化
	virtual void InitSkill(void);

	//敵の攻撃処理
	virtual void Attack(void);

	//スキル1
	virtual void Skill_1(void);

	//スキル2
	virtual void Skill_2(void);

	//アニメーション終了時の動き
	void FinishAnim(void)override;

private:
	
	STATE state_;	//現在の状態

	float alertCnt_;			//攻撃の警告時間カウンタ
	float breakCnt_;			//攻撃の休憩時間カウンタ

	std::vector<ATK> skills_;			//スキルの種類
	std::vector<ATK> nowSkill_;			//現在のスキル
	
	std::vector<ANIM> skillAnims_;		//スキルに対応したアニメーション
	ANIM nowSkillAnim_;					//現在のスキルアニメーション
		
	VECTOR colPos_;			//敵自身の当たり判定用の相対座標

	float searchRange_;		//索敵範囲
	float atkStartRange_;	//攻撃開始範囲
	float moveSpeed_;		//移動量
	bool isMove_;			//移動しているかどうか(true:移動中)

	int stunDefMax_;	//気絶防御値の最大値
	int stunDef_;		//気絶防御値

	float exp_;		//取得経験値

	VECTOR targetPos_;	//※デバッグ　標的の座標

	//更新(通常)
	void UpdateNml(void);
	//更新(攻撃警告)
	void UpdateAlert(void);
	//更新(攻撃)
	void UpdateAtk(void);
	//更新(休憩)
	void UpdateBreak(void);

	/// <summary>
	/// 標的のベクトルを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>標的への方向ベクトル</returns>
	const VECTOR GetTargetVec(void)const;

	//移動
	void Move(void);

	//スキルのランダム生成
	void RandSkill(void);
};

