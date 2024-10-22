#pragma once
#include<vector>
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"

class Enemy : public UnitBase
{
public:

	//アニメーション番号
	static constexpr int ANIM_IDLE = 14;	//待機アニメーション
	static constexpr int ANIM_WALK = 93;	//歩きアニメーション
	static constexpr int ANIM_RUN = 54;		//走りアニメーション
	static constexpr int ANIM_DAMAGE = 39;	//ダメージアニメーション
	static constexpr int ANIM_DEATH = 71;	//やられアニメーション
	static constexpr int ANIM_ENTRY = 74;	//出現アニメーション

	static constexpr float ATK_CDT = 60.0f;		//攻撃のクールダウンの時間
	static constexpr float ATK_BTC = 100.0f;	//攻撃の休憩時間

	static constexpr float WALK_SPEED = 2.0f;	//歩きの速度
	static constexpr float RUN_SPEED = 4.0f;	//走りの速度

	static constexpr ATK SKILL_1 = { AsoUtility::VECTOR_ZERO,1.0f,60.0f,120.0f,0.0f };	//スキル１
	static constexpr ATK SKILL_2 = { AsoUtility::VECTOR_ZERO,5.0f,180.0f,300.0f,0.0f };	//スキル２

	//現在状態
	enum class STATE
	{
		NORMAL			//通常
		,ALERT_ATTACK	//攻撃前(警告)
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

	//位置
	const VECTOR GetPos(void)const override { return trans_.pos; }
	//角度
	const VECTOR GetRot(void)const override { return trans_.rot; }
	//大きさ
	const VECTOR GetScl(void)const override { return trans_.scl; }
	//防御力
	const float GetDef(void)const override { return def_; }

	//攻撃クールダウン中かどうかを返す(true:攻撃クールダウン中)
	const bool IsAtkCD(void)const { return atkCdt_ < ATK_CDT; }

	const bool IsStun(void)const { return stunDef_ > stunDefMax_; }

	//取得経験値を返す
	const float GetExp(void) { return exp_; }

	/// <summary>
	/// ダメージ
	/// </summary>
	/// <param name="_damage">ダメージ量</param>
	void Damage(const int _damage);

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="_state">遷移する状態</param>
	void ChangeState(const STATE _state) { state_ = _state; }

protected:

	//キャラ固有設定
	virtual void SetParam(void);

	//スキルの初期化
	virtual void InitSkill(void);

private:
	
	STATE state_;	//現在の状態

	float atkCdt_;				//攻撃のクールダウンカウンタ
	float atkBtc_;				//攻撃の休憩時間カウンタ

	std::vector<ATK> skill_;	//スキルの種類
	ATK nowSkill_;				//現在のスキル

	float moveSpeed_;	//移動量

	int stunDefMax_;	//気絶防御値の最大値
	int stunDef_;		//気絶防御値

	float exp_;		//取得経験値

	//アニメーション番号の初期化
	void InitAnimNum(void);

	//更新(通常)
	void UpdateNml(void);
	//更新(攻撃警告)
	void UpdateAlert(void);
	//更新(攻撃)
	void UpdateAtk(void);
	//更新(休憩)
	void UpdateBreak(void);

	//索敵
	void Search(void);

	//移動
	void Move(void);
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="_moveSpeed">移動距離</param>
	/// <param name="_deg">移動方向(デグリー角)</param>
	void ProcessMove(const float _moveSpeed, const float _deg);
	/// <summary>
	/// 方向転換
	/// </summary>
	/// <param name="_deg">回転方向(デグリー角)</param>
	/// <param name="_axis">回転軸</param>
	void Turn(const float _deg, const VECTOR _axis);

	//敵の攻撃処理
	void Attack(void);
	//スキルのランダム生成
	void RandSkill(void);

};

