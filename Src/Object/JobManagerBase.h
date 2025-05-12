#pragma once
#include"../Manager/GameSystem/DataBank.h"
#include"Character/PlayableChara/PlayerBase.h"
class PlayerBase;
class PlayerDodge;
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

	//プレイヤーNoをセットして座標を決める
	void SetPlayerNum(const int _num) { playerNum_ = _num; }

	//初期座標にプレイヤーを置く
	void SetInitPos(int _num);

protected:

	PlayerBase* obj_;	//使用するインスタンス格納
	DataBank::PLAYER_INFO info_;
	InputManager::JOYPAD_NO padNum_;		//パッド番号
	virtual void AtkInput(void)=0;
	virtual void SkillOneInput(void)=0;
	virtual void SkillTwoInput(void)=0;

	int playerNum_;		//何Pか(初期座標を置く用)

	//攻撃系以外の入力処理(回避とかスキルチェンジとか)
	void ActionInput(PlayerBase* _player,PlayerDodge* _dodge);

};