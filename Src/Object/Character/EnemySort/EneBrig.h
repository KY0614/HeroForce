#pragma once
#include"Enemy.h"

class EneBrig : public Enemy
{
public:
	//****************************************************************
	//定数(キャラ固有)
	//****************************************************************

	//アニメーション番号(キャラ固有)
	static constexpr int ANIM_SKILL_ONE = 96;	//スキル1アニメーション(振りかざし)
	static constexpr int ANIM_PRE_SWING = 95;	//スキル1アニメーション(振りかぶり)

	//モデル関係
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,100.0f * CHARACTER_SCALE,0.0f };	//モデルの中心座標への相対座標

	//フレーム(ボーン)番号
	static constexpr int FRAME_KNIFE = 63;		//ナイフ

	//攻撃関係
	static constexpr float ALERT_TIME = 0.4f;	//攻撃の警告時間
	static constexpr float BREAK_TIME = 0.8f;	//攻撃の休憩時間

	//範囲関係
	static constexpr float SEARCH_RANGE = 1000.0f * CHARACTER_SCALE;		//索敵判定の大きさ
	static constexpr float ATK_START_RANGE = 200.0f * CHARACTER_SCALE;		//攻撃開始判定の大きさ

	//スキルの当たり判定半径
	static constexpr float SKILL_ONE_COL_RADIUS = 30.0f;	//スキル１の当たり判定半径

	//スキルの攻撃力
	static constexpr float SKILL_ONE_POW = 20.0f;			//スキル１の攻撃力

	//スキルの持続時間
	static constexpr float SKILL_ONE_DURATION = 1.0f;		//スキル１の持続時間

	//スキルの後隙
	static constexpr float SKILL_ONE_BACKLASH = 1.0f;		//スキル１の後隙

	//スキル関係
	static constexpr ATK SKILL_ONE = { AsoUtility::VECTOR_ZERO	//スキル１の要素
		,SKILL_ONE_COL_RADIUS
		,SKILL_ONE_POW
		,SKILL_ONE_DURATION
		,SKILL_ONE_BACKLASH
		,0.0f };

	//コンストラクタ
	EneBrig(const VECTOR& _spawnPos);

private:
	//****************************************************************
	//関数
	//****************************************************************

	//キャラ固有設定
	void SetParam(void)override;

	//アニメーション番号の初期化
	void InitAnim(void)override;

	//スキルの初期化
	void InitSkill(void)override;

	//警告時間中かどうかを返す
	const bool IsAlertTime(void)const override { return alertCnt_ < ALERT_TIME; }
	//休憩時間中かどうかを返す
	const bool IsBreak(void)const override { return breakCnt_ < BREAK_TIME; }

	//スキル1の警告
	void AlertSkill_One(void)override;

	//スキル1
	void Skill_One(void)override;

	//アニメーション終了時の動き
	void FinishAnim(void)override;
};

