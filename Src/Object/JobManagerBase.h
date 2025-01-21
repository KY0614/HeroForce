#pragma once
#include"Common/Transform.h"
#include"UnitBase.h"
#include"Character/PlayableChara/PlayerBase.h"

class JobManagerBase
{
public:

	JobManagerBase(void);
	~JobManagerBase(void);

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;

	PlayerBase* GetPlayer(void) { return obj_; }

	//Transformいただき
	const Transform& GetTransform(void)const { return obj_->GetTransform(); }

	//あたり判定
	const float GetRadius(void)const { return obj_->GetRadius(); }

	//位置
	const VECTOR GetPos(void)const { return obj_->GetPos(); }

	//防御力
	const float GetDef(void)const { return obj_->GetDef(); }
	//攻撃関係
	const UnitBase::ATK GetAtk(void)const { return obj_->GetAtk(); }

	//前のフレーム位置に置く
	void SetPrePos(void) { obj_->SetPrePos(); }

protected:

	PlayerBase* obj_;	//使用するインスタンス格納
};

