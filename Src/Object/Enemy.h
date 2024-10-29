#pragma once
#include<vector>
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"

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

	//モデルの中心座標への相対座標
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,20.0f,0.0f };

	//攻撃関係
	static constexpr float ALERT_TIME = 120.0f;	//攻撃の警告時間
	static constexpr float BREAK_TIME = 100.0f;	//攻撃の休憩時間

	//速度関係
	static constexpr float WALK_SPEED = 2.0f;	//歩きの速度
	static constexpr float RUN_SPEED = 4.0f;	//走りの速度

	//範囲関係
	static constexpr float SEARCH_RANGE = 120.0f;	//索敵判定の大きさ
	static constexpr float ATK_START_RANGE = 50.0f;	//攻撃開始判定の大きさ

	//スキル関係
	static constexpr ATK SKILL_1 = { AsoUtility::VECTOR_ZERO,1.0f,60.0f,120.0f,0.0f };	//スキル１
	static constexpr ATK SKILL_2 = { AsoUtility::VECTOR_ZERO,5.0f,180.0f,300.0f,0.0f };	//スキル２

	//スキルの当たり判定半径
	static constexpr float SKILL_1_COL_RADIUS = 10.0f;	//スキル１
	static constexpr float SKILL_2_COL_RADIUS = 24.0f;	//スキル２

	//敵自身の当たり判定半径
	static constexpr float MY_COL_RADIUS = 20.0f;

	//現在状態
	enum class STATE
	{
		NORMAL			//通常
		,ALERT			//攻撃前(警告)
		,ATTACK			//攻撃
		,BREAK			//休憩
		,MAX
	};

	//攻撃の種類
	//enum class ATK_PAT
	//{

	//};

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

	/// <summary>
	/// 目的の範囲に対象が入ったかを返す
	/// </summary>
	/// <param name="_myPos">自身の座標</param>
	/// <param name="_targetPos">対象の座標</param>
	/// <param name="_rangeRadius">範囲用の半径</param>
	/// <returns></returns>
	const bool Search(VECTOR _myPos, VECTOR _targetPos, float _rangeRadius)const;

	//敵自身の当たり判定座標を返す
	//const VECTOR GetColPos(void)const { return colPos_; }

	//現在のスキルの座標を返す
	//const VECTOR GetNowSkillPos(void)const { return nowSkill_.pos_; }

	//現在のスキルの当たり判定半径を返す
	const float	GetNowSkillColRadius(void)const { /*TODO*/ /*return nowSkillColRadius_;*/ }

	//敵自身の当たり判定半径を返す
	const float	GetMyColRadius(void)const { return colRadius_; }

	//取得経験値を返す
	const float GetExp(void)const { return exp_; }

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="_damage">ダメージ量</param>
	/// <param name="_stunPow">スタン攻撃力</param>
	void Damage(const int _damage, const int _stunPow);

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="_state">遷移する状態</param>
	void ChangeState(const STATE _state) { state_ = _state; }

protected:

	//キャラ固有設定
	virtual void SetParam(void);

	//アニメーション番号の初期化
	virtual void InitAnimNum(void);

	//スキルの初期化
	virtual void InitSkill(void);

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
	
	std::vector<float>skillColRadius_;		//スキルの当たり判定半径
	std::vector<float> nowSkillColRadius_;	//現在のスキルの当たり判定半径
	
	VECTOR colPos_;		//敵自身の当たり判定座標
	float colRadius_;	//敵自身の当たり判定半径

	float searchRange_;		//索敵範囲
	float atkStartRange_;	//攻撃開始範囲
	float moveSpeed_;		//移動量

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

	//敵の攻撃処理
	void Attack(void);
	//スキルのランダム生成
	void RandSkill(void);
};

