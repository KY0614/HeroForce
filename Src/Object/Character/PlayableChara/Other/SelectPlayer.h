#pragma once
#include"../../../../Utility/AsoUtility.h"
#include"../../../Common/Transform.h"
#include"../Manager/Generic/SceneManager.h"
#include "../../../UnitBase.h"

class SelectPlayer : public UnitBase
{
public:
	static constexpr float ROT_SPEED = 0.5f;

	static constexpr float ANIM_SPEED = 20.0f;
	static constexpr int IDLE_ANIM = 36;

	//コンストラクタ
	SelectPlayer(void);

	//デストラクタ
	~SelectPlayer(void) = default;

	//解放
	virtual void Destroy(void);

	//基本処理の４つは仮想関数化するのでしっかりオーバーライドするように
	//初期化
	virtual void Init(void)override;
	//更新
	virtual void Update(void)override;
	//描画
	virtual void Draw(void)override;

	//セッター
	void SetRole(int role){ role_ = role; };

	void SetPos(VECTOR pos);

	void SetRot(Quaternion quo) { for (auto& tran : trans_) { tran.quaRotLocal = quo; } };

	//VECTOR GetPos(void) { for (auto& tran : trans_) { return tran.pos; } };
	VECTOR GetPos(void) { for (auto& tran : transArray_) { return tran.pos; } };
	VECTOR GetPosArray(int i) { return transArray_[i].pos; };
	
	//void Anim(void);

private:

	Transform trans_[SceneManager::PLAYER_NUM];
	int role_;

	void Init3DModel(void);
};