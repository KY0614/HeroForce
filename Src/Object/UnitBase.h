#pragma once
#include"Common/Transform.h"


//test

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
		const bool IsBacklash_(void) { return 0 < (cnt_ - duration_) && (cnt_ - duration_) <= backlash_; };
	//カウンターリセット
		void ResetCnt(void) { cnt_ = 0; };
	};


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
	const Transform& GetTransform(void)const { return trans_;};

	//位置
	const VECTOR GetPos(void)const {return trans_.pos;};
	//角度
	const VECTOR GetRot(void)const {return trans_.rot;};
	//大きさ
	const VECTOR GetScl(void)const {return trans_.scl;};
	//防御力
	const float GetDef(void)const {return def_;};

protected:

	int mdlId_;			//モデル保存
	int hp_;			//体力
	Transform trans_;	//位置情報関係
	float def_;			//防御力

};

