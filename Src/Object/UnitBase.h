#pragma once
#include"Common/Transform.h"
#include<unordered_map>


class UnitBase
{
public:

	//攻撃関係
	struct ATK
	{
		VECTOR pos_;		//位置
		float pow_;			//攻撃力
		float duration_;	//持続時間（攻撃がどれくらい続くかを記述)
		float backlash_;	//後隙（後隙がどれくらい続くかを記述)
		float cnt_;			//カウンター

	//攻撃中かどうか
		const bool IsAttack(void)const { return 0 < cnt_ && cnt_ <= duration_; };
	//後隙がどうか
		const bool IsBacklash(void)const { return 0 < (cnt_ - duration_) && (cnt_ - duration_) <= backlash_; };
	//モーションが終了しているか
		const bool IsFinishMotion(void)const { return cnt_ > (duration_ + backlash_); };
	//カウンターリセット
		void ResetCnt(void) { cnt_ = 0; };
	};

	enum class ANIM
	{
		IDLE,		//待機
		WALK,		//歩き
		RUN,		//走り
		SKILL_1,	//スキル①
		SKILL_2,	//スキル②
		DODGE,		//回避
		DAMAGE,		//被ダメ
		DEATH,		//死亡
		ENTRY,		//出現
		UNIQUE_1,	//固有のやつ①
		UNIQUE_2,	//固有のやつ②
	};

	//定数
	// アニメーションの再生速度(仮）
	static constexpr float SPEED_ANIM = 20.0f;


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
	const bool IsAlive(void)const {return hp_ > 0;};

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

	//アニメーション関数
	void Anim(void);
	//アニメーションリセット
	void ResetAnim(const ANIM _anim,const float _speed);

protected:

	int hp_;			//体力
	Transform trans_;	//位置情報関係
	float def_;			//防御力

	//アニメ関係
	ANIM anim_;								//アニメステート
	std::unordered_map<ANIM, int> animNum_;	//アニメーションナンバー格納配列。
	int atcAnim_;							//アタッチするアニメを格納
	int animTotalTime_;						//アニメーションの総再生時間
	float stepAnim_;						//アニメーションの再生時間
	float speedAnim_;						//アニメーション速度

	//アニメーション終了時の動き
	virtual void FinishAnim(void);
};

