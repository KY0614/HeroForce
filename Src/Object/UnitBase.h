﻿#pragma once
#include"Common/Transform.h"
#include<unordered_map>
#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/GameSystem/Collision.h"

//test

class UnitBase
{
public:

	//攻撃関係
	struct ATK
	{
		VECTOR pos_;		//位置
		float radius_;		//当たり判定の半径
		float pow_;			//技威力
		float duration_;	//持続時間（攻撃がどれくらい続くかを記述)
		float backlash_;	//後隙（後隙がどれくらい続くかを記述)
		float cnt_;			//カウンター
		bool isHit_;

	//攻撃中かどうか
		const bool IsAttack(void)const { return 0 < cnt_ && cnt_ <= duration_; };
		//後隙がどうか
		const bool IsBacklash(void)const { return 0 < (cnt_ - duration_) && (cnt_ - duration_) <= backlash_; };
		//モーションが終了しているか
		const bool IsFinishMotion(void)const { return cnt_ > (duration_ + backlash_) && cnt_ > 0; };
		//カウンターリセット
		void ResetCnt(void) { cnt_ = 0; };
		//攻撃判定が終了状態であるかを返却(true=当てた後)
		const bool IsHit(void) { return isHit_; }
	};

	enum class ANIM
	{
		NONE,		//初期化用
		IDLE,		//待機
		WALK,		//歩き
		RUN,		//走り
		SKILL_1,	//スキル①
		SKILL_2,	//スキル②
		SKILL_3,	//スキル➂
		SKILL_4,	//スキル④
		SKILL_5,	//スキル➄
		DODGE,		//回避
		DAMAGE,		//被ダメ
		DEATH,		//死亡
		ENTRY,		//出現
		UNIQUE_1,	//固有のやつ①
		UNIQUE_2,	//固有のやつ②
		UNIQUE_3,	//固有のやつ③
		UNIQUE_4,	//固有のやつ④
		UNIQUE_5,	//固有のやつ⑤
	};

	//定数
	// アニメーションの再生速度(仮）
	static constexpr float SPEED_ANIM = 20.0f;
	//キャラクター大きさ設定
	static constexpr float CHARACTER_SCALE = 0.3f;
	//初期強化パーセント
	static constexpr float DEFAULT_PERCENT = 100.0f;


	//コンストラクタ
	UnitBase(void);
	//デストラクタ
	~UnitBase(void);
	//解放
	virtual void Destroy(void);

	//基本処理の４つは仮想関数化するのでしっかりオーバーライドするように
	//初期化
	virtual void Init(void);
	//更新
	virtual void Update(void);
	//描画
	virtual void Draw(void);

	//ゲッター各種
	//生存確認(生存しているとtrue)
	const bool IsAlive(void)const { return hp_ > 0; };

	//Transformいただき
	const Transform& GetTransform(void)const;

	const Transform* GetTransformEntity(void)const;

	//位置
	const VECTOR GetPos(void)const;
	//角度
	const VECTOR GetRot(void)const;
	//大きさ
	const VECTOR GetScl(void)const;
	//防御力
	const float GetDef(void)const;
	//攻撃関係
	const ATK GetAtk(void)const;
	const float GetCharaPow(void)const;

	//あたり判定
	const float GetRadius(void)const;
	//移動前の座標位置
	const VECTOR GetPrePos()const;

	//アニメーション関数
	void Anim(void);
	//アニメーションリセット
	void ResetAnim(const ANIM _anim, const float _speed);

	//配列用アニメーション関数
	void AnimArray(void);
	//アニメーションリセット
	void ResetAnimArray(const ANIM _anim, const float _speed);
	float GetAnimArrayTime(void);

	//攻撃関係
	//isHit設定用（外部）
	void SetIsHit(const bool _flag);

	/// <summary>
	/// ダメージ設定
	/// </summary>
	/// <param name="attackerPower"></param>攻撃者の攻撃力
	/// <param name="skillPower"></param>当てた技の技威力
	void SetDamage(const int attackerPower, const int skillPower);

	//残量HPの処理(少しずつHpを減らす)
	void SubHp();

	//位置の設定
	void SetPos(const VECTOR pos);
	void SetPrePos(void);

	//強化反映
	void SetAttack(const float percent);
	void SetDefense(const float percent);
	void SetSpeed(const float percent);
	void SetHpMax(const float hp);

	//パラメータ読み込み
	void ParamLoad();

protected:

	//各キャラクターのデフォルトのパラメーター
	float defAtk_;
	float defDef_;
	float defSpeed_;
	int defHp_;

	//各パラメータごとの強化％
	float atkUpPercent_;
	float defUpPercent_;
	float speedUpPercent_;

	int hp_;			//体力
	int hpMax_;			//体力最大値
	int damage_;		//ダメージ
	Transform trans_;	//位置情報関係
	Transform transArray_[4];	//位置情報関係
	float radius_;		//自身の当たり判定の半径
	float def_;			//防御力
	float atkPow_;		//攻撃力
	ATK atk_;			//現在のスキル
	VECTOR prePos_;		//移動前の座標位置
	float moveSpeed_;   //移動スピード

	//アニメ関係
	ANIM anim_;								//アニメステート
	std::unordered_map<ANIM, int> animNum_;	//アニメーションナンバー格納配列。
	int atcAnim_;							//アタッチするアニメを格納
	int animTotalTime_;						//アニメーションの総再生時間
	float stepAnim_;						//アニメーションの再生時間
	float speedAnim_;						//アニメーション速度

	//配列用アニメ関係
	int animArray_;								//アタッチするアニメを格納
	int animArrayTotalTime_;				//アニメーションの総再生時間
	float stepAnimArray_;						//アニメーションの再生時間
	float speedAnimArray_;						//アニメーション速度

	//アニメーション終了時の動き
	virtual void FinishAnim(void);
	//カウンタ増加
	void CntUp(float& _count);
	//カウンタ減少
	void CntDown(float& _count);
};

